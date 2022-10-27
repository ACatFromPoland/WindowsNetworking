#pragma once
#include <vector>
#include <cmath>
#include "Shared/Vector2.h"
#include "sWorld.h"


class World;

class CircleCollider;
class PointCollider;
class BoxCollider;

enum ColliderType
{
    COLLIDER_BOX,
    COLLIDER_CIRCLE,
    COLLIDER_POINT
};

class ColliderBase
{
public:
    ColliderType type;
                                            // TODO: REPLACE WITH ENTITY
    //virtual void getCollisions(World& world, std::vector<CBase*>& collisions);
    virtual bool doesCollide(ColliderBase* other);

    virtual bool collides(CircleCollider* circle1, CircleCollider* circle2);

    virtual bool collides(BoxCollider* box1, BoxCollider* box2);
    virtual bool collides(CircleCollider* circle, BoxCollider* box);

    //virtual bool collides(CircleCollider* circle, PointCollider* point);
    //virtual bool collides(BoxCollider* box, PointCollider* point);
};

class CircleCollider : public ColliderBase
{
public:
    Vector2 center;
    float radius;

    CircleCollider();
    CircleCollider(Vector2 center, float r);
};

class BoxCollider : public ColliderBase
{
public:
    Vector2 position;
    Vector2 size;
    float rotation;

    BoxCollider();
    BoxCollider(Vector2 position, Vector2 size);
};

//TODO:
/*
OBB Colliders

*/