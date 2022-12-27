#pragma once

class IStreamReader
{
public:
    virtual bool available() = 0;
    virtual int read() = 0;
};