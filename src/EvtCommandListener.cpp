#include "EvtCommandListener.h"

EvtCommandListener::EvtCommandListener(Stream *stream)
{
    _stream = stream;
}

void EvtCommandListener::when(char *command, EvtAction action)
{
    CommandAction commandAction;
    commandAction.command = command;
    commandAction.action = action;
    _commands[0] = commandAction;
}

bool EvtCommandListener::performTriggerAction(EvtContext *ctx)
{
    if (tryReadInstruction())
    {
        return true;
    }

    return false;
}

bool EvtCommandListener::isEventTriggered()
{
    if (!EvtListener::isEventTriggered())
    {
        return false;
    }

    return tryReadInstruction();
}

bool EvtCommandListener::tryReadCommand(Command *command)
{
    command->Value = CNONE;
    command->Data = 0;

    if (tryReadInstruction())
    {
        return convertToCommand(command);
    }
    return false;
}

bool EvtCommandListener::tryReadInstruction()
{
    _commandIndex = -1;
    _dataIndex = -1;
    while (_stream->available())
    {
        int ch = _stream->read();
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

bool EvtCommandListener::convertToCommand(Command *command)
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

void EvtCommandListener::setupListener()
{
}