#include <unity.h>
#include <pushButton.h>


void initial_state_is_false(void) {
    // arrange
    PushButton pushButton;

    // assert
    bool actual = pushButton.isPushed();
    TEST_ASSERT_EQUAL(false, actual);
}

void after_stateupdate_0_state_is_true(void) {
    // arrange
    PushButton pushButton;

    // actual
    pushButton.updateState(0);

    // assert
    bool actual = pushButton.isPushed();
    TEST_ASSERT_EQUAL(true, actual);
}

void after_stateupdate_1_state_is_false(void) {
    // arrange
    PushButton pushButton;

    // actual
    pushButton.updateState(1);

    // assert
    bool actual = pushButton.isPushed();
    TEST_ASSERT_EQUAL(false, actual);
}

void after_stateupdates_0_and_1_state_is_true(void) {
    // arrange
    PushButton pushButton;

    // actual
    pushButton.updateState(0);
    pushButton.updateState(1);

    // assert
    bool actual = pushButton.isPushed();
    TEST_ASSERT_EQUAL(true, actual);
}

void after_call_isPushed_state_is_false(void) {
    // arrange
    PushButton pushButton;
    pushButton.updateState(0);
    
    // actual
    pushButton.isPushed();

    // assert
    bool actual = pushButton.isPushed();
    TEST_ASSERT_EQUAL(false, actual);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(initial_state_is_false);
    RUN_TEST(after_stateupdate_0_state_is_true);
    RUN_TEST(after_stateupdate_1_state_is_false);
    RUN_TEST(after_stateupdates_0_and_1_state_is_true);
    RUN_TEST(after_call_isPushed_state_is_false);
        
    UNITY_END();

    return 0;
}