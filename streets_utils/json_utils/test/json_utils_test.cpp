#include <gtest/gtest.h>
#include <json_utils.hpp>
#include <json_utils_exception.hpp>

using namespace streets_utils::json_utils;

//---------------------test validate_json---------------------
TEST(json_utils_test, test_validate_invalid_json) {
    // Empty String
    std::string invalid_json = "";
    EXPECT_THROW( validate_json(invalid_json), json_utils_exception);

    // Property missing quotations
    invalid_json = "{ some_propert: \"some_value\"}";
    EXPECT_THROW( validate_json(invalid_json), json_utils_exception);

}

TEST(json_utils_test, test_validate_valid_json) {
    // Correct JSON
    std::string valid_json = "{ \"some_property\": \"some_value\"}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_FALSE(parsed_doc.HasParseError());
}
//---------------------test parse_uint_member---------------------
TEST(json_utils_test, test_get_json_uint_required_property_present){
    // Test with required property present
    std::string valid_json = "{ \"some_property\": 12345}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_EQ( 12345, parse_uint_member("some_property", parsed_doc, true));
}

TEST(json_utils_test, test_get_json_uint_required_property_not_present){
    // Test with required property no present
    std::string valid_json = "{ \"some_property_other\": 12345}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_THROW( parse_uint_member("some_property", parsed_doc, true), json_utils_exception);
}

TEST(json_utils_test, test_get_json_uint_required_property_wrong_type){
    // Test with required property present with wrong type
    std::string valid_json = "{ \"some_property\": -12345}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_THROW( parse_uint_member("some_property", parsed_doc, true), json_utils_exception);
}

TEST(json_utils_test, test_get_json_uint_optional_property_missing) {
    // Test with required property no present
    std::string valid_json = "{ \"some_property_other\": 12345}";
    auto parsed_doc = validate_json(valid_json);
    auto property = parse_uint_member("some_property", parsed_doc, false);
    EXPECT_FALSE( property.has_value());
}
//---------------------test get_json_int_property---------------------

TEST(json_utils_test, test_get_json_int_required_property_present){
    // Test with required property present
    std::string valid_json = "{ \"some_property\": -12345}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_EQ( -12345, parse_int_member("some_property", parsed_doc, true));
}

TEST(json_utils_test, test_get_json_int_required_property_not_present){
    // Test with required property no present
    std::string valid_json = "{ \"some_property_other\": 12345}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_THROW( parse_int_member("some_property", parsed_doc, true), json_utils_exception);
}

TEST(json_utils_test, test_get_json_int_required_property_wrong_type){
    // Test with required property present with wrong type
    std::string valid_json = "{ \"some_property\": true}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_THROW( parse_int_member("some_property", parsed_doc, true), json_utils_exception);
}

TEST(json_utils_test, test_get_json_int_optional_property_missing) {
    // Test with required property no present
    std::string valid_json = "{ \"some_property_other\": 12345}";
    auto parsed_doc = validate_json(valid_json);
    auto property = parse_int_member("some_property", parsed_doc, false);
    EXPECT_FALSE( property.has_value());
}
//---------------------test parse_bool_property---------------------

TEST(json_utils_test, test_get_json_bool_required_property_present){
    // Test with required property present
    std::string valid_json = "{ \"some_property\": true}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_EQ( true, parse_bool_member("some_property", parsed_doc, true));
}

TEST(json_utils_test, test_get_json_bool_required_property_not_present){
    // Test with required property no present
    std::string valid_json = "{ \"some_property_other\": true}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_THROW( parse_bool_member("some_property", parsed_doc, true), json_utils_exception);
}

TEST(json_utils_test, test_get_json_bool_required_property_wrong_type){
    // Test with required property present with wrong type
    std::string valid_json = "{ \"some_property\": 1234}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_THROW( parse_bool_member("some_property", parsed_doc, true), json_utils_exception);
}

TEST(json_utils_test, test_get_json_bool_optional_property_missing) {
    // Test with required property no present
    std::string valid_json = "{ \"some_property_other\": 12345}";
    auto parsed_doc = validate_json(valid_json);
    auto property = parse_bool_member("some_property", parsed_doc, false);
    EXPECT_FALSE( property.has_value());
}

//---------------------test get_json_double_property---------------------

TEST(json_utils_test, test_get_json_double_required_property_present){
    // Test with required property present
    std::string valid_json = "{ \"some_property\": 12.3}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_NEAR( 12.3, parse_double_member("some_property", parsed_doc, true).value(), 0.01);
}

TEST(json_utils_test, test_get_json_double_required_property_not_present){
    // Test with required property no present
    std::string valid_json = "{ \"some_property_other\": 12.3}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_THROW( parse_double_member("some_property", parsed_doc, true), json_utils_exception);
}

TEST(json_utils_test, test_get_json_double_required_property_wrong_type){
    // Test with required property present with wrong type
    std::string valid_json = "{ \"some_property\": 1234}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_THROW( parse_double_member("some_property", parsed_doc, true), json_utils_exception);
}

TEST(json_utils_test, test_get_json_double_optional_property_missing) {
    // Test with required property no present
    std::string valid_json = "{ \"some_property_other\": 12345}";
    auto parsed_doc = validate_json(valid_json);
    auto property = parse_double_member("some_property", parsed_doc, false);
    EXPECT_FALSE( property.has_value());
}
//---------------------test parse_string_property---------------------

TEST(json_utils_test, test_get_json_string_required_property_present){
    // Test with required property present
    std::string valid_json = "{ \"some_property\": \"some_property\" }";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_EQ("some_property", parse_string_member("some_property", parsed_doc, true));
}

TEST(json_utils_test, test_get_json_string_required_property_not_present){
    // Test with required property no present
    std::string valid_json = "{ \"some_property_other\": 12.3}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_THROW( parse_string_member("some_property", parsed_doc, true), json_utils_exception);
}

TEST(json_utils_test, test_get_json_string_required_property_wrong_type){
    // Test with required property present with wrong type
    std::string valid_json = "{ \"some_property\": 1234}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_THROW( parse_string_member("some_property", parsed_doc, true), json_utils_exception);
}

TEST(json_utils_test, test_get_json_string_optional_property_missing) {
    // Test with required property no present
    std::string valid_json = "{ \"some_property_other\": 12345}";
    auto parsed_doc = validate_json(valid_json);
    auto property = parse_string_member("some_property", parsed_doc, false);
    EXPECT_FALSE( property.has_value());
}

//---------------------test parse_object_property---------------------

TEST(json_utils_test, test_get_json_object_required_property_present){
    // Test with required property present
    std::string valid_json = "{ "
                "\"some_object\": {"
                    "\"object_name\" : \"object\","
                    "\"object_value\" : 123"
                "}" 
            "}";
    auto parsed_doc = validate_json(valid_json);
    auto object = parse_object_member("some_object", parsed_doc, true);
    EXPECT_EQ("object", parse_string_member("object_name", object.value(), true));
    EXPECT_EQ( 123, parse_int_member("object_value", object.value(), true) );

}

TEST(json_utils_test, test_get_json_object_required_property_not_present){
    // Test with required property no present
    std::string valid_json = "{ "
                "\"some_other_object\": {"
                    "\"object_name\" : \"object\","
                    "\"object_value\" : 123"
                "}" 
            "}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_THROW( parse_object_member("some_object", parsed_doc, true), json_utils_exception);
}

TEST(json_utils_test, test_get_json_object_required_property_wrong_type){
    // Test with required property present with wrong type
     std::string valid_json = "{ "
                "\"some_object\": [{"
                    "\"object_name\" : \"object\","
                    "\"object_value\" : 123"
                "}]" 
            "}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_THROW( parse_object_member("some_object", parsed_doc, true), json_utils_exception);
}

TEST(json_utils_test, test_get_json_object_optional_property_missing) {
    // Test with required property no present
    std::string valid_json = "{ \"some_property_other\": 12345}";
    auto parsed_doc = validate_json(valid_json);
    auto property = parse_object_member("some_property", parsed_doc, false);
    EXPECT_FALSE( property.has_value());
}
//---------------------test parse_array_property---------------------

TEST(json_utils_test, test_get_json_array_required_property_present){
    // Test with required property present
    std::string valid_json = "{ "
                "\"some_array\": "
                    "[456, 2452, -1232, 2345]" 
                "}";
    auto parsed_doc = validate_json(valid_json);
    auto array = parse_array_member("some_array", parsed_doc, true);

    EXPECT_EQ(4, array.value().Size());
    EXPECT_EQ(456, array.value()[0].GetInt());
    EXPECT_EQ(2452, array.value()[1].GetInt());
    EXPECT_EQ(-1232, array.value()[2].GetInt());
    EXPECT_EQ(2345, array.value()[3].GetInt());
}

TEST(json_utils_test, test_get_json_array_required_property_not_present){
    // Test with required property no present
    std::string valid_json = "{ "
                "\"some_other_array\": "
                    "[456, 2452, -1232, 2345]" 
                "}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_THROW( parse_array_member("some_array", parsed_doc, true), json_utils_exception);
}

TEST(json_utils_test, test_get_json_array_required_property_wrong_type){
    // Test with required property present with wrong type
     std::string valid_json = "{ "
                "\"some_array\": [{"
                    "\"object_name\" : \"object\","
                    "\"object_value\" : 123"
                "}]" 
            "}";
    auto parsed_doc = validate_json(valid_json);
    EXPECT_THROW( parse_array_member("some_object", parsed_doc, true), json_utils_exception);
}

TEST(json_utils_test, test_get_json_array_optional_property_missing) {
    // Test with required property no present
    std::string valid_json = "{ \"some_property_other\": 12345}";
    auto parsed_doc = validate_json(valid_json);
    auto property = parse_array_member("some_property", parsed_doc, false);
    EXPECT_FALSE( property.has_value());
}
//---------------------test write optional property---------------------
// TODO: Functionality not working yet,
// TEST(json_utils_test, test_write_option_property){
//     rapidjson::Value obj(rapidjson::kObjectType);
//     rapidjson::Document doc;
//     // Write optional present value
//     std::optional<double> double_optional;
//     double_optional = 43.2;
//     write_optional_member("some_double_property", double_optional, obj,  doc.GetAllocator());
//     EXPECT_TRUE(obj.HasMember("some_double_property"));
//     EXPECT_TRUE(obj.FindMember("some_double_property")->value.IsNumber());
//     EXPECT_NEAR(43.2, obj.FindMember("some_double_property")->value.GetDouble(),  0.01 );

//     std::optional<std::string> string_optional_not_present;
//     // write_optional_member("some_string_property", string_optional_not_present, obj,  doc.GetAllocator());

//     // EXPECT_FALSE( obj.HasMember("some_string_property"));
// }

