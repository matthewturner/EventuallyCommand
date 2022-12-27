#pragma once

#include "IStreamReader.h"

class MockStreamReader : public IStreamReader
{
public:
    void setCommand(const char *command);
    bool available() override;
    int read() override;

private:
    const char *_command;
    short _index;
};