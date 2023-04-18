#include "service_configuration.h"


namespace streets_service {
    // Constructor
    service_configuration::service_configuration( const std::string &filepath ): filepath(filepath){
        // Parse manifest.json configuration file into rapidjson::Document
        rapidjson::Document doc = parse_configuration_file();
        // Use service level configuration parameters from Document (i.e. loglevel and service_name)
        // to configure spdlog default logger (terminal and file sinks)
        configure_logger(doc);
        // Use configurations array to populate configuration map with service specific configuration
        // parameters from Document
        update_configuration(doc);
        SPDLOG_INFO("Printing Configuration Parameters ---------------------------------------");
        for(const auto& conf : configuration_map)
        {
            SPDLOG_INFO("{0} : {1} ", conf.first.c_str(), conf.second.value.c_str());
        }
    };

    rapidjson::Document service_configuration::parse_configuration_file() {
        // Open file
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw streets_service::streets_configuration_exception("Unable to open Streets configuration file " + filepath + " !"); 
        }
        // Add file contents to stream and parse stream into Document
        rapidjson::IStreamWrapper isw(file);
        rapidjson::Document doc;
        doc.ParseStream(isw);
        if (doc.HasParseError()){
            SPDLOG_ERROR("Error  : {0} Offset: {1} ", doc.GetParseError(), doc.GetErrorOffset());
            throw streets_service::streets_configuration_exception("Document parse error!");
        }
        file.close();
        return doc;
    };

    void service_configuration::configure_logger( const rapidjson::Document &doc ) {
        if ( doc.HasMember("service_name") && doc.FindMember("service_name")->value.IsString() ) {
            set_service_name(doc.FindMember("service_name")->value.GetString());
            create_default_logger( _service_name );
        }
        else {
            SPDLOG_WARN("Parameter \"service_name\" missing/incorrectly formatted in manifest.json! Setting \"service_name\" to streets_service!");
            create_default_logger("streets_service");
        }
        if ( doc.HasMember("loglevel") && doc.FindMember("loglevel")->value.IsString() ) {
            set_loglevel( doc.FindMember("loglevel")->value.GetString() );
        }
        else {
            SPDLOG_WARN("Parameter \"loglevel\" missing/incorrectly formatted in manifest.json! Setting \"loglevel\" to INFO!");
            set_loglevel("info");
        }
    }

    void service_configuration::update_configuration( const rapidjson::Document &doc) {
        SPDLOG_DEBUG("Updating Configuration Map");
        if ( doc.FindMember("configurations")->value.IsArray() ) {
            parse_configurations_array(doc.FindMember("configurations")->value.GetArray());
        }
        else {
            SPDLOG_WARN("No configurations found in manifest.json!");
        }   
    };

    void service_configuration::update_log_level( const rapidjson::Document &doc ){
        if ( doc.FindMember("loglevel")->value.IsString() ) {
            set_loglevel( doc.FindMember("loglevel")->value.GetString() );
        }
        else {
            SPDLOG_WARN("Parameter \"loglevel\" missing/incorrectly formatted in manifest.json! Setting \"loglevel\" to INFO!");
            set_loglevel("info");
        }
    };

    void service_configuration::parse_configurations_array( const rapidjson::GenericArray<true, rapidjson::Value> &arr) {
        std::unique_lock<std::mutex> lck(config_lock);
        for ( auto& cnf: arr ) {
            std::string property_name = cnf.FindMember("name")->value.GetString();
            auto val = configuration_map.find( property_name );
            if ( val != configuration_map.end() && cnf.IsObject()) {
                configuration config = val->second;
                configuration file_config;
                file_config.fromJson( cnf.GetObject());
                if ( config != file_config ) {
                    SPDLOG_INFO("Updating configuration {0}!", property_name);
                    configuration_map.erase(property_name);
                    configuration_map.insert(std::make_pair(property_name, file_config));
                }
            }
            else if ( val == configuration_map.end() && cnf.IsObject() ) {
                configuration file_config;
                file_config.fromJson(cnf.GetObject() );
                configuration_map.insert(std::make_pair(property_name, file_config));
            }
            else {
                throw streets_service::streets_configuration_exception("Configuration parameter " + property_name + " not properly formatted!");
            }  
        }
    };

    
    void service_configuration::set_loglevel(const std::string &loglevel ) const{
        // Get main logger and set loglevel
        auto main_logger = spdlog::get("main");
        if ( main_logger != nullptr ) {
            main_logger->set_level(spdlog::level::from_str(loglevel));
            spdlog::set_level(spdlog::level::from_str(loglevel));
            SPDLOG_INFO( "Log Level set to {0}!", spdlog::level::to_string_view(spdlog::get_level()));
        }
        else {
            throw streets_configuration_exception("Log Level set failed! No main logger configured!");
        }
    }

    void service_configuration::create_default_logger(const std::string &service_name ) const{
        try {
            // Create logger thread pool
            spdlog::init_thread_pool(8192, 1);
            // Create logger file and stdout sinks
            // Rotate files at 11:59 PM
            auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("../logs/" + service_name, 23, 59, false, 3);
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            // Link thread pool and sinks to async logger
            auto logger = std::make_shared<spdlog::async_logger>("main",  spdlog::sinks_init_list({console_sink, file_sink}),spdlog::thread_pool());
            // Set pattern [2022-3-31 13:00:00.000] [loglevel] [file_name.cpp:123]
            logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%#] %v");
            // Register logger and set as default
            spdlog::set_default_logger(logger);
            SPDLOG_INFO("Default Logger initialized!");
        }   
        catch (const spdlog::spdlog_ex& ex)
        {
            SPDLOG_ERROR( "Log initialization failed: {0}!",ex.what());
        }
    };


    void service_configuration::check_update() {
        try {
            // Check last write time of manifest.json configuration file to see if updates have been made
            std::time_t time = boost::filesystem::last_write_time(filepath);
            SPDLOG_TRACE("Last Modified Time {0} vs Stored Last Modified Time {1}", time, last_modified);
            if ( time > last_modified) {
                // If updates have been made parse manifest.json into Document and update loglevel and 
                // any changed configuration parameters.
                rapidjson::Document doc = parse_configuration_file();
                update_log_level(doc);
                update_configuration(doc);
                // Set new update time to the last update read
                last_modified = time;
            }
        }
        catch (const std::exception &e) {
            throw streets_configuration_exception(e.what());
        }
    }

    void service_configuration::set_service_name(const std::string &service_name) {
        _service_name = service_name;
    }   

}








