#include <unity.h>

#include "MockStreamReader.h"
#include "CommandListener.h"

MockStreamReader streamReader;
Command command;
CommandListener commandListener(&streamReader);

void setUp(void)
{
}

void tearDown(void) {}

void test_invalid_command(void)
{
    streamReader.setCommand("blah");
    bool actual = commandListener.tryReadCommand(&command);
    TEST_ASSERT_EQUAL(Commands::CNONE, command.Value);
}

void test_invalid_command_returns_false(void)
{
    streamReader.setCommand("blah");
    bool actual = commandListener.tryReadCommand(&command);
    TEST_ASSERT_FALSE(actual);
}

void test_non_terminated_command(void)
{
    streamReader.setCommand(">set");
    bool actual = commandListener.tryReadCommand(&command);
    TEST_ASSERT_EQUAL(Commands::CNONE, command.Value);
}

void test_set_command(void)
{
    streamReader.setCommand(">set!");
    bool actual = commandListener.tryReadCommand(&command);
    TEST_ASSERT_EQUAL(Commands::SET, command.Value);
}

void test_valid_command_returns_true(void)
{
    streamReader.setCommand(">set!");
    bool actual = commandListener.tryReadCommand(&command);
    TEST_ASSERT_TRUE(actual);
}

void test_show_command(void)
{
    streamReader.setCommand(">show!");
    bool actual = commandListener.tryReadCommand(&command);
    TEST_ASSERT_EQUAL(Commands::SHOW, command.Value);
}

void test_embedded_command(void)
{
    streamReader.setCommand("random>set!random");
    bool actual = commandListener.tryReadCommand(&command);
    TEST_ASSERT_EQUAL(Commands::SET, command.Value);
}

void test_command_with_missing_data(void)
{
    streamReader.setCommand(">set:!");
    bool actual = commandListener.tryReadCommand(&command);
    TEST_ASSERT_EQUAL(Commands::SET, command.Value);
    TEST_ASSERT_EQUAL(0, command.Data);
}

void test_command_with_invalid_data(void)
{
    streamReader.setCommand(">set:xxx!");
    bool actual = commandListener.tryReadCommand(&command);
    TEST_ASSERT_EQUAL(Commands::SET, command.Value);
    TEST_ASSERT_EQUAL(0, command.Data);
}

void test_command_with_positive_data(void)
{
    streamReader.setCommand(">set:35!");
    bool actual = commandListener.tryReadCommand(&command);
    TEST_ASSERT_EQUAL(Commands::SET, command.Value);
    TEST_ASSERT_EQUAL(35, command.Data);
}

void test_command_with_negative_data(void)
{
    streamReader.setCommand(">set:-35!");
    bool actual = commandListener.tryReadCommand(&command);
    TEST_ASSERT_EQUAL(Commands::SET, command.Value);
    TEST_ASSERT_EQUAL(-35, command.Data);
}

void test_command_with_large_data(void)
{
    streamReader.setCommand(">set:1641092494 !");
    bool actual = commandListener.tryReadCommand(&command);
    TEST_ASSERT_EQUAL(Commands::SET, command.Value);
    TEST_ASSERT_EQUAL(1641092494, command.Data);
}

void test_set_schedule_command(void)
{
    streamReader.setCommand(">set-schedule:1141!");
    bool actual = commandListener.tryReadCommand(&command);
    TEST_ASSERT_EQUAL(Commands::SET_SCHEDULE, command.Value);
    TEST_ASSERT_EQUAL(1141, command.Data);
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
    RUN_TEST(test_set_schedule_command);
    UNITY_END();

    return 0;
}