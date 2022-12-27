#include "CommandListener.h"

CommandListener::CommandListener(IStreamReader *streamReader)
{
    _streamReader = streamReader;
}

bool CommandListener::tryReadCommand(Command *command)
{
    command->Value = CNONE;
    command->Data = 0;

    if (tryReadInstruction())
    {
        return convertToCommand(command);
    }
    return false;
}

bool CommandListener::tryReadInstruction()
{
    _commandIndex = -1;
    _dataIndex = -1;
    while (_streamReader->available())
    {
        int ch = _streamReader->read();
        switch (ch)
        {
        case '>':
            _commandIndex = 0;
            break;
        case ':':
            _dataIndex = 0;
            break;
        case '!':
            _commandBuffer[_commandIndex] = '\0';
            _commandIndex++;
            if (_dataIndex >= 0)
            {
                _dataBuffer[_dataIndex] = '\0';
                _dataIndex++;
            }
            return true;
        default:
            if (_dataIndex == -1)
            {
                _commandBuffer[_commandIndex] = ch;
                _commandIndex++;
            }
            else
            {
                _dataBuffer[_dataIndex] = ch;
                _dataIndex++;
            }
            break;
        }
    }
    return false;
}

bool CommandListener::convertToCommand(Command *command)
{
    command->Value = CNONE;
    command->Data = 0;

    if (_commandIndex == 0)
    {
        return false;
    }
    if (strcmp(_commandBuffer, "set") == 0)
    {
        command->Value = SET;
        command->Data = atol(_dataBuffer);
        return true;
    }
    if (strcmp(_commandBuffer, "set-schedule") == 0)
    {
        command->Value = SET_SCHEDULE;
        command->Data = atol(_dataBuffer);
        return true;
    }
    if (strcmp(_commandBuffer, "show") == 0)
    {
        command->Value = SHOW;
        return true;
    }
    if (strcmp(_commandBuffer, "status") == 0)
    {
        command->Value = STATUS;
        return true;
    }
    return false;
}
