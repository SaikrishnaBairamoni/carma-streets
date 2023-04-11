#include "streets_service.h"

namespace streets_service {
    bool streets_service::initialize() {
        try {
            streets_configuration::create("../manifest.json");
            _simulation_mode = get_system_config("SIMULATION_MODE").compare("TRUE") == 0;
            std::string time_sync_topic = get_system_config("TIME_SYNC_TOPIC");
            streets_clock_singleton::create(_simulation_mode);
            _service_name = streets_configuration::get_service_name();
            if ( _simulation_mode ) {
                if (!initialize_kafka_consumer(time_sync_topic, _time_consumer)){
                    return false;
                }
            }
        } catch( const streets_configuration_exception &e) {
            SPDLOG_ERROR("Exception occured during {0} initialization : {1}" , _service_name , e.what());
            return false;
        }
        return true;

    }

    bool streets_service::initialize_kafka_producer( const std::string &producer_topic, std::shared_ptr<kafka_clients::kafka_producer_worker> &producer) {
        
        auto client = std::make_unique<kafka_clients::kafka_client>();
        std::string bootstrap_server = streets_configuration::get_string_config("bootstrap_server");

        producer = client->create_producer(bootstrap_server, producer_topic);
        if (!producer->init())
        {
            SPDLOG_CRITICAL("Kafka producer initialize error on topic {0}", producer_topic);
            return false;
        }
        SPDLOG_DEBUG("Initialized Kafka producer on topic {0}!", producer_topic);
        return true;
    }
 
    bool streets_service::initialize_kafka_consumer(const std::string &consumer_topic,  
                                                std::shared_ptr<kafka_clients::kafka_consumer_worker> &kafka_consumer) {
        auto client = std::make_unique<kafka_clients::kafka_client>();
        std::string bootstrap_server = streets_configuration::get_string_config("bootstrap_server");
        kafka_consumer = client->create_consumer(bootstrap_server, consumer_topic, _service_name);
        if (!kafka_consumer->init())
        {
            SPDLOG_CRITICAL("Kafka initialize error");
            return false;
        }
        SPDLOG_DEBUG("Initialized Kafka consumer!");
        return true;
    }

    void streets_service::consume_time_sync_message()  {
        _time_consumer->subscribe();
        while (_time_consumer->is_running())
        {
            const std::string payload = _time_consumer->consume(1000);
            if (payload.length() != 0)
            {
                SPDLOG_DEBUG("Consumed: {0}", payload);
                simulation::time_sync_message msg;
                msg.fromJson(payload);
                streets_clock_singleton::update(msg.timestep);
                
            }

        }        
    }

    void streets_service::start() {
        if (_simulation_mode ) {
            std::thread time_sync_thread(&streets_service::consume_time_sync_message, this);
            time_sync_thread.join();
        }
    }
}