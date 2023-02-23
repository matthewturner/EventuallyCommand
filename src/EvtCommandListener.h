#ifndef EVT_COMMAND_LISTENER_H
#define EVT_COMMAND_LISTENER_H

#include <Eventually.h>
#include <EvtCommandAction.h>
#include <Arduino.h>

#include <stdlib.h>
#include "stdint.h"
#include "string.h"

#ifndef EVENTUALLY_MAX_COMMANDS
#define EVENTUALLY_MAX_COMMANDS 10
#endif
#ifndef EVENTUALLY_COMMAND_BUFFER_LENGTH
#define EVENTUALLY_COMMAND_BUFFER_LENGTH 10
#endif
#ifndef EVENTUALLY_DATA_BUFFER_LENGTH
#define EVENTUALLY_DATA_BUFFER_LENGTH 8
#endif

struct commandAction
{
    EvtCommandAction Action;
    const char *Command;
} typedef CommandAction;

class EvtCommandListener : public EvtListener
{
public:
    EvtCommandListener(Stream *stream, short readDelayMs = 5);
    bool tryReadCommand();
    void when(const char *command, EvtCommandAction action);

    void setupListener();
    bool isEventTriggered();
    bool performTriggerAction(EvtContext *ctx);

private:
    char _commandBuffer[EVENTUALLY_COMMAND_BUFFER_LENGTH];
    char _dataBuffer[EVENTUALLY_DATA_BUFFER_LENGTH];
    short _commandIndex = -1;
    long _dataIndex = -1;
    Stream *_stream;
    short _commandActionIndex = 0;
    unsigned long _readDelayMs;
    CommandAction _commands[EVENTUALLY_MAX_COMMANDS];

    void appendToCommandIfPossible(char ch);
    void appendToDataIfPossible(char ch);
};

#endif