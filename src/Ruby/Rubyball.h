#pragma once
#include "Entity.h"
#include "Rubyman.h"
class Rubyball: public Entity{
	friend class RubyScene;
	friend class DefendingRubymanState;
	friend class AttackingRubymanState;
	friend class AttackSupportRubymanState;

	friend class Rubyman;

	Rubyman* m_holder = nullptr;



public:


	void Pass() { m_holder = nullptr; };
	
	void OnUpdate() override;

	void OnCollision(Entity* collidedWith) override;
};

