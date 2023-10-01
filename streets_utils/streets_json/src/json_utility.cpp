#include "json_utility.hpp"

namespace streets_utils::json_utils {

    rapidjson::Document validate_json(const std::string &json) {
        rapidjson::Document doc;
        doc.Parse(json);
        if (doc.HasParseError())
        {
            throw json_parsing_exception("Message JSON is misformatted. JSON parsing failed!");
        }
        return doc;
    }
    std::optional<int64_t> parse_int_property(const std::string &prop_name, const rapidjson::Value &doc, bool required ){
        std::optional<int64_t> property;
        if (doc.HasMember(prop_name.c_str()) && doc.FindMember(prop_name.c_str())->value.IsInt64())
        {
            property = doc[prop_name.c_str()].GetInt64();
        }
        else if (required)
        {
            throw json_parsing_exception("Missing or incorrect type for required property " + prop_name + "!");
        }
        return property;
    }

    std::optional<uint64_t> parse_uint_property(const std::string &prop_name,  const rapidjson::Value &doc, bool required ) {
        std::optional<u_int64_t> property;
        if (doc.HasMember(prop_name.c_str()) && doc.FindMember(prop_name.c_str())->value.IsUint64())
        {
            property = doc[prop_name.c_str()].GetUint64();
        }
        else if (required)
        {
            throw json_parsing_exception("Missing or incorrect type for required property " + prop_name + "!");
        }
        return property;
    };

    std::optional<bool> parse_bool_property(const std::string &prop_name,  const rapidjson::Value &doc, bool required ) {
        std::optional<bool> property;
        if (doc.HasMember(prop_name.c_str()) && doc.FindMember(prop_name.c_str())->value.IsBool())
        {
            property = doc[prop_name.c_str()].GetBool();
        }
        else if (required)
        {
            throw json_parsing_exception("Missing or incorrect type for required property " + prop_name + "!");
        }
        return property;
    };

    std::optional<std::string> parse_string_property(const std::string &prop_name,  const rapidjson::Value &doc, bool required ){
        std::optional<std::string> property;
        if (doc.HasMember(prop_name.c_str()) && doc.FindMember(prop_name.c_str())->value.IsString())
        {
            property = doc[prop_name.c_str()].GetString();
        }
        else if (required)
        {
            throw json_parsing_exception("Missing or incorrect type for required property " + prop_name + "!");
        }
        return property;
    };

    std::optional<double> parse_double_property(const std::string &prop_name, const rapidjson::Value &doc, bool required) {
        std::optional<double> property;
        if (doc.HasMember(prop_name.c_str()) && doc.FindMember(prop_name.c_str())->value.IsDouble())
        {
            property = doc[prop_name.c_str()].GetDouble();
        }
        else if (required)
        {
            throw json_parsing_exception("Missing or incorrect type for required property " + prop_name + "!");
        }
        return property;
    };

    std::optional<rapidjson::Value::ConstObject> parse_object_property(const std::string &prop_name, const rapidjson::Value &doc, bool required) {
        if (doc.HasMember(prop_name.c_str()) && doc.FindMember(prop_name.c_str())->value.IsObject())
        {
            return doc[prop_name.c_str()].GetObject();
        }
        else if (required)
        {
            throw json_parsing_exception("Missing or incorrect type for required property " + prop_name + "!");
        }
        return std::nullopt;
    }

    std::optional<rapidjson::Value::ConstArray> parse_array_property(const std::string &prop_name, const rapidjson::Value &doc, bool required) {
        if (doc.HasMember(prop_name.c_str()) && doc.FindMember(prop_name.c_str())->value.IsArray())
        {
            return doc[prop_name.c_str()].GetArray();
        }
        else if (required)
        {
            throw json_parsing_exception("Missing or incorrect type for required property " + prop_name + "!");
        }
        return std::nullopt;
    }



}