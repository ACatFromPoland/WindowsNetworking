#include "SFML/Graphics.hpp"

#include "../Shared/DynamicArray.h"
#include "../Shared/Vector2.h"

enum class ShapeType
{
    EMPTY,
    RECTNAGLE,
    CIRCLE
};

enum class BrushType
{
    WALL = 0,
    INVIS,
    PLAYERSPAWN,
    BOTSPAWN,
    HEALTH,
    AMMO,
    LAST
};

sf::Color brushColors[(int)BrushType::LAST] =
{
    sf::Color::Black,
    sf::Color::White,
    sf::Color::Red,
    sf::Color::Cyan,
    sf::Color::Magenta,
    sf::Color::Yellow
};

class Drawable
{
public:
    Vector2 position;
    ShapeType type;
    Drawable()
    {
        type = ShapeType::EMPTY;
        position = Vector2(FLT_MAX, FLT_MAX);
    }
};

class Circle : public Drawable
{
public:
    float radius;
    Circle()
    {
        type = ShapeType::CIRCLE;
        radius = FLT_MIN;
    }
};

class Rectangle : public Drawable
{
public:
    BrushType rType;
    Vector2 size;
    Rectangle()
    {
        rType = BrushType::WALL;
        type = ShapeType::RECTNAGLE;
        size = Vector2(FLT_MIN, FLT_MIN);
    }
};

sf::CircleShape drawCirle(Circle& cir)
{
    sf::CircleShape circle;
    circle.setPosition(*(sf::Vector2f*)&cir.position);
    circle.setRadius(cir.radius);
    return circle;
}

sf::RectangleShape drawRectangle(Rectangle& rect)
{
    sf::RectangleShape rectangle;
    rectangle.setPosition(*(sf::Vector2f*)&rect.position);
    rectangle.setSize(*(sf::Vector2f*)&rect.size);
    rectangle.setFillColor(brushColors[(int)rect.rType]);
    return rectangle;
}

struct Brush
{
    bool pressed = false;
    bool placeable = false; 
    bool dontWork = false;
    ShapeType type = ShapeType::EMPTY;
    BrushType rType = BrushType::WALL;

    Drawable* temporary = nullptr;
};

struct Menu
{
    Brush* brush;

    void getColor(Vector2 mousePosition)
    {
        bool b = false;

        for (int i = 0; i < (int)BrushType::LAST; i++)
        {
            Vector2 boxMin = Vector2(0.0f, (i * 50.0f));
            Vector2 boxMax = Vector2(45.0f, (i * 50.0f) + 45.0f);
            
            bool inBounds = (mousePosition.x > boxMin.x) &&
                (mousePosition.y > boxMin.y) &&
                (mousePosition.x < boxMax.x) &&
                (mousePosition.y < boxMax.y);

            if (inBounds) {
                b = true;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && inBounds)
            {
                brush->rType = (BrushType)i;
            }
        }

        brush->dontWork = b;
    }

    void draw(sf::RenderWindow& window)
    {
        for (int i = 0; i < (int)BrushType::LAST; i++)
        {
            sf::RectangleShape box;
            box.setFillColor(brushColors[i]);
            box.setPosition(0.0f, (i * 50.0f));
            box.setSize(sf::Vector2f(45.0f, 45.0f));
            window.draw(box);
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(900, 900), "Map Maker");

    Brush brush;
    brush.type = ShapeType::RECTNAGLE;

    Menu menu {&brush};

    DynamicArray<Drawable*> drawables;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color(146, 146, 144));
        
        menu.draw(window);

        // Get Input
        sf::Vector2f mP = sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
        Vector2 mousePosition = *(Vector2*)&mP;

        menu.getColor(mousePosition);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !brush.dontWork)
        {
            if (!brush.pressed)
            {
                brush.pressed = true;

                switch (brush.type)
                {
                case ShapeType::CIRCLE:
                    brush.temporary = new Circle();
                    break;
                case ShapeType::RECTNAGLE:
                    brush.temporary = new Rectangle();
                    ((Rectangle*)&brush.temporary)->rType = brush.rType;
                    break;
                }
                
                brush.temporary->position = mousePosition;
            }
            else
            {
                float distToMouse = (float)brush.temporary->position.dist(mousePosition);
                
                switch (brush.type)
                {
                case ShapeType::CIRCLE:
                    ((Circle*)brush.temporary)->radius = distToMouse/2.0f;
                    break;
                case ShapeType::RECTNAGLE:
                    ((Rectangle*)brush.temporary)->size = Vector2(mousePosition.x - brush.temporary->position.x, mousePosition.y - brush.temporary->position.y);
                    break;
                }
                
                switch (brush.temporary->type)
                {
                case ShapeType::CIRCLE:
                    window.draw(drawCirle(*((Circle*)brush.temporary)));
                    break;
                case ShapeType::RECTNAGLE:
                    window.draw(drawRectangle(*((Rectangle*)brush.temporary)));
                    break;
                }

                brush.placeable = true;

                // Uncommenting this fucks
               // printf("%d, %d\n", (int)((Rectangle*)&brush.temporary)->rType, (int)brush.rType);
            }
        }
        else
        {
            brush.pressed = false;
            if (brush.placeable)
            {
                brush.placeable = false;

                drawables.pushBack(brush.temporary);

                brush.temporary = nullptr;
            }
        }
            

        // Draw Frame
        for (Drawable* toDraw : drawables)
        {
            switch (toDraw->type)
            {
            case ShapeType::CIRCLE:
                window.draw(drawCirle(*((Circle*)toDraw)));
                break;
            case ShapeType::RECTNAGLE:
                window.draw(drawRectangle(*((Rectangle*)toDraw)));
                break;
            }
        }

        window.display();
    }

    return 0;
}