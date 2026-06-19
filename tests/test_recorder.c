#include <stdlib.h>
#include <string.h>
#include "support/unity.h"
#include "../recorder.h"

static Packet *packets;
static char time_buffer[20] = "2026-06-18 16:02:25";

void setUp(void) {
    packets = calloc(1, sizeof(Packet));
}


void tearDown(void) {
    free(packets);
    packets = NULL;
}

void test_size_of_packet(void){
    Packet packet;

    TEST_ASSERT_EQUAL_INT(43, sizeof(packet));

}

void test_correct_string_parsing(void){

    char buffer[] = "$DATA,30,12,90#";

    parse(buffer, 0, &packets, time_buffer);

    TEST_ASSERT_EQUAL_STRING("DATA", packets[0].telemetry);
    TEST_ASSERT_EQUAL_INT(30, packets[0].speed);
    TEST_ASSERT_EQUAL_INT(12, packets[0].satellites);
    TEST_ASSERT_EQUAL_INT(90, packets[0].battery);

}

void test_check_empty_string(void){
    char empty_string[] = "";

    int result = parse(empty_string, 0, &packets, time_buffer);

    TEST_ASSERT_TRUE(result);
}

void test_check_low_battery(void){
    char str[] = "$DATA,40,10,14#";

    int result = parse(str, 0, &packets, time_buffer);

    TEST_ASSERT_TRUE(result);
}

void test_write_to_file(void){
    strcpy(packets[0].time, time_buffer);

    char str[] = "DATA";
    strcpy(packets[0].telemetry, str);

    packets[0].speed = 40;
    packets[0].satellites = 12;
    packets[0].battery = 50;

    write_to_file(packets, 1);

    FILE *file = fopen("blackbox.bin", "rb");

    Packet data;

    size_t read = fread(&data, sizeof(Packet), 1, file);

    fclose(file);

    TEST_ASSERT_EQUAL_INT(1, read);
    TEST_ASSERT_EQUAL_STRING(data.time, time_buffer);
    TEST_ASSERT_EQUAL_STRING(data.telemetry, "DATA");
    TEST_ASSERT_EQUAL_INT(data.speed, 40);
    TEST_ASSERT_EQUAL_INT(data.satellites, 12);
    TEST_ASSERT_EQUAL_INT(data.battery, 50);

}

int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_size_of_packet);
    RUN_TEST(test_correct_string_parsing);
    RUN_TEST(test_check_empty_string);
    RUN_TEST(test_check_low_battery);
    RUN_TEST(test_write_to_file);

    return UNITY_END();
}
