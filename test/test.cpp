#include <unity.h>
#include <bla.h>


Bla bla;
// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void example_test(void) {
    int actual = bla.yolo();
    TEST_ASSERT_EQUAL(1, actual);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(example_test);
    UNITY_END();

    return 0;
}