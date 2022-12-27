#include <ArduinoFake.h>

#include "EvtCommandListener.h"
#include <unity.h>

using namespace fakeit;

Stream *stream;
Command command;
EvtCommandListener *target;

void setUp(void)
{
    ArduinoFakeReset();
}

void tearDown(void)
{
    delete stream;
    delete target;
}

void test_invalid_command(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('b', 'l', 'a', 'h');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->tryReadCommand(&command);
    TEST_ASSERT_EQUAL(Commands::CNONE, command.Value);
}

void test_invalid_command_returns_false(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('b', 'l', 'a', 'h');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->tryReadCommand(&command);
    TEST_ASSERT_FALSE(actual);
}

void test_non_terminated_command(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->tryReadCommand(&command);
    TEST_ASSERT_EQUAL(Commands::CNONE, command.Value);
}

void test_set_command(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->tryReadCommand(&command);
    TEST_ASSERT_EQUAL(Commands::SET, command.Value);
}

void test_valid_command_returns_true(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->tryReadCommand(&command);
    TEST_ASSERT_TRUE(actual);
}

void test_embedded_command(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('r', 'a', 'n', '>', 's', 'e', 't', '!', 'd', 'o', 'm');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->tryReadCommand(&command);
    TEST_ASSERT_TRUE(actual);
    TEST_ASSERT_EQUAL(Commands::SET, command.Value);
}

void test_command_with_missing_data(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', ':', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->tryReadCommand(&command);
    TEST_ASSERT_TRUE(actual);
    TEST_ASSERT_EQUAL(Commands::SET, command.Value);
    TEST_ASSERT_EQUAL(0, command.Data);
}

void test_command_with_invalid_data(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', ':', 'x', 'x', 'x', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->tryReadCommand(&command);
    TEST_ASSERT_TRUE(actual);
    TEST_ASSERT_EQUAL(Commands::SET, command.Value);
    TEST_ASSERT_EQUAL(0, command.Data);
}

void test_command_with_positive_data(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', ':', '3', '5', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->tryReadCommand(&command);
    TEST_ASSERT_TRUE(actual);
    TEST_ASSERT_EQUAL(Commands::SET, command.Value);
    TEST_ASSERT_EQUAL(35, command.Data);
}

void test_command_with_negative_data(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', ':', '-', '3', '5', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->tryReadCommand(&command);
    TEST_ASSERT_TRUE(actual);
    TEST_ASSERT_EQUAL(Commands::SET, command.Value);
    TEST_ASSERT_EQUAL(-35, command.Data);
}

void test_command_with_large_data(void)
{
    When(Method(ArduinoFake(Stream), available)).Return(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0);
    When(Method(ArduinoFake(Stream), read)).Return('>', 's', 'e', 't', ':', '1', '6', '4', '1', '0', '9', '2', '4', '9', '4', '!');
    stream = ArduinoFakeMock(Stream);
    target = new EvtCommandListener(stream);

    bool actual = target->tryReadCommand(&command);
    TEST_ASSERT_TRUE(actual);
    TEST_ASSERT_EQUAL(Commands::SET, command.Value);
    TEST_ASSERT_EQUAL(1641092494, command.Data);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_invalid_command);
    RUN_TEST(test_invalid_command_returns_false);
    RUN_TEST(test_non_terminated_command);
    RUN_TEST(test_set_command);
    RUN_TEST(test_valid_command_returns_true);
    RUN_TEST(test_embedded_command);
    RUN_TEST(test_command_with_missing_data);
    RUN_TEST(test_command_with_invalid_data);
    RUN_TEST(test_command_with_positive_data);
    RUN_TEST(test_command_with_negative_data);
    RUN_TEST(test_command_with_large_data);
    UNITY_END();

    return 0;
}