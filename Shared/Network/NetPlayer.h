#pragma once
#include "NetEntity.h"

class NetPlayer : public NetEntity
{
public:
    const static int playerInputsSize = 6;

    NetInt(localId);
    NetByte(classId);
    NetByte(weaponId);
    NetByte(health);

    virtual void _update() = 0;
};

struct InputStruct
{
    union
    {
        bool inputs[6];
        struct
        {
            bool w, a, s, d;
            bool mouseL;
            bool mouseR;
        };
    };
};