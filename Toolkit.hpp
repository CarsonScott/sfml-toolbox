#ifndef TOOLKIT_HPP_INCLUDED
#define TOOLKIT_HPP_INCLUDED

#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <string>
#include <sstream>

#define PVector sf::Vector2f
#define vect std::vector
#define Array std::vector<float>
#define Matrix std::vector<Array>
#define PI 3.1415926



bool in_range(float val, float lower, float upper){
    if(val > lower && val < upper)
        return true;
    else
        return false;
}

bool in_range(PVector val, PVector lower, PVector upper){
    if(in_range(val.x, lower.x, upper.x) && in_range(val.y, lower.y, upper.y))
        return true;
    else
        return false;
}

float getValue(sf::Color src){
    float val = (src.r + src.g + src.b)/3;
    return val;
}

sf::Image applyGrid(sf::Image& src, sf::Vector2i grid){
    sf::Image newImage = src;
    for(int y = 0; y < newImage.getSize().y; y += grid.y){
        for(int x = 0; x < newImage.getSize().x; x ++){
            newImage.setPixel(x, y, sf::Color::Red);
        }
    }

    for(int x = 0; x < newImage.getSize().x; x += grid.x){
        for(int y = 0; y < newImage.getSize().y; y++){
            newImage.setPixel(x, y, sf::Color::Red);
        }
    }
    return newImage;
}



sf::Color add(sf::Color c1, sf::Color c2){
    float r = c1.r+c2.r;
    float g = c1.g+c2.g;
    float b = c1.b+c2.b;
    float a = c1.a + c2.a;
    return sf::Color(r, g, b, a);
}

sf::Color average(sf::Color c1, sf::Color c2){
    float r = (c1.r+c2.r)/2;
    float g = (c1.g+c2.g)/2;
    float b = (c1.b+c2.b)/2;

    return sf::Color(r, g, b);
}

sf::Color subtract(sf::Color c1, sf::Color c2){
    float r = c1.r-c2.r;
    float g = c1.g-c2.g;
    float b = c1.b-c2.b;
    return sf::Color(r, g, b);
}


sf::Image scaleDown(sf::Image& src, PVector kSize, bool avg=true){
    std::vector<sf::Vector2i> points;
    for(int x = 0; x < src.getSize().x-kSize.x; x += kSize.x){
        for(int y = 0; y < src.getSize().y-kSize.y; y += kSize.y){
            points.push_back(sf::Vector2i(x, y));
        }
    }

    sf::Image dst;
    dst.create(floor(src.getSize().x/kSize.x), floor(src.getSize().y/kSize.y));
    for(int i = 0; i < points.size(); i++){
        float r = 0;
        float g = 0;
        float b = 0;
        for(int x = points[i].x; x < points[i].x+kSize.x; x++){
            for(int y = points[i].y; y < points[i].y+kSize.y; y++){
                sf::Color c = src.getPixel(x, y);
                if(avg){
                    r += c.r;
                    g += c.g;
                    b += c.b;
                }
                else if((r+g+b)/3 < (c.r+c.g+c.b)/3){
                    r = c.r;
                    g = c.g;
                    b = c.b;
                }
            }
        }
        int pixels = kSize.x*kSize.y;

        if(avg){
            r /= pixels;
            g /= pixels;
            b /= pixels;
        }
        dst.setPixel(points[i].x/kSize.x, points[i].y/kSize.y, sf::Color(r, g, b));
    }

    return dst;
}

sf::Image scaleLight(sf::Image& src, PVector kSize, int thresh, int a){
    std::vector<sf::Vector2i> points;
    for(int x = 0; x < src.getSize().x-kSize.x; x += kSize.x){
        for(int y = 0; y < src.getSize().y-kSize.y; y += kSize.y){
            points.push_back(sf::Vector2i(x, y));
        }
    }

    sf::Image dst;
    dst.create(floor(src.getSize().x/kSize.x), floor(src.getSize().y/kSize.y));
    for(int i = 0; i < points.size(); i++){
        float r = 0;
        float g = 0;
        float b = 0;
        for(int x = points[i].x; x < points[i].x+kSize.x; x++){
            for(int y = points[i].y; y < points[i].y+kSize.y; y++){
                sf::Color c = src.getPixel(x, y);
                r += c.r;
                g += c.g;
                b += c.b;
            }
        }
        int pixels = kSize.x*kSize.y;

        r /= pixels;
        g /= pixels;
        b /= pixels;



        if((r+g+b)/3 < thresh){
            a *= -1;
        }

        r += a;
        g += a;
        b += a;

        if(r > 255) r = 255;
        else if(r < 0) r = 0;
        if(g > 255) g = 255;
        else if(g < 0) g = 0;
        if(b > 255) b = 255;
        else if(b < 0) b = 0;

        dst.setPixel(points[i].x/kSize.x, points[i].y/kSize.y, sf::Color(r, g, b));
    }

    return dst;
}

class Lever: public sf::Text{
    bool active;
public:
    Lever(){
        active = false;
    }

    Lever(std::string txt){
        setString(txt);
        active = false;
    }

    void handleEvent(sf::Event& event, PVector mouse){
        if(event.type == sf::Event::MouseButtonPressed){
            if(getGlobalBounds().contains(mouse)){
                if(active)
                    active = false;
                else
                    active = true;
            }
        }
    }

    bool getState(){
        return active;
    }

    void reset(){
        active = false;
    }
};

/*---------------------------------------------------*/

int random(double limit, int base=0)
{
    int r = rand() % int(limit) + base;
    if(r >= limit)
    {
        r = limit;
    }
    return r;
}

std::string random_str(int length)
{
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz1234567890";
    std::string str = "";
    for(int i = 0; i < length; i++)
    {
        str += alphabet.at(random(alphabet.size()));
    }
    return str;
}

std::string to_string(float number)
{
    std::stringstream ss(std::stringstream::in | std::stringstream::out);
    ss << number;
    return ss.str();
}

sf::Color getRandomColor(){
    return sf::Color(random(255), random(255), random(255));
}

/*---------------------------------------------------*/

PVector operator +(PVector a, PVector b){
    return PVector(a.x+b.x, a.y+b.y);
}

PVector operator -(PVector a, PVector b){
    return PVector(a.x-b.x, a.y-b.y);
}

PVector operator *(PVector a, PVector b){
    return PVector(a.x*b.x, a.y*b.y);
}

PVector getOffset(PVector a, PVector b)
{
    PVector offset(b.x - a.x, b.y - a.y);
    return offset;
}

float getDistance(PVector a, PVector b)
{
    PVector offset = getOffset(a, b);
    float distance = sqrt(pow(offset.x, 2) + pow(offset.y, 2));
    return distance;
}

PVector getCenter(sf::FloatRect rect)
{
    PVector center(rect.width/2, rect.height/2);
    return center;
}

PVector normalize(PVector v)
{
    PVector p;
    if(abs(v.x) > 1)
    {
        p.x = v.x / abs(v.x);
    }
    if(abs(v.y) > 1)
    {
        p.y = v.y / abs(v.y);
    }
    return p;
}

PVector mult(PVector v, float n)
{
    PVector p;
    p.x = v.x*n;
    p.y = v.y*n;
    return p;
}

PVector mult(PVector a, PVector b)
{
    PVector p;
    p.x = a.x*b.x;
    p.y = a.y*b.y;
    return p;
}

PVector div(PVector v, float n){
    PVector p;
    p.x = v.x/n;
    p.y = v.y/n;
    return p;
}

PVector sub(PVector a, PVector b)
{
    PVector d;
    d.x = a.x - b.x;
    d.y = a.y - b.y;
    return d;
}

PVector add(PVector a, PVector b)
{
    PVector s;
    s.x = a.x + b.x;
    s.y = a.y + b.y;
    return s;
}

PVector limit(PVector v, float m)
{
    PVector p = v;
    if(abs(v.x) > m){
        p.x = m * (v.x/abs(v.x));
    }

    if(abs(v.y) > m){
        p.y = m * (v.y/abs(v.y));
    }
    return p;
}

PVector sum(std::vector<PVector> vectors){
    PVector sum;
    for(int i = 0; i < vectors.size(); i++){
        sum = add(sum, vectors[i]);
    }
    return sum;
}

/*---------------------------------------------------*/

class AdjustableView: public sf::View
{
public:
    void handleEvent(sf::Event& event)
    {
        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::S){
                zoom(1.5);

            }
            if(event.key.code == sf::Keyboard::W){
                zoom(.75);
            }

            if(event.key.code == sf::Keyboard::Left){
                setCenter(PVector(getCenter()) + PVector(-100, 0));
            }
            if(event.key.code == sf::Keyboard::Right){
                setCenter(PVector(getCenter()) + PVector(100, 0));
            }
            if(event.key.code == sf::Keyboard::Up){
                setCenter(PVector(getCenter()) + PVector(0, -100));
            }
            if(event.key.code == sf::Keyboard::Down){
                setCenter(PVector(getCenter()) + PVector(0, 100));
            }
        }
    }
};

/*---------------------------------------------------*/

class Button: public sf::Text
{
    bool active;
public:
    Button()
    {
        active = false;
    }

    void handleEvent(sf::Event& event, PVector mousePos)
    {
        if(event.type == sf::Event::MouseButtonPressed)
        {
            if(getGlobalBounds().contains(mousePos))
            {
                active = true;
            }
        }
    }

    bool selected()
    {
        return active;
    }

    void update()
    {
        active = false;
    }
};

/*---------------------------------------------------*/

class Line: public sf::RectangleShape
{
    float thickness;
public:
    Line(float thick=5){
        thickness = thick;
        setFillColor(sf::Color(200, 200, 200, 150));
    }

    void setThickness(float thick){
        thickness = thick;
    }

    void setPoints(PVector a, PVector b)
    {
        sf::Color c = getFillColor();
        c.a = 100;
        setFillColor(c);

        setPosition(a);
        float d = getDistance(a, b);
        setSize(PVector(d, thickness));
        setOrigin(0, getSize().y/2);
        PVector offset = getOffset(a, b);
        float angle = atan2(offset.y, offset.x);
        setRotation(angle*180/PI);
    }
};

/*---------------------------------------------------*/

class Walker: public sf::CircleShape
{
    float timer;
    float period;
    PVector velocity;
    unsigned int step_count;
    sf::FloatRect boundary;

    void step()
    {
        step_count ++;
        if(random(10) <= 5)
        {
            velocity.x *= -1;
        }
        else
        {
            velocity.y *= -1;
        }
    }

public:
    Walker(float step_time, sf::FloatRect bounds)
    {
        period = step_time;
        timer = 0;

        float speed = 125;
        velocity = PVector(speed, speed);
        step_count = 0;

        boundary = bounds;
    }

    void update(float dt)
    {
        timer += dt;
        if(timer >= period)
        {
            timer = 0;
            step();
        }

        move(velocity.x*dt, velocity.y*dt);
        if(!boundary.contains(getPosition()))
        {
            setPosition(random(boundary.width), random(boundary.height));
        }
    }

    unsigned int get_steps()
    {
        return step_count;
    }
};

/*---------------------------------------------------*/

class CheckerBoard
{
    PVector cellSize;
    PVector boardSize;
    std::vector<sf::Color> colors;
    std::vector<sf::RectangleShape> rects;

    void generateColors(int cLimit)
    {
        for(int i = 0; i < cLimit; i++)
        {
            sf::Color color(random(255, 5), random(255, 5), random(255, 5), random(255, 5));
            colors.push_back(color);
        }
    }
public:
    CheckerBoard(PVector windowSize, int rows, int col)
    {
        boardSize = windowSize;
        cellSize.x = boardSize.x / rows;
        cellSize.y = boardSize.y / col;

        colors.clear();
        generateColors( random(sqrt(rows + col), 2) );

        int counter = 0;
        for(float i = 0; i < windowSize.x; i+=cellSize.x)
        {
            for(float j = 0; j < windowSize.y; j+=cellSize.y)
            {
                sf::RectangleShape rect;
                rect.setPosition(i, j);
                rect.setSize(cellSize);

                sf::Color color = colors[counter];
                rect.setFillColor(color);
                counter ++;

                if(counter == colors.size())
                {
                    counter = 0;
                }

                rects.push_back(rect);
            }
        }
    }
    void draw(sf::RenderWindow& window)
    {
        for(int i = 0; i < rects.size(); i++)
        {
            window.draw(rects[i]);
        }
    }
    void draw(sf::RenderTexture& texture)
    {
        for(int i = 0; i < rects.size(); i++)
        {
            texture.draw(rects[i]);
        }
    }
};

/*---------------------------------------------------*/

class Trackbar
{
    bool active;
    unsigned int limit;
    unsigned int value;

    sf::RectangleShape track;
    sf::RectangleShape bar;

    void setBar(PVector pos)
    {
        PVector barPos = bar.getPosition();
        barPos.x = pos.x;

        PVector range(track.getPosition().x, track.getPosition().x + track.getSize().x);
        if(barPos.x < range.x)
        {
            barPos.x = range.x;
        }
        else if(barPos.x > range.y)
        {
            barPos.x = range.y;
        }

        bar.setPosition(barPos);
    }
public:
    Trackbar(unsigned int length, unsigned int val=0)
    {
        active = false;
        limit = length;
        value = val;

        track.setFillColor(sf::Color(200, 200, 150, 200));
        track.setOutlineColor(sf::Color(255, 255, 255));
        track.setOutlineThickness(1);

        track.setSize(PVector(300, 10));
        track.setOrigin(0, track.getSize().y/2);

        bar.setFillColor(sf::Color(150, 75, 75, 150));
        bar.setOutlineColor(sf::Color(150, 150, 150));
        bar.setOutlineThickness(1);

        bar.setSize(PVector(15, 20));
        bar.setOrigin(getCenter(bar.getGlobalBounds()));

        PVector barPos = track.getPosition();
        barPos.x += limit/track.getSize().x * value;
        bar.setPosition(barPos);
    }

    void setPosition(PVector pos)
    {
        track.setPosition(pos);

        PVector barPos = track.getPosition();
        barPos.x += limit/track.getSize().x * value;
        bar.setPosition(barPos);
    }

    PVector getPosition(){
        return track.getPosition();
    }

    PVector getSize(){
        return track.getSize();
    }

    void handleEvent(sf::Event& event, PVector mousePos)
    {
        if(event.type == sf::Event::MouseButtonPressed)
        {
            if(track.getGlobalBounds().contains(mousePos) || bar.getGlobalBounds().contains(mousePos))
            {
                active = true;
            }
        }
        else if(event.type == sf::Event::MouseButtonReleased)
        {
            if(active)
            {
                active = false;
            }
        }

        if(active)
        {
            setBar(mousePos);
        }
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(track);
        window.draw(bar);
    }

    unsigned int getValue()
    {
        int val = bar.getPosition().x - track.getPosition().x;
        return val * limit/track.getSize().x;
    }
};

/*---------------------------------------------------*/

class TileSet
{
    std::vector<sf::Texture> tiles;
public:
    TileSet(sf::Image& src, PVector tileSize)
    {
        sf::Vector2u imgSize = src.getSize();
        for(int y = 0; y < imgSize.y; y+=tileSize.y)
        {
            for(int x = 0; x < imgSize.x; x+=tileSize.x)
            {
                sf::Texture tile;
                tile.loadFromImage(src, sf::IntRect(sf::Vector2i(x, y), sf::Vector2i(tileSize)));
                tiles.push_back(tile);
            }
        }
    }

    unsigned int size()
    {
        return tiles.size();
    }
};

/*---------------------------------------------------*/

struct Brush
{
    PVector position;
    float radius;
    sf::Color effect;
};

class ImageEditor
{
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
public:
    ImageEditor(PVector imgSize)
    {
        image.create(imgSize.x, imgSize.y, sf::Color(25, 50, 100));
        texture.loadFromImage(image);
        sprite.setTexture(texture);
    }

    void edit(Brush& brush)
    {
        PVector pos = brush.position;
        float r = brush.radius;
        for(int y = pos.y-r; y < pos.y+r; y++)
        {
            for(int x = pos.x-r; x < pos.x+r; x++)
            {
                float d = getDistance(pos, PVector(x, y));
                if(sprite.getGlobalBounds().contains(x, y) && d < r)
                {
                    float a = pow(d, 2);// * pow(r, 2);
                    sf::Color c = image.getPixel(x, y);

                    c.r += brush.effect.r*a;
                    c.g += brush.effect.g*a;
                    c.b += brush.effect.b*a;

                    image.setPixel(x, y, c);
                }
            }
        }
    }

    void draw(sf::RenderWindow& window)
    {
        texture.update(image);
        sprite.setTexture(texture);
        window.draw(sprite);
    }

    sf::Image& getImage()
    {
        return image;
    }
};

/*---------------------------------------------------*/

class Gallery
{
    std::vector<sf::Texture*> textures;
    std::vector<sf::Sprite*> sprites;
    bool active;
    sf::Sprite* selected;
    sf::FloatRect boundary;
public:
    Gallery(std::vector<sf::Image>& images, sf::FloatRect bounds)
    {
        boundary = bounds;
        active = false;
        selected = NULL;
        for(int i = 0; i < images.size(); i++)
        {
            sf::Texture* t = new sf::Texture();
            t->loadFromImage(images[i], sf::IntRect(0, 0, sf::Texture::getMaximumSize(), sf::Texture::getMaximumSize()));
            textures.push_back(t);

            sf::Sprite* s = new sf::Sprite();
            s->setTexture(*textures.back());
            s->setOrigin(getCenter(s->getGlobalBounds()));
            s->setScale(.2, .2);
            sprites.push_back(s);
        }
    }

    void handleEvent(sf::Event& event, PVector mousePos)
    {
        if(event.type == sf::Event::MouseButtonPressed)
        {
            if(!active)
            {
                for(int i = sprites.size()-1; i >= 0; i--)
                {
                    if(sprites[i]->getGlobalBounds().contains(mousePos))
                    {
                        active = true;

                        selected = sprites[i];
                        sprites.erase(sprites.begin() + i);
                        sprites.push_back(selected);
                        selected->setScale(.22, .22);
                        break;
                    }
                }
            }
        }
        else if(event.type == sf::Event::MouseButtonReleased)
        {
            if(active)
            {
                active = false;
                selected->setScale(.2, .2);
                selected = NULL;
            }
        }
    }

    void update(float dt, PVector mousePos, float speed=0)
    {
        if(active)
        {
            if(selected->getPosition() != mousePos)
            {
                PVector v = sub(mousePos, selected->getPosition());
                v = mult(v, 8);
                selected->move(v.x*dt, v.y*dt);
            }
        }

        for(int i = 0; i < sprites.size(); i++)
        {
            for(int j = 0; j < sprites.size(); j++)
            {
                sf::FloatRect img_a = sprites[i]->getGlobalBounds();
                sf::FloatRect img_b = sprites[j]->getGlobalBounds();

                PVector pos_a = sprites[i]->getPosition();
                PVector pos_b = sprites[j]->getPosition();

                if(i!=j && img_a.intersects(img_b))
                {
                    float area_a = img_a.width * img_a.height;
                    float area_b = img_b.width * img_b.height;

                    PVector v = sub(pos_a, pos_b);
                    if(speed != 0)
                        v = mult(v, speed);
                    sprites[i]->move(v.x*dt, v.y*dt);
                }

            }
        }
    }

    void organize(PVector pos)
    {
        for(int i = 0; i < sprites.size(); i++)
        {
            sprites[i]->setPosition(50 + i*sprites[i]->getGlobalBounds().width/2, 50);
            if(i > 0)
            {
                sprites[i]->move(sprites[i-1]->getGlobalBounds().width/2, 0);
            }

            pos.x -= random(10);
            pos.y -= random(10);
            sprites[i]->setPosition(pos);
        }
    }

    void draw(sf::RenderWindow& window)
    {
        for(int i = 0; i < sprites.size(); i++)
        {
            window.draw(*sprites[i]);
        }
    }
};

/*---------------------------------------------------*/

#endif // TOOLKIT_HPP_INCLUDED
