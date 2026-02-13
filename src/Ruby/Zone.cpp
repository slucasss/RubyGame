#include "Zone.h"
#include "Rubyball.h"

void Zone::OnCollision(Entity* collidewith){
	if (auto ball = dynamic_cast<Rubyball*>(collidewith)) {
	
	}
}
