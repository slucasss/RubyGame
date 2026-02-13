#include "DetectionArea.h"
#include "Debug.h"


void DetectionArea::Draw() {
	Debug::Get()->DrawCircle(GetPosition().x, GetPosition().y, GetRadius(), sf::Color(255, 255, 0, 25));
}
