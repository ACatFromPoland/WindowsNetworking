#pragma once
#include "sThreading.h"
#include "sWorld.h"
#include <iostream>

void handlePacket(packetData& packet, World& world);
void handleHeader(packetData& packet, World& world);

static int lastPacketID = 0;