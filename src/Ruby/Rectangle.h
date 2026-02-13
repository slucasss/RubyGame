#pragma once
#include "Utils.h"
#include "Debug.h"

class Rubyman;


class Rectangle{

	friend Rubyman;

	sf::Vector2f m_position;  
	float m_width;			
	float m_height;

	Rubyman* m_owner;

public:
	Rectangle(sf::Vector2f pos, float widht, float height, Rubyman* owner);

	void Draw();

	bool IsInside(float x, float y);


};

