#include <stdlib.h>
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

int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_size_of_packet);
    RUN_TEST(test_correct_string_parsing);
    RUN_TEST(test_check_empty_string);

    return UNITY_END();
}
