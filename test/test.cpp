#include <unity.h>
#include "../src/bla/bla.cpp"


// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_led_builtin_pin_number(void) {
    TEST_ASSERT_EQUAL(12, 12);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_led_builtin_pin_number);
    UNITY_END();

    return 0;
}