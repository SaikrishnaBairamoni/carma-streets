
#include "gtest/gtest.h"
#include "vehicle.h"
#include "osm.h"
#include "rapidjson/document.h"
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"


TEST(test_vehicle, update)
{

    osm localmap("osm.json");

    vehicle test_veh;

    /* these set of tests make sure that the scheduling service has succesfully added the vehicle information for the first time */
    const char* paylod = "{\"metadata\": {\"timestamp\": 1623677096000}, \"payload\": {\"v_id\": \"DOT-507\", \"v_length\": 5, \"min_gap\": 10, \"react_t\": 1.5, \"max_accel\": 5, \"max_decel\": 5, \"cur_speed\": 5.0, \"cur_accel\": 0.0, \"lat\": 38.9012, \"lng\": -77.21289, \"cur_lane_id\": 0, \"cur_ds\": 7.0, \"direction\": \"straight\", \"entry_lane_id\": 0, \"link_lane_id\": 8, \"is_allowed\": false, \"est_paths\": [{\"ts\": 1623677096200, \"lat\": 38.9012, \"lng\":-77.21289, \"id\": 0, \"ds\": 6}, {\"ts\": 1623677096400, \"lat\": 38.9012, \"lng\":-77.21289, \"id\": 0, \"ds\": 5}, {\"ts\": 1623677096600, \"lat\": 38.9012, \"lng\":-77.21289, \"id\": 0, \"ds\": 4}]}}";

    rapidjson::Document message;
    message.SetObject();
    message.Parse(paylod);

    test_veh.update(message, localmap);

    ASSERT_EQ("DOT-507", test_veh.get_id());
    ASSERT_EQ(5, test_veh.get_length());
    ASSERT_EQ(10, test_veh.get_minGap());
    ASSERT_EQ(1.5, test_veh.get_reactionT());
    ASSERT_EQ(5, test_veh.get_accelMax());
    ASSERT_EQ(-5, test_veh.get_decelMax());
    ASSERT_EQ("straight", test_veh.get_direction());
    ASSERT_EQ("0", test_veh.get_entryLaneID());
    ASSERT_EQ("8", test_veh.get_linkID());
    ASSERT_LE(0, test_veh.get_linkPriority());
    ASSERT_LE(0, test_veh.get_departurePosition());
    ASSERT_LE(0, test_veh.get_flexET());
    ASSERT_LE(0, test_veh.get_flexST());
    ASSERT_EQ(false, test_veh.get_access());

    ASSERT_EQ(1623677096.000, test_veh.get_curTime());
    ASSERT_EQ(7, test_veh.get_curDistance());
    ASSERT_EQ(5, test_veh.get_curSpeed());
    ASSERT_EQ(0, test_veh.get_curAccel());
    ASSERT_EQ("0", test_veh.get_curLaneID());

    ASSERT_EQ("EV", test_veh.get_curState());
    EXPECT_EQ(-1, test_veh.get_actualST());
    EXPECT_EQ(-1, test_veh.get_actualET());
    EXPECT_EQ(-1, test_veh.get_actualDT());

    // if (test_veh.get_curLaneID() == test_veh.get_entryLaneID()){
    //     if (test_veh.get_curDistance() < 2 && test_veh.get_curSpeed() <= 0.1){
    //         ASSERT_EQ("RDV", test_veh.get_curState());
    //         ASSERT_EQ(1623677096.000, test_veh.get_actualST());
    //         EXPECT_EQ(-1, test_veh.get_actualET());
    //         EXPECT_EQ(-1, test_veh.get_actualDT());
    //     } else{
    //         ASSERT_EQ("EV", test_veh.get_curState());
    //         EXPECT_EQ(-1, test_veh.get_actualST());
    //         EXPECT_EQ(-1, test_veh.get_actualET());
    //         EXPECT_EQ(-1, test_veh.get_actualDT());
    //     }
    // }
    // else if (test_veh.get_curLaneID() == test_veh.get_linkID()){
    //     ASSERT_EQ("DV", test_veh.get_curState());
    //     ASSERT_EQ(1623677096.000, test_veh.get_actualET());
    //     ASSERT_LE(test_veh.get_actualST(), test_veh.get_actualET());
    //     ASSERT_GT(test_veh.get_actualST(), -1);
    //     EXPECT_EQ(-1, test_veh.get_actualDT());
    // }
    // else{
    //     ASSERT_EQ("LV", test_veh.get_curState());
    //     ASSERT_EQ(1623677096.000, test_veh.get_actualDT());
    //     ASSERT_LE(test_veh.get_actualET(), test_veh.get_actualDT());
    //     ASSERT_LE(test_veh.get_actualST(), test_veh.get_actualET());
    //     ASSERT_GT(test_veh.get_actualET(), -1);
    //     ASSERT_GT(test_veh.get_actualST(), -1);
    // }

    ASSERT_EQ(3, test_veh.get_futureInfo().size());
    for (int i = 0; i < test_veh.get_futureInfo().size(); ++i){
        ASSERT_EQ(1623677096.000 + (i + 1)*0.2, test_veh.get_futureInfo()[i].timestamp);
        ASSERT_EQ("0", test_veh.get_futureInfo()[i].lane_id);
        ASSERT_EQ(7 - (i + 1)*1, test_veh.get_futureInfo()[i].distance);
    }

    /* these set of tests make sure that the scheduling service has succesfully updated the vehicle information from the previous update */
    message["metadata"]["timestamp"].SetInt64(1623677097000);
    message["payload"]["cur_ds"].SetDouble(2);
    message["payload"]["cur_speed"].SetDouble(2);
    message["payload"]["cur_accel"].SetDouble(-3);
    test_veh.update(message, localmap);
    ASSERT_EQ(1623677097.000, test_veh.get_curTime());
    ASSERT_EQ(2, test_veh.get_curDistance());
    ASSERT_EQ(2, test_veh.get_curSpeed());
    ASSERT_EQ(-3, test_veh.get_curAccel());
    ASSERT_EQ("0", test_veh.get_curLaneID());
    ASSERT_EQ("EV", test_veh.get_curState());

    /* these set of tests make sure that the scheduling service does not replace the current vehicle information with an old update */
    message["metadata"]["timestamp"].SetInt64(1623677096000);
    message["payload"]["cur_ds"].SetDouble(7);
    message["payload"]["cur_speed"].SetDouble(5);
    message["payload"]["cur_accel"].SetDouble(0);
    test_veh.update(message, localmap);
    ASSERT_EQ(1623677097.000, test_veh.get_curTime());
    ASSERT_EQ(2, test_veh.get_curDistance());
    ASSERT_EQ(2, test_veh.get_curSpeed());
    ASSERT_EQ(-3, test_veh.get_curAccel());
    ASSERT_EQ("0", test_veh.get_curLaneID());
    ASSERT_EQ("EV", test_veh.get_curState());

    /* these set of tests make sure that the scheduling service can succesfully updated the vehicle state */
    message["metadata"]["timestamp"].SetInt64(1623677098000);
    message["payload"]["cur_ds"].SetDouble(0);
    message["payload"]["cur_speed"].SetDouble(0);
    message["payload"]["cur_accel"].SetDouble(-2);
    test_veh.update(message, localmap);
    ASSERT_EQ(1623677098.000, test_veh.get_curTime());
    ASSERT_EQ(0, test_veh.get_curDistance());
    ASSERT_EQ(0, test_veh.get_curSpeed());
    ASSERT_EQ(-2, test_veh.get_curAccel());
    ASSERT_EQ("0", test_veh.get_curLaneID());
    ASSERT_EQ("RDV", test_veh.get_curState());
    EXPECT_EQ(1623677098.000, test_veh.get_actualST());

    message["metadata"]["timestamp"].SetInt64(1623677099000);
    message["payload"]["cur_accel"].SetDouble(0);
    test_veh.update(message, localmap);
    ASSERT_EQ(1623677099.000, test_veh.get_curTime());
    ASSERT_EQ("RDV", test_veh.get_curState());
    EXPECT_EQ(1623677098.000, test_veh.get_actualST());

    message["metadata"]["timestamp"].SetInt64(1623677100000);
    message["payload"]["is_allowed"].SetBool(true);
    test_veh.update(message, localmap);
    ASSERT_EQ(1623677100.000, test_veh.get_curTime()); 
    ASSERT_EQ("8", test_veh.get_curLaneID());
    ASSERT_EQ(localmap.get_laneLength(test_veh.get_curLaneID()), test_veh.get_curDistance());
    ASSERT_EQ("DV", test_veh.get_curState());
    EXPECT_EQ(1623677098.000, test_veh.get_actualST());
    EXPECT_EQ(1623677100.000, test_veh.get_actualET());

    message["metadata"]["timestamp"].SetInt64(1623677101000);
    message["payload"]["cur_lane_id"].SetInt(8);
    message["payload"]["cur_ds"].SetDouble(18);
    message["payload"]["cur_speed"].SetDouble(2);
    message["payload"]["cur_accel"].SetDouble(2);
    test_veh.update(message, localmap);
    ASSERT_EQ(1623677101.000, test_veh.get_curTime());
    ASSERT_EQ(18, test_veh.get_curDistance());
    ASSERT_EQ(2, test_veh.get_curSpeed());
    ASSERT_EQ(2, test_veh.get_curAccel());
    ASSERT_EQ("8", test_veh.get_curLaneID());
    ASSERT_EQ("DV", test_veh.get_curState());
    EXPECT_EQ(1623677098.000, test_veh.get_actualST());
    EXPECT_EQ(1623677100.000, test_veh.get_actualET());

    message["metadata"]["timestamp"].SetInt64(1623677105000);
    message["payload"]["cur_lane_id"].SetInt(4);
    message["payload"]["cur_ds"].SetDouble(295);
    message["payload"]["cur_speed"].SetDouble(10);
    message["payload"]["cur_accel"].SetDouble(0);
    test_veh.update(message, localmap);
    ASSERT_EQ(1623677105.000, test_veh.get_curTime());
    ASSERT_EQ(295, test_veh.get_curDistance());
    ASSERT_EQ(10, test_veh.get_curSpeed());
    ASSERT_EQ(0, test_veh.get_curAccel());
    ASSERT_EQ("4", test_veh.get_curLaneID());
    ASSERT_EQ("LV", test_veh.get_curState());
    EXPECT_EQ(1623677098.000, test_veh.get_actualST());
    EXPECT_EQ(1623677100.000, test_veh.get_actualET());
    EXPECT_EQ(1623677105.000, test_veh.get_actualDT());

}