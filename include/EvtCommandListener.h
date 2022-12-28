#pragma once

#include <EvtListener.h>
#include <Arduino.h>

#include <stdlib.h>
#include "stdint.h"
#include "string.h"

#define MAX_COMMANDS 10

struct commandAction
{
    EvtAction Action;
    const char *Command;
} typedef CommandAction;

class EvtCommandListener : public EvtListener
{
public:
    EvtCommandListener(Stream *stream);
    bool tryReadCommand();
    void when(const char *command, EvtAction action);

    void setupListener();
    bool isEventTriggered();
    bool performTriggerAction(EvtContext *ctx);

    /// @brief The data from the last command
    /// @return The value
    long data();

private:
    char _commandBuffer[20];
    char _dataBuffer[40];
    short _commandIndex = -1;
    long _dataIndex = -1;
    Stream *_stream;
    short _commandActionIndex = 0;
    CommandAction _commands[MAX_COMMANDS];
};