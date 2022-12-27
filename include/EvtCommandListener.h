#pragma once

#include <EvtListener.h>
#include <Arduino.h>

#include <stdlib.h>
#include "stdint.h"
#include "string.h"

#define MAX_COMMANDS 10

enum commands
{
    CNONE = 0,
    SET = 1,
    SHOW = 2,
    STATUS = 6,
    SET_SCHEDULE = 7
};
typedef enum commands Commands;

struct command
{
    Commands Value;
    long Data;
};
typedef struct command Command;

struct commandAction
{
    EvtAction action;
    char *command;
} typedef CommandAction;

class EvtCommandListener : public EvtListener
{
public:
    EvtCommandListener(Stream *stream);
    bool tryReadCommand(Command *command);
    void when(char *command, EvtAction action);

    void setupListener();
    bool isEventTriggered();
    bool performTriggerAction(EvtContext *ctx);

private:
    bool tryReadInstruction();
    bool convertToCommand(Command *command);
    char _commandBuffer[20];
    char _dataBuffer[40];
    short _commandIndex = -1;
    long _dataIndex = -1;
    Stream *_stream;
    CommandAction _commands[MAX_COMMANDS];
};