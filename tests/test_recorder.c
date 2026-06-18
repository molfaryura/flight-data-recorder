#include <stdlib.h>
#include "support/unity.h"
#include "../recorder.h"


void setUp(void) {

}


void tearDown(void) {

}

void test_size_of_packet(void){
    Packet packet;

    TEST_ASSERT_EQUAL_INT(43, sizeof(packet));

}

void test_correct_string_parsing(void){
    Packet *packets = calloc(1, sizeof(Packet));

    char time_buffer[20] = "2026-06-18 16:02:25";

    char buffer[] = "$DATA,30,12,90#";

    parse(buffer, 0, &packets, time_buffer);

    TEST_ASSERT_EQUAL_STRING("DATA", packets[0].telemetry);
    TEST_ASSERT_EQUAL_INT(30, packets[0].speed);
    TEST_ASSERT_EQUAL_INT(12, packets[0].satellites);
    TEST_ASSERT_EQUAL_INT(90, packets[0].battery);

}


int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_size_of_packet);
    RUN_TEST(test_correct_string_parsing);

    return UNITY_END();
}
