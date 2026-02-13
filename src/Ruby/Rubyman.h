#pragma once
#include "Entity.h"
#include "StateMachine.h"

class Rectangle;
class DetectionArea;
class Rubyball;
class Zone;

class Rubyman : public Entity{
	int m_team;
	Rubyball* m_ball;  //Quand le joueur n'a pas la balle, c'est a nullptr

	float m_danger;
	sf::Vector2f m_initialPosition;

	float m_baseSpeed;
	float m_boostedSpeed;

	float m_speed;
	float m_speedBoostTimer;

	float m_force;

	Zone* m_goalZone;
	DetectionArea* m_detection;
	Rectangle* m_detectionRectangle;

public:

	Rubyman() : m_team(0), m_danger(0), m_ball(nullptr), m_goalZone(nullptr), m_detection(nullptr), m_detectionRectangle(nullptr), m_baseSpeed(0), m_boostedSpeed(0), m_speed(0), m_speedBoostTimer(0), m_force(0){}; 

	enum class State {
		Attack,
		Defense,
		DefenseSupport,
		DefenseOffens,
		FollowBall,  
		Pass,
		Support,
		Invulnerable,


		Count
	};


	StateMachine<Rubyman> m_stateMachine;
	int stateCount = (int)State::Count;


	friend class RubyScene;

	friend class AttackingRubymanState;
	friend class DefendingRubymanState;
	friend class FollowBallRubymanState;
	friend class PassRubymanState;
	friend class AttackSupportRubymanState;
	friend class InvulnerableRubymanState;
	friend class DefenseSupportRubymanState;
	friend class DefenseOffenseRubymanState;

	friend class TeamHasBallCondition;
	friend class TeamHasNotBallCondition;
	friend class NobodyHasBallCondition;
	friend class SupportCondition;
	friend class PassCondition;


	void SetInitialPosition() { m_initialPosition = GetPosition(); };
	int GetTeam() { return m_team; };
	float GetDanger() { return m_danger; };
	void SetTeam(int team) { m_team = team; };
	void SetGoalZone(Zone* z) { m_goalZone = z; };
	bool HasBall() { return bool(m_ball); }
	Rubyman::State GetState() const {return static_cast<Rubyman::State>(m_stateMachine.GetState());};

	void OnInitialize() override;
	void OnCollision(Entity* collidedWith) override;
	void OnUpdate() override;
	void EnnemyArround();

	void circleAroundBall();
	void passBlock();

	void sayball();

	void Pass(Rubyman* rubyman);

	void AttackAlgorithm();

};

