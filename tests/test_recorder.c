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


int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_size_of_packet);

    return UNITY_END();
}
