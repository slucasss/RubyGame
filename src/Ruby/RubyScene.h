#pragma once

#include "Scene.h"
#include "Team.h"



inline bool DrawDebug = false;
inline bool PassDebug = false;



class Rubyman;
class Rubyball;
class Zone;
class Team;

class RubyScene : public Scene{
	friend class Rubyman;


	Team* m_firstTeam;
	Team* m_secondTeam;
	int m_teamHasBall;


	Rubyball* m_ball;


	Zone* m_zoneTeam1;
	Zone* m_zoneTeam2;
	
	int mode;
	Rubyman* selected;
	
	void CreateRubyman(int team, int x, int y, float speed, float force);
	void CreateBall();
	void CreateZones(int team, int x, int y);


public:


	int WhoHasBall() { return m_teamHasBall; };
	Rubyball* Getball() const { return m_ball; };

	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;

	void Score(int team);

	Team* GetTeam(int t);
	Team* GetEnnemyTeam(int t);

	~RubyScene();

	Zone* GetGoalZone(int i);

};

