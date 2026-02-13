#pragma once
#include <vector>
#include <map>


class Rubyman;


class Team{
	std::vector<Rubyman*> m_team;
	std::vector<Rubyman*> m_offdef;
	std::vector<Rubyman*> m_supdef;

	std::map<float, Rubyman*> m_lineRepartition;

	Rubyman* m_hasBall;
	
	int m_score;
	int m_flag;

	bool m_isPassing;

	friend Rubyman;
	friend class RubyScene;
	friend Rubyman;
	friend class PassRubymanState;
	friend class AttackingRubymanState;
	friend class DefendingRubymanState;
	friend class AttackSupportRubymanState;
	friend class DefenseOffenseRubymanState;
	friend class DefenseSupportRubymanState;

	friend class PassCondition;

public:

	Team() : m_isPassing(false), m_flag(0), m_score(0), m_hasBall(nullptr) {};

	Team(int flag, RubyScene* scene);

	bool HasBall() { return (bool)m_hasBall; };
	int GetScore(){ return m_score; };
	void IncreaseScore(int score) { m_score += score; };


	void AddRubyman(Rubyman* rubyman);

	void Update();

	Rubyman* iterate(int i) { return m_team[i]; };
	int GetSize() { return m_team.size(); };



	void Pass(bool p) { m_isPassing = p; };
	bool GetPass() { return m_isPassing; };

	void ResetLineRepartition();

};

