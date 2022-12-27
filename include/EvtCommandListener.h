#pragma once

#include "IStreamReader.h"

#include <stdlib.h>
#include "stdint.h"
#include "string.h"

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

class EvtCommandListener
{
public:
    EvtCommandListener(IStreamReader *streamReader);
    bool tryReadCommand(Command *command);

private:
    bool tryReadInstruction();
    bool convertToCommand(Command *command);
    char _commandBuffer[20];
    char _dataBuffer[40];
    short _commandIndex = -1;
    long _dataIndex = -1;
    IStreamReader *_streamReader;
};