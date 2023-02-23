#include "EvtCommandListener.h"

EvtCommandListener::EvtCommandListener(Stream *stream, short readDelayMs)
{
    _stream = stream;
    _readDelayMs = readDelayMs;
}

void EvtCommandListener::when(const char *command, EvtCommandAction action)
{
    CommandAction commandAction;
    commandAction.Command = command;
    commandAction.Action = action;
    _commands[_commandActionIndex] = commandAction;
    _commandActionIndex++;
}

bool EvtCommandListener::performTriggerAction(EvtContext *ctx)
{
    for (short i = 0; i < _commandActionIndex; i++)
    {
        CommandAction action = _commands[i];
        if (strcmp(_commandBuffer, action.Command) == 0)
        {
            long data = -1;
            if (_dataIndex > 1)
            {
                data = atol(_dataBuffer);
            }
            return action.Action(this, ctx, data);
        }
    }

    return false;
}

bool EvtCommandListener::isEventTriggered()
{
#ifdef EVT_SUPPORTS_DISABLE
    if (!EvtListener::isEventTriggered())
    {
        return false;
    }
#endif

    return tryReadCommand();
}

bool EvtCommandListener::tryReadCommand()
{
    _commandIndex = -1;
    _dataIndex = -1;
    while (_stream->available())
    {
        delay(_readDelayMs);
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
                appendToCommandIfPossible(ch);
            }
            else
            {
                appendToDataIfPossible(ch);
            }
            break;
        }
    }
    return false;
}

void EvtCommandListener::appendToCommandIfPossible(char ch)
{
    if (_commandIndex >= EVENTUALLY_COMMAND_BUFFER_LENGTH - 1)
    {
        _commandBuffer[EVENTUALLY_COMMAND_BUFFER_LENGTH - 1] = '\0';
        _commandIndex++;
    }
    else
    {
        _commandBuffer[_commandIndex] = ch;
        _commandIndex++;
    }
}

void EvtCommandListener::appendToDataIfPossible(char ch)
{
    if (_dataIndex >= EVENTUALLY_DATA_BUFFER_LENGTH - 1)
    {
        _dataBuffer[EVENTUALLY_DATA_BUFFER_LENGTH - 1] = '\0';
        _dataIndex++;
    }
    else
    {
        _dataBuffer[_dataIndex] = ch;
        _dataIndex++;
    }
}

void EvtCommandListener::setupListener()
{
}