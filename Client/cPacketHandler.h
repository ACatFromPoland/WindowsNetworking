#pragma once
#include "cThreading.h"
#include "cWorld.h"
#include "Shared/NetPackets.h"
#include <iostream>

void handlePacket(packetData& packet, World& world);

void handleHeader(packetData& packet, World& world);

void handleEntities(packetData& packet, World& world);