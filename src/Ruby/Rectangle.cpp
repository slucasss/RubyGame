#include "Rectangle.h"

Rectangle::Rectangle(sf::Vector2f pos, float widht, float height, Rubyman* owner): m_position(pos), m_width(widht), m_height(height), m_owner(owner){
}

bool Rectangle::IsInside(float x, float y)
{
	if (x > m_position.x && x < m_position.x + m_width && y > m_position.y && y < m_position.y + m_height) {
		return true;
	}
	return false;
}

void Rectangle::Draw() {
	Debug::Get()->DrawLine(m_position.x, m_position.y, m_position.x, m_position.y + m_height, sf::Color::Red);

	Debug::Get()->DrawLine(m_position.x, m_position.y, m_position.x + m_width, m_position.y, sf::Color::Red);

	Debug::Get()->DrawLine(m_position.x, m_position.y + m_height, m_position.x + m_width, m_position.y + m_height, sf::Color::Red);

	Debug::Get()->DrawLine(m_position.x + m_width, m_position.y, m_position.x + m_width, m_position.y + m_height, sf::Color::Red);
}