#ifndef SHAREDSERIAL_H
#define SHAREDSERIAL_H

#include "serialcontrol.h"

// 单例：全局共享一个串口实例
class SharedSerial
{
public:
    static SerialControl* instance()
    {
        static SerialControl serial("/dev/ttyAMA0", 115200);
        return &serial;
    }

private:
    SharedSerial() = delete;
};

#endif