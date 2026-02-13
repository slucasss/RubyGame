#include "Debug.h"
#include <iostream>

#include "RubymanStates.h"
#include "Rubyman.h"
#include "RubyScene.h"
#include "Rubyball.h"
#include "Zone.h"
#include "Utils.h"
#include "Rectangle.h"
#include <cmath>
#include <vector>

#include <string>



void AttackingRubymanState::Start(Rubyman* type){

}

void AttackingRubymanState::Update(Rubyman* type, float dt) {


	if (PassDebug == true) {
		type->m_stateMachine.TryTransitionTo(type, (int)Rubyman::State::Pass);
		return;
	}


	RubyScene* scene = dynamic_cast<RubyScene*>(type->GetScene());
	Zone* goalZone = scene->GetGoalZone(type->GetTeam());
	if (scene->GetTeam(type->GetTeam())->GetPass() == true) {
		if (type->GetTeam() == 1) {
			type->GoToPosition(goalZone->GetPosition().x - 50, type->GetPosition().y);
		}

		else if (type->GetTeam() == 2) {
			type->GoToPosition(goalZone->GetPosition().x + 50, type->GetPosition().y);
		}
	}

	if (type->m_ball != nullptr) {   //Schema d'attaque du joueur ayant la balle. avance, et si y a trop de monde devant, fait la passe. Possiblement, faire en sorte que les joueurs puissent contourner les ennemies
		
		

		scene->GetTeam(type->GetTeam())->Pass(false);
		


		//On test si il y a des joueurs ennemies dans le rectangle de detection 
		std::map<sf::Vector2f, float> EnnemyMap;
		std::vector<sf::Vector2f> EnnemyInBox;

		Team* EnnemyTeam = scene->GetEnnemyTeam(type->GetTeam());


		sf::Vector2f pos1 = type->GetPosition();

		for (int i = 0; i < EnnemyTeam->GetSize(); i++) {
			Rubyman* e = EnnemyTeam->iterate(i);
			if (type->m_detectionRectangle->IsInside(e->GetPosition().x, e->GetPosition().y)) {
				
				EnnemyInBox.push_back(e->GetPosition());
			}
		}
		if (EnnemyInBox.size() >= 1) {
			type->m_stateMachine.TryTransitionTo(type, (int)Rubyman::State::Pass);
		}


		//Dessin du rectangle de detection a l'avant
		if (DrawDebug == true) {
			type->m_detectionRectangle->Draw();
		}


		type->AttackAlgorithm();

		//type->GoToPosition(x, y, type->m_speed);
	}
}

void AttackingRubymanState::End(Rubyman* type){
	type->GoToPosition(type->GetPosition().x, type->GetPosition().y, 0);
	/*RubyScene* scene = dynamic_cast<RubyScene*>(type->GetScene());
	scene->GetTeam(type->GetTeam())->ResetLineRepartition();*/

}

bool AttackingRubymanState::CanTransitionFrom(Rubyman* type, int currentState) {
	switch (Rubyman::State(currentState)) {
	case Rubyman::State::Attack:
		return false;
	case Rubyman::State::Pass:
		if (type->m_ball != nullptr) {
			return true;
		}
		else return false;
	}
	return true;
}






void AttackSupportRubymanState::Start(Rubyman* type){

}

void AttackSupportRubymanState::Update(Rubyman* type, float dt){
	if (PassDebug == true) {
		return;
	}

	RubyScene* scene = dynamic_cast<RubyScene*>(type->GetScene());
	if (scene->GetTeam(type->GetTeam())->m_hasBall == nullptr) {
		return;
	}

	
	float y = type->GetPosition().y;

	if (!scene->GetTeam(type->GetTeam())->GetPass() && scene->Getball()->m_holder != nullptr) {
		y = scene->Getball()->m_holder->GetPosition().y;
	}

	if (type->GetTeam() == 1 && PassCondition().Test(type) ){ 
		type->GoToPosition(scene->GetTeam(type->GetTeam())->m_hasBall->GetPosition().x - 50, type->GetPosition().y);
	}

	else if (type->GetTeam() == 2 && PassCondition().Test(type)) {
		type->GoToPosition(scene->GetTeam(type->GetTeam())->m_hasBall->GetPosition().x + 50, type->GetPosition().y);
	}

	else if (type->GetTeam() == 1) {
		if (y <= scene->GetWindowHeight()/3) {
			type->GoToPosition(scene->GetTeam(type->GetTeam())->m_hasBall->GetPosition().x - 50, scene->GetWindowHeight());
		}
		else if (y <= scene->GetWindowHeight() * 2 / 3) {
			if (Utils::GetDistance(type->GetPosition().x, type->GetPosition().y, type->GetPosition().x, 0) < Utils::GetDistance(type->GetPosition().x, type->GetPosition().y, type->GetPosition().x, scene->GetWindowHeight())) {
				type->GoToPosition(scene->GetTeam(type->GetTeam())->m_hasBall->GetPosition().x - 50, 0);
			}
			else type->GoToPosition(scene->GetTeam(type->GetTeam())->m_hasBall->GetPosition().x - 50, scene->GetWindowHeight());
		}
		else type->GoToPosition(scene->GetTeam(type->GetTeam())->m_hasBall->GetPosition().x - 50, 0);
	}

	else if (type->GetTeam() == 2) {
		if (y <= scene->GetWindowHeight() / 3) {
			type->GoToPosition(scene->GetTeam(type->GetTeam())->m_hasBall->GetPosition().x - 50, scene->GetWindowHeight());
		}
		else if (y <= scene->GetWindowHeight() * 2 / 3) {
			if (Utils::GetDistance(type->GetPosition().x, type->GetPosition().y, type->GetPosition().x, 0) < Utils::GetDistance(type->GetPosition().x, type->GetPosition().y, type->GetPosition().x, scene->GetWindowHeight())) {
				type->GoToPosition(scene->GetTeam(type->GetTeam())->m_hasBall->GetPosition().x - 50, 0);
			}
			else type->GoToPosition(scene->GetTeam(type->GetTeam())->m_hasBall->GetPosition().x - 50, scene->GetWindowHeight());
		}
		else type->GoToPosition(scene->GetTeam(type->GetTeam())->m_hasBall->GetPosition().x - 50, 0);
	}
}

void AttackSupportRubymanState::End(Rubyman* type){

}

bool AttackSupportRubymanState::CanTransitionFrom(Rubyman* type, int currentState) {
	return true;
}






void FollowBallRubymanState::Start(Rubyman* type){

}

void FollowBallRubymanState::Update(Rubyman* type, float dt){
	if (PassDebug == true) {
		return;
	}
	Team* team = type->GetScene<RubyScene>()->GetTeam(type->GetTeam());
	if (team->GetPass() == false) {   //Si l'equipe n'est pas en train de faire une passe, alors on suit la balle
		Rubyball* ball = type->GetScene<RubyScene>()->Getball();
		type->GoToPosition(ball->GetPosition().x, ball->GetPosition().y, type->m_speed);
	}
}

void FollowBallRubymanState::End(Rubyman* type){
	type->GoToPosition(type->GetPosition().x, type->GetPosition().y, 0);
}

bool FollowBallRubymanState::CanTransitionFrom(Rubyman* type, int currentState)
{
	switch (Rubyman::State(currentState)) {
	case Rubyman::State::FollowBall:
		return false;

	}
	return true;
}
	



	
void DefendingRubymanState::Start(Rubyman* type){
	std::vector <std::pair<Rubyman*, float>> distances;

	Team* team = type->GetScene<RubyScene>()->GetTeam(type->GetTeam());
	float ballX = type->GetScene<RubyScene>()->Getball()->GetPosition().x;
	float ballY = type->GetScene<RubyScene>()->Getball()->GetPosition().y;
	if (team->m_offdef.size() <= 0 && team->m_supdef.size() <= 0) {
		for (int i = 0; i < team->GetSize(); i++) {
			Rubyman* r = team->m_team[i];
			float X = r->GetPosition().x;
			float Y = r->GetPosition().y;
			float dist = Utils::GetDistance(X, Y, ballX, ballY);
			distances.push_back({ r, dist });
		}
		sort(distances.begin(), distances.end(), [](const auto a, const auto b) {return a.second < b.second; });
		int i = 0;
		for (auto it : distances) {
			if (i <= 2) {
				team->m_offdef.push_back(it.first);
			}
			else team->m_supdef.push_back(it.first);
			i++;
		}
	}
}

void DefendingRubymanState::Update(Rubyman* type, float dt) {
	if (PassDebug == true) {
		return;
	}

	Rubyball* ball = type->GetScene<RubyScene>()->Getball();
	Team* team = type->GetScene<RubyScene>()->GetTeam(type->GetTeam());

	if (std::find(team->m_offdef.begin(), team->m_offdef.end(), type) != team->m_offdef.end()) {
		type->m_stateMachine.TryTransitionTo(type, (int)Rubyman::State::DefenseOffens);
	}

	if (std::find(team->m_supdef.begin(), team->m_supdef.end(), type) != team->m_supdef.end()) {
		type->m_stateMachine.TryTransitionTo(type, (int)Rubyman::State::DefenseSupport);

	}


}

void DefendingRubymanState::End(Rubyman* type)
{
	
}

bool DefendingRubymanState::CanTransitionFrom(Rubyman* type, int currentState)
{
	switch (Rubyman::State(currentState)) {
	case Rubyman::State::Defense:
		return false;
	case Rubyman::State::Invulnerable:
		return false;
	}
	return true;
}






void DefenseSupportRubymanState::Start(Rubyman* type){
}

void DefenseSupportRubymanState::Update(Rubyman* type, float dt){
	type->passBlock();
}

void DefenseSupportRubymanState::End(Rubyman* type){
	RubyScene* scene = dynamic_cast<RubyScene*>(type->GetScene());
	Team* team = scene->GetTeam(type->GetTeam());
	if (scene->GetTeam(type->GetTeam())->m_offdef.size() > 0 && scene->GetTeam(type->GetTeam())->m_supdef.size() > 0) {
		team->m_offdef.clear();
		team->m_supdef.clear();
	}
	type->GoToPosition(type->GetPosition().x, type->GetPosition().y, 0);
}

bool DefenseSupportRubymanState::CanTransitionFrom(Rubyman* type, int currentState){
	switch (Rubyman::State(currentState)) {
	case Rubyman::State::Attack:
		return false;
	case Rubyman::State::Invulnerable:
		return false;
	case Rubyman::State::FollowBall:
		return false;
	case Rubyman::State::Pass:
		return false;
	}
	return true;
}






void DefenseOffenseRubymanState::Start(Rubyman* type)
{
}

void DefenseOffenseRubymanState::Update(Rubyman* type, float dt){
	RubyScene* scene = dynamic_cast<RubyScene*>(type->GetScene());

	if (type->GetDanger() < 3 && m_timer <= 0) {
		type->circleAroundBall();
	}

	else if (type->GetDanger() >= 3 && m_timer <= 0) {
		type->GoToPosition(scene->Getball()->GetPosition().x, scene->Getball()->GetPosition().y, type->m_speed);
		m_timer = m_cooldown;
	}
	m_timer -= dt;
}

void DefenseOffenseRubymanState::End(Rubyman* type){
	m_timer = 0;
	RubyScene* scene = dynamic_cast<RubyScene*>(type->GetScene());
	Team* team = scene->GetTeam(type->GetTeam());
	if (scene->GetTeam(type->GetTeam())->m_offdef.size() > 0 && scene->GetTeam(type->GetTeam())->m_supdef.size() > 0) {
		team->m_offdef.clear();
		team->m_supdef.clear();
	}
	type->GoToPosition(type->GetPosition().x, type->GetPosition().y, 0);
}

bool DefenseOffenseRubymanState::CanTransitionFrom(Rubyman* type, int currentState){
	switch (Rubyman::State(currentState)) {
	case Rubyman::State::Attack:
		return false;
	case Rubyman::State::Invulnerable:
		return false;
	case Rubyman::State::FollowBall:
		return false;
	case Rubyman::State::Pass:
		return false;
	}
	return true;
}








void PassRubymanState::Start(Rubyman* type){

}

void PassRubymanState::Update(Rubyman* type, float dt){
	
	if (type->m_ball != nullptr) {

		std::map<Rubyman*, float> passTable;   //les coequipiers / leur distance avec le joueur ayant la balle.
		RubyScene* scene = dynamic_cast<RubyScene*>(type->GetScene());
		Team* team = type->GetScene<RubyScene>()->GetTeam(type->GetTeam());
		Team* ennemyTeam = type->GetScene<RubyScene>()->GetEnnemyTeam(type->GetTeam());

		for (Rubyman* r : team->m_team) {
			sf::Vector2f vector(r->GetPosition().x - type->GetPosition().x, r->GetPosition().y - type->GetPosition().y);
			float norme = Utils::GetDistance(r->GetPosition().x, r->GetPosition().y, type->GetPosition().x, type->GetPosition().y);
			
			bool CanPass = true;
			int factor = 0;

			if (type->GetTeam() == 1) {
				if (r->GetPosition().x > type->GetPosition().x) {
					factor = -1;
					CanPass = false;
				}
			}
			else if(type->GetTeam() == 2){
				if (r->GetPosition().x < type->GetPosition().x) {
					factor = -1;
					CanPass = false;
				}
			}
			

			if (CanPass) {
				for (auto e : ennemyTeam->m_team) {

					sf::Vector2f ennemyCoor(e->GetPosition().x - type->GetPosition().x, e->GetPosition().y - type->GetPosition().y);
					sf::Vector2f direction = vector / norme;


					//On regarde si l'ennemies est plac� de mani�re orthogonal au vecteur de lanc� de la balle.  
					float scalaire = direction.x * ennemyCoor.x + direction.y * ennemyCoor.y;
					if (scalaire < 0 || scalaire > norme) {
						continue;
					}

					sf::Vector2f ennemyClosestPoint = type->GetPosition() + direction * scalaire;

					float ennemydistance = Utils::GetDistance(e->GetPosition().x, e->GetPosition().y, ennemyClosestPoint.x, ennemyClosestPoint.y);




					float allowDistance = type->m_ball->GetRadius() + e->GetRadius() + (Utils::GetDistance(type->GetPosition().x, type->GetPosition().y, ennemyClosestPoint.x, ennemyClosestPoint.y)) * 0.4;

					if (ennemydistance < allowDistance) {
						CanPass = false;
						factor = -2;
						if (DrawDebug == true) {
							Debug::Get()->DrawLine(e->GetPosition().x, e->GetPosition().y, ennemyClosestPoint.x, ennemyClosestPoint.y, sf::Color::Blue);
						}

					}
					else {
						if (DrawDebug) {
							Debug::Get()->DrawLine(e->GetPosition().x, e->GetPosition().y, ennemyClosestPoint.x, ennemyClosestPoint.y, sf::Color::Green);
						}
					}

				}

			}

			
			if (CanPass) {
				factor = norme;
			}
			
			passTable[r] = factor;	
		}

		std::map<Rubyman*, float> passable;


		//on crée une map contenant les joueurs et après un nombre indiquant si il faut leur passer la balle ou pas. On enleve ceux dont la passe est impossible, et on créer un facteur qui depend de leur proximité avec le joueur, les ennemies et le centre du terrain.
		for (auto& i : passTable) {
			if (i.second <= 0) {
				continue;
			}
			else{
				float s1 = abs((scene->GetWindowHeight() / 2) - i.first->GetPosition().y)*2;
				float s2 = (i.second);
				float s3 = (i.first->m_danger) * 100;
				float score = s1 + s2 + s3;

				if (DrawDebug == true) {
					Debug::Get()->DrawText(i.first->GetPosition().x - 15, i.first->GetPosition().y + 10, std::to_string(score), sf::Color::Yellow);
				}

				passable[i.first] = score;
			}
		}

		

		Rubyman* bestR = nullptr;
		float best = 100000;

		//et maintenant on fait cherche celui qui a le score le plus bas
		for (auto i : passable) {
			if (i.second < best) {
				bestR = i.first;
				best = i.second;
			}
		}


		if (PassDebug == false) {
			if (passable.size() > 0) {

				type->Pass(bestR);

				scene->GetTeam(type->GetTeam())->Pass(true);
			}

			type->m_stateMachine.TryTransitionTo(type, (int)Rubyman::State::Attack);
		}


		if(DrawDebug == true)
		{//Ligne de debug

			std::map<Rubyman*, float>::iterator it;
			for (it = passTable.begin(); it != passTable.end(); it++) {
				if (it->second == -1) {
					Debug::Get()->DrawLine(it->first->GetPosition().x, it->first->GetPosition().y, type->GetPosition().x, type->GetPosition().y, sf::Color::Magenta);
				}
				else if(it->second == -2){
					Debug::Get()->DrawLine(it->first->GetPosition().x, it->first->GetPosition().y, type->GetPosition().x, type->GetPosition().y, sf::Color::Blue);
				}
				else {
					int green = 255 * exp(-0.0015 * it->second);
					sf::Color colorPass(255, green, 0);  //Fait varier la couleur entre le jaune et le rouge, en fonction de la distance
					Debug::Get()->DrawLine(it->first->GetPosition().x, it->first->GetPosition().y, type->GetPosition().x, type->GetPosition().y, colorPass);
				}
			}
		}
	}
}

void PassRubymanState::End(Rubyman* type){
	//type->GoToPosition(type->GetPosition().x, type->GetPosition().y, 0);
}

bool PassRubymanState::CanTransitionFrom(Rubyman* type, int currentState){
	if (type->m_ball == nullptr) {
		return false;
	}
	switch (currentState) {
	case (int)Rubyman::State::Defense:
		return false;
	case (int)Rubyman::State::Pass:
		return false;
	case (int)Rubyman::State::Invulnerable:
		return false;
	}
	
	return true;
}





void InvulnerableRubymanState::Start(Rubyman* type) {
	m_timer = 1.5f;
}

void InvulnerableRubymanState::Update(Rubyman* type, float dt) {
	AttackingRubymanState::Update(type, dt);

	m_timer -= dt;
	if (m_timer <= 0.f) {
		m_timer = 0.f;
		type->m_stateMachine.TryTransitionTo(type, (int)Rubyman::State::Attack);
	}

	if (DrawDebug == true) {
		Debug::Get()->DrawText(type->GetPosition().x - 15, type->GetPosition().y + 10, std::to_string(m_timer), sf::Color::Magenta);
	}
}

void InvulnerableRubymanState::End(Rubyman* type) {
	m_timer = 0.f;
}

bool InvulnerableRubymanState::CanTransitionFrom(Rubyman* type, int currentState){
	return true;
}




bool TeamHasBallCondition::Test(Rubyman* type) {
	RubyScene* scene = dynamic_cast<RubyScene*>(type->GetScene());
	if (scene->GetTeam(type->GetTeam())->HasBall() == false || type->m_ball == nullptr) {
		return false;
	}
	return true;
}

bool NobodyHasBallCondition::Test(Rubyman* type) {
	RubyScene* scene = dynamic_cast<RubyScene*>(type->GetScene());
	if (scene == nullptr) {
		return false;
	}
	int team = scene->WhoHasBall();
	if (team != 0) {
		return false;
	}
	return true;
}

bool EnnemyHasBallCondition::Test(Rubyman* type)
{
	RubyScene* scene = dynamic_cast<RubyScene*>(type->GetScene());
	if (scene == nullptr) {
		return false;
	}

	int team = scene->WhoHasBall();

	if (team == type->GetTeam() || team == 0) {
		return false;
	}
	return true;
}

bool SupportCondition::Test(Rubyman* type){
	RubyScene* scene = dynamic_cast<RubyScene*>(type->GetScene());
	if (scene->GetTeam(type->GetTeam())->HasBall() == false || type->m_ball != nullptr) {
		return false;
	}
	return true;
}

bool PassCondition::Test(Rubyman* type) {
	RubyScene* scene = dynamic_cast<RubyScene*>(type->GetScene());
	Team* ennemyTeam = scene->GetEnnemyTeam(type->GetTeam());
	int index = 0;
	if (scene->GetTeam(type->GetTeam())->HasBall() == false && type->m_ball != nullptr) {
		return false;
	}
	else if (type->GetDanger() <= 2 ) {
		for (int i = 0; i < ennemyTeam->m_team.size(); i++) {
			if (Utils::GetDistance(type->GetPosition().x, type->GetPosition().y,ennemyTeam->m_team[i]->GetPosition().x, ennemyTeam->m_team[i]->GetPosition().y) > ennemyTeam->m_team[i]->m_speed) {
				index++;
			}
		}
	}
	if (index >= 4) {
		return true;
	}
	return false;
}
