#include "MockStreamReader.h"

void MockStreamReader::setCommand(const char *command)
{
    _index = 0;
    _command = command;
}

bool MockStreamReader::available()
{
    return *(_command + _index) != '\0';
}

int MockStreamReader::read()
{
    _index++;
    return *(_command + _index - 1);
}