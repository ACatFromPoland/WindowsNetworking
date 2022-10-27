#include "Collision.h"

bool ColliderBase::doesCollide(ColliderBase* other)
{
    if (this->type == COLLIDER_CIRCLE && other->type == COLLIDER_CIRCLE)
        return collides((CircleCollider*)this, (CircleCollider*)other);

    if (this->type == COLLIDER_BOX && other->type == COLLIDER_BOX)
        return collides((BoxCollider*)this, (BoxCollider*)other);

    if (this->type == COLLIDER_CIRCLE && other->type == COLLIDER_BOX)
        return collides((CircleCollider*)this, (BoxCollider*)other);

    if (this->type == COLLIDER_BOX && other->type == COLLIDER_CIRCLE)
        return collides((CircleCollider*)other, (BoxCollider*)this);

    /* 
    if (this->type == COLLIDER_POINT || other->type == COLLIDER_POINT)
    {
        if (this->type == COLLIDER_CIRCLE)
            return collides((CircleCollider*)this, (PointCollider*)other);
        else
            return collides((CircleCollider*)other, (PointCollider*)this);

        if (this->type == COLLIDER_BOX)
            return false; //collides((BoxCollider*)this, (PointCollider*)other);
        else
            return false; //collides((BoxCollider*)other, (PointCollider*)this);
    }
    */
}

CircleCollider::CircleCollider()
{
    type = ColliderType::COLLIDER_CIRCLE;
    center = Vector2(0.0f, 0.0f);
    radius = 1.f;
}

CircleCollider::CircleCollider(Vector2 center, float r)
{
    type = ColliderType::COLLIDER_CIRCLE;
    this->center = center;
    this->radius = r;
}

BoxCollider::BoxCollider()
{
    type = ColliderType::COLLIDER_BOX;
    position = Vector2(0.0f, 0.0f);
    size = Vector2(0.0f, 0.0f);
    rotation = 0.0f;
}

BoxCollider::BoxCollider(Vector2 position, Vector2 size)
{
    type = ColliderType::COLLIDER_BOX;
    this->position = position;
    this->size = size;
    rotation = 0.0f;
}


// TODO: REPLACE WITH ENTITY AND WORLD
/*
void ColliderBase::getCollisions(World& world, std::vector<CBase*>& collisions)
{
    collisions.clear();

    for (CBase* entity : world.entities)
    {
        ColliderBase* collider = entity->getCollider();
        if (collider == this)
            continue;

        if (doesCollide(collider))
        {
            collisions.push_back(entity);
        }
    }
}
*/

bool ColliderBase::collides(CircleCollider* circle1, CircleCollider* circle2)
{
    float dist = std::sqrt(std::pow(circle2->center.x - circle1->center.x, 2.0f) + std::pow(circle2->center.y - circle1->center.y, 2.0f));
    return (std::abs(dist) < circle1->radius + circle2->radius);
}

bool ColliderBase::collides(BoxCollider* box1, BoxCollider* box2)
{
    return (
        box1->position.x < box2->position.x + box2->size.x &&
        box1->position.x + box2->size.x > box2->position.x &&
        box1->position.y < box2->position.y + box2->size.y &&
        box2->size.y + box1->position.y > box2->position.y
        );
}

bool ColliderBase::collides(CircleCollider* circle, BoxCollider* box)
{
    // temporary variables to set edges for testing
    float testX = circle->center.x;
    float testY = circle->center.y;

    // which edge is closest?
    if (circle->center.x < box->position.x)                            testX = box->position.x;                        // test left edge
    else if (circle->center.x > box->position.x + (box->size.x * 2.0f))       testX = box->position.x + (box->size.x * 2.0f);   // right edge
    if (circle->center.y < box->position.y)                            testY = box->position.y;                        // top edge
    else if (circle->center.y > box->position.y + (box->size.y * 2.0f))       testY = box->position.y + (box->size.y * 2.0f);   // bottom edge

    // get distance from closest edges
    float distX = circle->center.x - testX;
    float distY = circle->center.y - testY;
    float distance = sqrt((distX * distX) + (distY * distY));

    // if the distance is less than the radius, collision!
    if (distance <= circle->radius) {
        return true;
    }
    return false;
}

/* 
bool ColliderBase::collides(CircleCollider* circle, PointCollider* point)
{
    float dist = std::sqrt(std::pow(circle->center.x - point->x, 2.0f) + std::pow(circle->center.y - point->y, 2.0f));
    return (std::abs(dist) < circle->radius);
}

bool ColliderBase::collides(BoxCollider* box, PointCollider* point)
{
    return (
        point->x > box->position.x &&
        point->x < box->position.x + box->size.x &&
        point->y > box->position.y &&
        point->y < box->position.y + box->size.y
        );
}
*/