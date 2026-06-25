#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "support/unity.h"
#include "../recorder.h"

static Packet *packets;
static uint32_t raw_time = 1782364564;

void setUp(void) {
    packets = calloc(1, sizeof(Packet));
}


void tearDown(void) {
    free(packets);
    packets = NULL;
}

void test_size_of_packet(void){
    Packet packet;

    TEST_ASSERT_EQUAL_INT(8, sizeof(packet));

}

void test_correct_string_parsing(void){

    char buffer[] = "$DATA,30,12,90#";

    parse(buffer, 0, &packets, raw_time);

    TEST_ASSERT_EQUAL_UINT8(1, packets[0].telemetry);
    TEST_ASSERT_EQUAL_INT(30, packets[0].speed);
    TEST_ASSERT_EQUAL_INT(12, packets[0].satellites);
    TEST_ASSERT_EQUAL_INT(90, packets[0].battery);

}

void test_check_empty_string(void){
    char empty_string[] = "";

    int result = parse(empty_string, 0, &packets, raw_time);

    TEST_ASSERT_TRUE(result);
}

void test_check_low_battery(void){
    char str[] = "$DATA,40,10,14#";

    int result = parse(str, 0, &packets, raw_time);

    TEST_ASSERT_TRUE(result);
}

void test_write_to_file(void){
    packets[0].time = raw_time;

    packets[0].telemetry = 2 ;

    packets[0].speed = 40;
    packets[0].satellites = 12;
    packets[0].battery = 50;

    write_to_file("test_write.bin", packets, 1);

    FILE *file = fopen("test_write.bin", "rb");

    Packet data;

    size_t read = fread(&data, sizeof(Packet), 1, file);

    fclose(file);

    TEST_ASSERT_EQUAL_INT(1, read);
    TEST_ASSERT_EQUAL_UINT32(data.time, raw_time);
    TEST_ASSERT_EQUAL_UINT8(data.telemetry, 2);
    TEST_ASSERT_EQUAL_INT(data.speed, 40);
    TEST_ASSERT_EQUAL_INT(12, data.satellites);
    TEST_ASSERT_EQUAL_INT(data.battery, 50);

}

void test_read_from_file(void){
    packets[0].time = raw_time;

    packets[0].telemetry = 2;

    packets[0].speed = 20;
    packets[0].satellites = 5;
    packets[0].battery = 85;

    FILE *file = fopen("test_read.bin", "wb");
    fwrite(packets, sizeof(Packet), 1, file);
    fclose(file);

    Packet *data = read_from_file("test_read.bin");

    TEST_ASSERT_EQUAL_UINT32(data[0].time, raw_time);
    TEST_ASSERT_EQUAL_UINT8(data[0].telemetry, 2);
    TEST_ASSERT_EQUAL_INT(data[0].speed, 20);
    TEST_ASSERT_EQUAL_INT(data[0].satellites, 5);
    TEST_ASSERT_EQUAL_INT(data[0].battery, 85);

    free(data);

}

int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_size_of_packet);
    RUN_TEST(test_correct_string_parsing);
    RUN_TEST(test_check_empty_string);
    RUN_TEST(test_check_low_battery);
    RUN_TEST(test_write_to_file);
    RUN_TEST(test_read_from_file);

    return UNITY_END();
}
