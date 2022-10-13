#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <typeinfo>

using namespace sf;
using namespace std;



class Rectagle
{
public:
	RectangleShape shape;
	void set_position(float x, float y) { shape.setPosition(x, y); }
	void set_size(float x, float y) { shape.setSize(Vector2f(x, y)); }
	float left() const { return shape.getPosition().x; }
	float right() const { return shape.getPosition().x + shape.getSize().x; }
	float top() const { return shape.getPosition().y; }
	float bottom() const { return shape.getPosition().y + shape.getSize().y; }

};


