#ifndef EVT_COMMAND_LISTENER_H
#define EVT_COMMAND_LISTENER_H

#include <Eventually.h>
#include <EvtCommandAction.h>
#include <Arduino.h>

#include <stdlib.h>
#include "stdint.h"
#include "string.h"

#define MAX_COMMANDS 10

struct commandAction
{
    EvtCommandAction Action;
    const char *Command;
} typedef CommandAction;

class EvtCommandListener : public EvtListener
{
public:
    EvtCommandListener(Stream *stream);
    EvtCommandListener(Stream *stream, short readDelayMs);
    bool tryReadCommand();
    void when(const char *command, EvtCommandAction action);

    void setupListener();
    bool isEventTriggered();
    bool performTriggerAction(EvtContext *ctx);

private:
    char _commandBuffer[20];
    char _dataBuffer[40];
    short _commandIndex = -1;
    long _dataIndex = -1;
    Stream *_stream;
    short _commandActionIndex = 0;
    unsigned long _readDelayMs = 5;
    CommandAction _commands[MAX_COMMANDS];
};

#endif