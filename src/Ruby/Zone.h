#pragma once
#include "Entity.h"
class Zone : public Entity{
private:
	int m_team;
	friend class Rubyman;

public:

	Zone() : m_team(0) {};

	int GetTeam() { return m_team; };
	void SetTeam(int team) { m_team = team; };

	void OnCollision(Entity* collidewith) override;
};

