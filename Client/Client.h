#pragma once
#include <SFML/Graphics.hpp>

#include "ANet/Client.h"
#include "ANet/NetBuffer.h"
#include "Shared/Network/Network.h"
#include "ClientNet.h"

#include "c_player.h"
#include "Renderable.h"

class World
{
public:
    Player localPlayer;
    DynamicArray<Player> players;

    void handlePacket(NetBuffer& buffer)
    {
        int headerCount;
        BUFFER_READ(buffer, headerCount);

        for (int i = 0; i < headerCount; i++)
        {
            HeaderType header;
            BUFFER_READ(buffer, header);

            switch (header)
            {
            case GENERIC:
                break;

            case CONNECT:
                int localPlayerID;
                BUFFER_READ(buffer, localPlayerID);
                localPlayer.id = localPlayerID;

                break;

            case ENTITY:

                EntityTypes entType;
                BUFFER_READ(buffer, entType);
                unsigned char entCount;
                BUFFER_READ(buffer, entCount);

                for (int i = 0; i < entCount; i++)
                {
                    switch (entType)
                    {
                    case PLAYER:
                        Player& p = players.pushBack();
                        p.ReadFromBuffer(buffer);
                    }
                }
                break;
            }
        }
    }

    void update()
    {
        for (Player& player : players)
        {
            player._update();
        }
    }

    void renderFrame(sf::RenderWindow& window)
    {
        for (Player& player : players)
        {
            player._render(window);
        }
    }
};

void getInputs(InputStruct& inputs)
{
    inputs.inputs[0] = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    inputs.inputs[1] = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    inputs.inputs[2] = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    inputs.inputs[3] = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    inputs.inputs[4] = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    inputs.inputs[5] = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
}