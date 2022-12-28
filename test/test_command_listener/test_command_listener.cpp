#include <ArduinoFake.h>

#include "EvtCommandListener.h"
#include "EvtContext.h"
#include <unity.h>

using namespace fakeit;

Stream *stream;
EvtContext ctx;
EvtCommandListener *target;
bool _called = false;
long _data = -1;

void setUp(void)
{
    ArduinoFakeReset();
    _called = false;
}

void tearDown(void)
{
    delete stream;
    delete target;
}

bool mockMethod(EvtListener *listener, EvtContext *ctx, long data)
{
    _called = true;
    _data = data;
    return true;
}

void test_not_triggered_by_invalid_command(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('b', 'l', 'a', 'h');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->isEventTriggered();
    TEST_ASSERT_FALSE(actual);
}

void test_not_triggered_by_non_terminated_command(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->isEventTriggered();
    TEST_ASSERT_FALSE(actual);
}

void test_not_triggered_by_non_terminated_command_does_not_call_action(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    target->isEventTriggered();
    bool actual = target->performTriggerAction(&ctx);
    TEST_ASSERT_FALSE(actual);
    TEST_ASSERT_FALSE(_called);
}

void test_triggered_by_valid_command(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->isEventTriggered();
    TEST_ASSERT_TRUE(actual);
}

void test_triggered_by_valid_command_calls_action(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    target->when("set", (EvtCommandAction)mockMethod);

    target->isEventTriggered();
    bool actual = target->performTriggerAction(&ctx);
    TEST_ASSERT_TRUE(actual);
    TEST_ASSERT_TRUE(_called);
}

void test_triggered_by_embedded_command(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('r', 'a', 'n', '>', 's', 'e', 't', '!', 'd', 'o', 'm');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->isEventTriggered();
    TEST_ASSERT_TRUE(actual);
}

void test_triggered_by_command_with_missing_data(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', ':', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->isEventTriggered();
    TEST_ASSERT_TRUE(actual);
}

void test_triggered_by_command_with_invalid_data(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', ':', 'x', 'x', 'x', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->isEventTriggered();
    TEST_ASSERT_TRUE(actual);
}

void test_triggered_by_command_with_invalid_data_calls_action(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', ':', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    target->when("set", (EvtCommandAction)mockMethod);

    target->isEventTriggered();
    bool actual = target->performTriggerAction(&ctx);
    TEST_ASSERT_TRUE(actual);
    TEST_ASSERT_TRUE(_called);
    TEST_ASSERT_EQUAL(-1, _data);
}

void test_triggered_by_command_with_positive_data(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', ':', '3', '5', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->isEventTriggered();
    TEST_ASSERT_TRUE(actual);
}

void test_triggered_by_command_with_positive_data_calls_action(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', ':', '3', '5', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    target->when("set", (EvtCommandAction)mockMethod);

    target->isEventTriggered();
    bool actual = target->performTriggerAction(&ctx);
    TEST_ASSERT_TRUE(actual);
    TEST_ASSERT_TRUE(_called);
    TEST_ASSERT_EQUAL(35, _data);
}

void test_triggered_by_command_with_single_digit_data_calls_action(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', ':', '3', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    target->when("set", (EvtCommandAction)mockMethod);

    target->isEventTriggered();
    bool actual = target->performTriggerAction(&ctx);
    TEST_ASSERT_TRUE(actual);
    TEST_ASSERT_TRUE(_called);
    TEST_ASSERT_EQUAL(3, _data);
}

void test_triggered_by_command_with_negative_data(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', ':', '-', '3', '5', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->isEventTriggered();
    TEST_ASSERT_TRUE(actual);
}

void test_triggered_by_command_with_negative_data_calls_action(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', ':', '-', '3', '5', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    target->when("set", (EvtCommandAction)mockMethod);

    target->isEventTriggered();
    bool actual = target->performTriggerAction(&ctx);
    TEST_ASSERT_TRUE(actual);
    TEST_ASSERT_TRUE(_called);
    TEST_ASSERT_EQUAL(-35, _data);
}

void test_triggered_by_command_with_large_data(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', ':', '1', '6', '4', '1', '0', '9', '2', '4', '9', '4', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->isEventTriggered();
    TEST_ASSERT_TRUE(actual);
}

void test_triggered_by_command_with_large_data_calls_action(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', ':', '1', '6', '4', '1', '0', '9', '2', '4', '9', '4', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    target->when("set", (EvtCommandAction)mockMethod);

    target->isEventTriggered();
    bool actual = target->performTriggerAction(&ctx);
    TEST_ASSERT_TRUE(actual);
    TEST_ASSERT_TRUE(_called);
    TEST_ASSERT_EQUAL(1641092494, _data);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_not_triggered_by_invalid_command);
    RUN_TEST(test_not_triggered_by_non_terminated_command);
    RUN_TEST(test_not_triggered_by_non_terminated_command_does_not_call_action);
    RUN_TEST(test_triggered_by_valid_command);
    RUN_TEST(test_triggered_by_valid_command_calls_action);
    RUN_TEST(test_triggered_by_embedded_command);
    RUN_TEST(test_triggered_by_command_with_missing_data);
    RUN_TEST(test_triggered_by_command_with_invalid_data);
    RUN_TEST(test_triggered_by_command_with_invalid_data_calls_action);
    RUN_TEST(test_triggered_by_command_with_positive_data);
    RUN_TEST(test_triggered_by_command_with_positive_data_calls_action);
    RUN_TEST(test_triggered_by_command_with_single_digit_data_calls_action);
    RUN_TEST(test_triggered_by_command_with_negative_data);
    RUN_TEST(test_triggered_by_command_with_negative_data_calls_action);
    RUN_TEST(test_triggered_by_command_with_large_data);
    RUN_TEST(test_triggered_by_command_with_large_data_calls_action);
    UNITY_END();

    return 0;
}