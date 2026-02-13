#include <iostream>
#include <vector>
#include <Math.h>
#include <string>
#include "Rubyman.h"

#include <Utils.h>
#include "Debug.h"


#include "Rubyball.h"
#include "RubymanStates.h"
#include "RubyScene.h"

#include "Team.h"
#include "DetectionArea.h"
#include "Rectangle.h"
#include "Zone.h"

constexpr float M_PI = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097566593344612847564823378678316527120190914564856692346034861045432664821339360726;


void Rubyman::OnInitialize(){

	{

		m_stateMachine.AddState(new AttackingRubymanState);
		

		Transition<Rubyman>* t = m_stateMachine.AddTransition((int)State::Attack, (int)State::Defense);
		t->AddCondition(new EnnemyHasBallCondition);

		t = m_stateMachine.AddTransition((int)State::Attack, (int)State::FollowBall);
		t->AddCondition(new NobodyHasBallCondition);

		t = m_stateMachine.AddTransition((int)State::Attack, (int)State::Support);
		t->AddCondition(new SupportCondition);


	}

	{
		m_stateMachine.AddState(new DefendingRubymanState);
	}

	{
		m_stateMachine.AddState(new DefenseSupportRubymanState);
		Transition<Rubyman>* t = m_stateMachine.AddTransition((int)State::DefenseSupport, (int)State::Invulnerable);
		t->AddCondition(new TeamHasBallCondition);

		t = m_stateMachine.AddTransition((int)State::DefenseSupport, (int)State::Support);
		t->AddCondition(new SupportCondition);

		t = m_stateMachine.AddTransition((int)State::DefenseSupport, (int)State::FollowBall);
		t->AddCondition(new NobodyHasBallCondition);
	}

	{
		m_stateMachine.AddState(new DefenseOffenseRubymanState);
		Transition<Rubyman>* t = m_stateMachine.AddTransition((int)State::DefenseOffens, (int)State::Invulnerable);
		t->AddCondition(new TeamHasBallCondition);

		t = m_stateMachine.AddTransition((int)State::DefenseOffens, (int)State::Support);
		t->AddCondition(new SupportCondition);

		t = m_stateMachine.AddTransition((int)State::DefenseOffens, (int)State::FollowBall);
		t->AddCondition(new NobodyHasBallCondition);
	}
	
	{
		m_stateMachine.AddState(new FollowBallRubymanState);

		Transition<Rubyman>* t = m_stateMachine.AddTransition((int)State::FollowBall, (int)State::Invulnerable);
		t->AddCondition(new TeamHasBallCondition);

		t = m_stateMachine.AddTransition((int)State::FollowBall, (int)State::Support);
		t->AddCondition(new SupportCondition);

		t = m_stateMachine.AddTransition((int)State::FollowBall, (int)State::Defense);
		t->AddCondition(new EnnemyHasBallCondition);
	}
	
	{
		m_stateMachine.AddState(new PassRubymanState);

		
		Transition<Rubyman>* t = m_stateMachine.AddTransition((int)State::Pass, (int)State::Defense);
		t->AddCondition(new EnnemyHasBallCondition);
		
	}

	{
		m_stateMachine.AddState(new AttackSupportRubymanState);


		Transition<Rubyman>* t = m_stateMachine.AddTransition((int)State::Support, (int)State::Defense);
		t->AddCondition(new EnnemyHasBallCondition);

		t = m_stateMachine.AddTransition((int)State::Support, (int)State::FollowBall);
		t->AddCondition(new NobodyHasBallCondition);

		t = m_stateMachine.AddTransition((int)State::Support, (int)State::Invulnerable);
		t->AddCondition(new TeamHasBallCondition);
	}

	{
		m_stateMachine.AddState(new InvulnerableRubymanState);
	}
	
	
	m_detection = CreateEntity<DetectionArea>(150, sf::Color(255, 0, 0, 0));
	m_detection->SetPosition(GetPosition().x, GetPosition().y);

}

void Rubyman::OnCollision(Entity* collidedWith){
	
	
	if (Rubyman* rubyman = dynamic_cast<Rubyman*>(collidedWith)) {
		if (m_ball != nullptr) {
			if (rubyman->m_team != m_team && m_stateMachine.GetState() != (int)State::Invulnerable) {  //Si un jouer ayant la balle entre en collision avec un joueur adverse, l'adversaire recuperer la balle. Problème, y a pas de temps ou la balle ne peut aps etre recuperer, donc ca s'execute a chaque frame

					rubyman->m_ball = m_ball;
					rubyman->m_ball->m_holder = rubyman;

					m_ball = nullptr;

					rubyman->m_speed = rubyman->m_boostedSpeed;
					rubyman->m_speedBoostTimer = 1.5;

					rubyman->m_ball->SetPosition(rubyman->GetPosition().x, rubyman->GetPosition().y);
					rubyman->m_stateMachine.TryTransitionTo(this, (int)State::Invulnerable);
			}
		}
	}
	else if (Rubyball* ball = dynamic_cast<Rubyball*>(collidedWith)){ //Et si il entre en contact direct avec la balle, il la recupere
		if (m_ball == nullptr) {
			m_ball = ball;
			ball->m_holder = this;
			m_speed = m_boostedSpeed;
			m_speedBoostTimer = 1.5;

			m_ball->SetPosition(GetPosition().x, GetPosition().y);
			m_stateMachine.TryTransitionTo(this, (int)State::Invulnerable);
		}	
	}
}

void Rubyman::OnUpdate(){
	if (m_ball != nullptr && this != this->m_ball->m_holder) {
		m_ball = nullptr;
	}


	if (GetPosition().y < GetRadius()) {
		SetPosition(GetPosition().x, GetRadius());
	}
	if (GetPosition().y > GetScene()->GetWindowHeight() - GetRadius()) {
		SetPosition(GetPosition().x, GetScene()->GetWindowHeight()- GetRadius());
	}


	if (m_ball != nullptr) {
		m_ball->SetPosition(GetPosition().x, GetPosition().y);  //On place la ball au centre du joueur qui l'a
	}
	else { //Si un joueur n'a pas la balle, il a une vitesse normal
		m_speed = m_baseSpeed;
	}
	
	m_detection->SetPosition(GetPosition().x, GetPosition().y);
	EnnemyArround();

	m_stateMachine.Update(this, GetDeltaTime());

	
	sf::Vector2f pos;
	if (m_team == 1) {
		pos = sf::Vector2f(GetPosition().x, GetPosition().y - m_detectionRectangle->m_height / 2);
	}
	else if (m_team == 2) {
		pos = sf::Vector2f(GetPosition().x - m_detectionRectangle->m_width, GetPosition().y - m_detectionRectangle->m_height /2);
	}
	m_detectionRectangle->m_position = pos;
	
	
	if (m_speedBoostTimer > 0.f) {
		m_speedBoostTimer -= GetDeltaTime();

	}
	else{
		m_speedBoostTimer = 0.f;
		m_speed = m_baseSpeed;
	}
	
	

	if (DrawDebug) {
		m_detection->Draw();
	}
	
}

void Rubyman::EnnemyArround() {
	RubyScene* scene = GetScene<RubyScene>();
	float danger = 0.f;

	{
		Team* team = scene->GetEnnemyTeam(m_team);

		for (int i = 0; i < team->GetSize(); i++) {
			if (m_detection->IsColliding(team->iterate(i))) {
				danger += 100 / Utils::GetDistance(GetPosition().x, GetPosition().y, team->iterate(i)->GetPosition().x, team->iterate(i)->GetPosition().y);
			}
		}
	}

	m_danger = danger;
}

void Rubyman::circleAroundBall(){
	Team* team = GetScene<RubyScene>()->GetTeam(m_team);
	Rubyball* ball = GetScene<RubyScene>()->Getball();
	int around = 0;
	around = 360 / (team->m_offdef.size() + 1);
	for (int i = 0; i < team->m_offdef.size(); i++) {
		switch (m_team) {
		case 1:
			team->m_offdef[i]->GoToPosition(ball->GetPosition().x + 40 * cos(around * i), ball->GetPosition().y + 40 * sin(around * i), m_speed);
			break;
		case 2:
			team->m_offdef[i]->GoToPosition(ball->GetPosition().x + 40 * cos(around * i + 180), ball->GetPosition().y + 40 * sin(around * i + 180), m_speed);
			break;
		}
	}
}

void Rubyman::passBlock(){
	Team* ennemyTeam = GetScene<RubyScene>()->GetEnnemyTeam(this->GetTeam());
	Team* team = GetScene<RubyScene>()->GetTeam(this->GetTeam());
	float BallX = GetScene<RubyScene>()->Getball()->GetPosition().x;
	float BallY = GetScene<RubyScene>()->Getball()->GetPosition().y;
	int index = 0;
	for (int i = 0; i < ennemyTeam->m_team.size(); i++) {
		for (int j = index; j < team->m_supdef.size(); j++) {
			if (ennemyTeam->m_team[i]->m_ball == nullptr) {
				team->m_supdef[j]->GoToPosition((ennemyTeam->m_team[i]->GetPosition().x + BallX) / 2, (ennemyTeam->m_team[i]->GetPosition().y + BallY) / 2, m_speed);
				index++;
				break;
			}
		}
	}
}

void Rubyman::sayball()
{
	if (m_ball != nullptr) {
		std::cout << "J'ai la balle" << std::endl;
	}
	else {
		std::cout << "J'ai pas la balle" << std::endl;
	}
}

void Rubyman::Pass(Rubyman* rubyman){
	if (m_ball == nullptr || rubyman == nullptr) {
		return;
	}

	sf::Vector2f dir = rubyman->GetPosition() - GetPosition();
	Utils::Normalize(dir);
	
	sf::Vector2f coor = (GetPosition() + (dir * GetRadius())) + dir * m_ball->GetRadius() ;

	m_ball->SetPosition(coor.x, coor.y);
	m_ball->Pass();

	m_ball->GoToPosition(rubyman->GetPosition().x, rubyman->GetPosition().y, m_force);
}

void Rubyman::AttackAlgorithm(){
	//On prend des points autour du joueur, on applique un algortihme (le point fait deplacer vers le but, vers des ennemies, vers des alliés) et ensuite on prend le meilleur et on se deplace par la bas
	std::vector<std::pair<sf::Vector2f, float>> points;


	int nbrPoint = 14;

	float angle = (2 * M_PI) / nbrPoint;
	int distance = 100;
	
	for (int i = 0; i < nbrPoint; i++) {
		float x = GetPosition().x + distance * cos(angle * i);
		float y = GetPosition().y + distance * sin(angle * i);

		sf::Vector2f coor{ x, y };
		points.push_back({ coor, 0.f });
	}

	for (auto& i : points) {
		RubyScene* scene = dynamic_cast<RubyScene*>(GetScene());
		std::vector<Rubyman*> EnnemyTeam = scene->GetEnnemyTeam(GetTeam())->m_team;
		for (auto e : EnnemyTeam) {
			i.second += Utils::GetDistance(e->GetPosition().x, e->GetPosition().y, i.first.x, i.first.y) ;
		}


		if (GetTeam() == 1) {
			i.second -= Utils::GetDistance(i.first.x, i.first.y, m_goalZone->GetPosition().x - m_goalZone->GetRadius() + 100, GetScene()->GetWindowHeight()/2) * 5.1f;
		}
		else /*if(GetTeam() == 2)*/ {
			i.second -= Utils::GetDistance(i.first.x, i.first.y, m_goalZone->GetPosition().x + m_goalZone->GetRadius() - 100 , GetScene()->GetWindowHeight() / 2) * 5.1f;
		}

	}		
	std::sort(points.begin(), points.end(), [](const auto a, const auto b) {return a.second > b.second; });
	
	for (auto& it : points) {
		GoToPosition(it.first.x, it.first.y, m_speed);
		break;
	}

	if (DrawDebug == true) {
		for (auto& it : points) {
			Debug::Get()->DrawCircle(it.first.x, it.first.y, 5, sf::Color::Red);
			Debug::Get()->DrawText(it.first.x - 10, it.first.y - 30, std::to_string(it.second), sf::Color::Blue);
		}
	}
	
}

