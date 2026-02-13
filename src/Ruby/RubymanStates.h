#pragma once
#include "StateBase.h"
#include "ICondition.h"
#include "Transition.h"

class Rubyman;
class Zone;

class AttackingRubymanState : public StateBase<Rubyman> {

    friend Rubyman;
    friend class InvulnerableRubymanState;

    Zone* m_goalZone;

    void Start(Rubyman* type) override ;
    void Update(Rubyman* type, float dt) override;
    void End(Rubyman* type) override ;

	bool CanTransitionFrom(Rubyman* type, int currentState) override;
};

class AttackSupportRubymanState : public StateBase<Rubyman> {

    friend Rubyman;
    friend class InvulnerableRubymanState;

    void Start(Rubyman* type) override;
    void Update(Rubyman* type, float dt) override;
    void End(Rubyman* type) override;

    bool CanTransitionFrom(Rubyman* type, int currentState) override;
};

class DefendingRubymanState : public StateBase<Rubyman> {
    void Start(Rubyman* type) override;
    void Update(Rubyman* type, float dt) override;
    void End(Rubyman* type) override;

    bool CanTransitionFrom(Rubyman* type, int currentState) override;
};

class DefenseSupportRubymanState : public StateBase<Rubyman> {
    void Start(Rubyman* type) override;
    void Update(Rubyman* type, float dt) override;
    void End(Rubyman* type) override;

    bool CanTransitionFrom(Rubyman* type, int currentState) override;
};

class DefenseOffenseRubymanState : public StateBase<Rubyman> {
    float m_timer = 0;
	float m_cooldown = 0.5f;
    void Start(Rubyman* type) override;
    void Update(Rubyman* type, float dt) override;
    void End(Rubyman* type) override;

    bool CanTransitionFrom(Rubyman* type, int currentState) override;
public:
    DefenseOffenseRubymanState() : m_timer(0), m_cooldown(0.5f){}
};

class FollowBallRubymanState : public StateBase<Rubyman> {
    void Start(Rubyman* type) override;
    void Update(Rubyman* type, float dt) override;
    void End(Rubyman* type) override;

    bool CanTransitionFrom(Rubyman* type, int currentState) override;
};

class PassRubymanState : public StateBase<Rubyman>{
    void Start(Rubyman * type) override;
    void Update(Rubyman* type, float dt) override;
    void End(Rubyman* type) override;

    bool CanTransitionFrom(Rubyman* type, int currentState) override;
};

class InvulnerableRubymanState : public AttackingRubymanState { //En enfant de l'etat d'attaque sinon on peut pas acceder au update et lors de l'invulnerabilit�, le joueur fait rien

    float m_timer;

    void Start(Rubyman* type) override;
    void Update(Rubyman* type, float dt) override;
    void End(Rubyman* type) override;

    bool CanTransitionFrom(Rubyman* type, int currentState) override;
};

class TeamHasBallCondition : public ICondition<Rubyman>{
    bool Test(Rubyman* type) override;
};

class EnnemyHasBallCondition : public ICondition<Rubyman>{
    bool Test(Rubyman* type) override;
};

class NobodyHasBallCondition : public ICondition<Rubyman> {
    bool Test(Rubyman* type) override;
};

class SupportCondition : public ICondition<Rubyman> {
    bool Test(Rubyman* type) override;
};

class PassCondition : public ICondition<Rubyman> {
	friend class AttackSupportRubymanState;
bool Test(Rubyman* type) override; 
};