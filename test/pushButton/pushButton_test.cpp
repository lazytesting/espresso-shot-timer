#include <unity.h>
#include <pushButton.h>


void initial_state_is_false(void) {
    // arrange
    PushButton pushButton(0);

    // assert
    bool actual = pushButton.isPushed();
    TEST_ASSERT_EQUAL(false, actual);
}


void after_stateupdate_1_state_is_false(void) {
    // arrange
    PushButton pushButton(0);

    // actual
    pushButton.updateState(1);

    // assert
    bool actual = pushButton.isPushed();
    TEST_ASSERT_EQUAL(false, actual);
}

void after_stateupdates_1_and_0_state_is_true(void) {
    // arrange
    PushButton pushButton(0);
    pushButton.isPushed();

    // actual
    pushButton.updateState(1);
    pushButton.updateState(0);

    // assert
    bool actual = pushButton.isPushed();
    TEST_ASSERT_EQUAL(true, actual);
}

void after_call_isPushed_state_is_false(void) {
    // arrange
    PushButton pushButton(0);
    pushButton.updateState(0);
    
    // actual
    pushButton.isPushed();

    // assert
    bool actual = pushButton.isPushed();
    TEST_ASSERT_EQUAL(false, actual);
}

void when_state_not_changed_state_is_false(void) {
    // arrange
    PushButton pushButton(0);
    pushButton.updateState(0);
    
    
    // actual
    pushButton.updateState(0);
    pushButton.isPushed();

    // assert
    bool actual = pushButton.isPushed();
    TEST_ASSERT_EQUAL(false, actual);
}



void with_multiple_instance_no_inteference(void) {
    // arrange
    PushButton pushButton1(0);
    PushButton pushButton2(0);
    pushButton1.updateState(1);
    pushButton1.isPushed();
    pushButton2.updateState(1);
    pushButton2.isPushed();
    
    // actual
    pushButton1.updateState(0);
    pushButton2.updateState(0);

    // assert
    bool actual1 = pushButton1.isPushed();
    TEST_ASSERT_EQUAL(true, actual1);

    bool actual2 = pushButton2.isPushed();
    TEST_ASSERT_EQUAL(true, actual2);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(initial_state_is_false);
    RUN_TEST(after_stateupdate_1_state_is_false);
    RUN_TEST(after_stateupdates_1_and_0_state_is_true);
    RUN_TEST(after_call_isPushed_state_is_false);
    RUN_TEST(when_state_not_changed_state_is_false);
    RUN_TEST(with_multiple_instance_no_inteference);
    UNITY_END();

    return 0;
}