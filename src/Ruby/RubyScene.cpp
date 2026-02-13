#include <iostream>
#include <fstream>
#include <sstream>

#include "RubyScene.h"

#include "Zone.h"
#include "Rubyman.h"
#include "Rubyball.h"
#include "Rectangle.h"




void RubyScene::CreateRubyman(int team, int x, int y, float speed, float force){
	

	Rubyman* rubyman = nullptr;
	switch (team) {
	case 1:
		rubyman = CreateEntity<Rubyman>(30, sf::Color(223, 147, 233));
		m_firstTeam->AddRubyman(rubyman);

		//On pourrai set les statistiques ici
		break;
	
	case 2:
		rubyman = CreateEntity<Rubyman>(30, sf::Color::Black);
		m_secondTeam->AddRubyman(rubyman);

		break;
	default:
		std::cout << "equipe numero " << team << " inexistante" << std::endl;
		return;
	}

	rubyman->SetTag(3);
	rubyman->SetTeam(team);
	rubyman->SetPosition(x, y);
	rubyman->SetInitialPosition();
	rubyman->SetRigidBody(true);
	rubyman->SetGoalZone(GetGoalZone(team));
	
	rubyman->m_baseSpeed = speed;
	rubyman->m_boostedSpeed = 2 * speed;
	rubyman->m_speed = speed;

	rubyman->m_force = force;
	

	
	sf::Vector2f size{ 100, 100 };
	sf::Vector2f pos;
	if (rubyman->m_team == 1) {
		pos = sf::Vector2f(rubyman->GetPosition().x, rubyman->GetPosition().y - size.y/2);
	}
	else if (rubyman->m_team == 2) {
		pos = sf::Vector2f((rubyman->GetPosition().x - size.x), rubyman->GetPosition().y - (size.y/2));
	}

	rubyman->m_detectionRectangle = new Rectangle(pos, size.x, size.y, rubyman);
	
	


}

void RubyScene::CreateBall(){
	m_ball = CreateEntity<Rubyball>(15, sf::Color(255, 122, 0, 255));
	m_ball->SetPosition(GetWindowWidth() / 2.f, GetWindowHeight() / 2.f);
}

void RubyScene::CreateZones(int team, int x, int y){
	Zone* zone = CreateEntity<Zone>(1000, sf::Color(255, 255, 255, 100));
	zone->SetPosition(x, y);
	zone->SetTeam(team);
	switch (team) {
	case 1:
		m_zoneTeam1 = zone;
		break;
	case 2:
		m_zoneTeam2 = zone;
		break;
	default:
		std::cout << "equipe numero " << team << " inexistante" << std::endl;
		return;
	}
}

void RubyScene::OnInitialize(){

	m_firstTeam = new Team(1, this);
	m_secondTeam = new Team(2, this);


	CreateZones(1, -850, GetWindowHeight() / 2.f);
	CreateZones(2, GetWindowWidth()+850, GetWindowHeight() / 2.f);


	const char* path = "../../src/Ruby/SaumonEngine.jpeg";
	std::ifstream file("../../../src/Ruby/Team.txt");
	std::string line;

	while (std::getline(file, line)) {
		if (line == "#T1") {
			while (std::getline(file, line) && !line.empty() && line[0] != '#') {
				std::istringstream ligne(line);
				int x;
				int y;
				int force;
				int speed;


				ligne >> x >> y >> force >> speed;
				CreateRubyman(1, GetWindowWidth() * 0.25 + x, GetWindowHeight() / 2.f + y, speed, force);

			}
		}
		if (line == "#T2") {
			while (std::getline(file, line) && !line.empty() && line[0] != '#') {
				std::istringstream ligne(line);
				int x;
				int y;
				int force;
				int speed;


				ligne >> x >> y >> force >> speed;
				CreateRubyman(2, GetWindowWidth() * 0.75 + x, GetWindowHeight() / 2.f + y, speed, force);
			}
		}
	}
	CreateBall();
}

void RubyScene::OnEvent(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::S) {
			mode += 1;
			mode = mode % 2;
		}
		else if (event.key.code == sf::Keyboard::D) {
			DrawDebug = !DrawDebug;
		}
		else if (event.key.code == sf::Keyboard::P) {
			PassDebug = !PassDebug;
		}
	
	}

	if (event.type != sf::Event::EventType::MouseButtonPressed)
		return;

	if (mode == 0) {
		if (event.mouseButton.button == sf::Mouse::Button::Left)
		{
			m_ball->GoToPosition(event.mouseButton.x, event.mouseButton.y, 100.f);
		}
		if (event.mouseButton.button == sf::Mouse::Button::Right)
		{
			Rubyman* ru = nullptr;
			for (Rubyman* r : m_firstTeam->m_team) {
				if (r->IsInside(event.mouseButton.x, event.mouseButton.y)) {
					ru = r;
					break;
				}
			}
			for (Rubyman* r : m_secondTeam->m_team) {
				if (r->IsInside(event.mouseButton.x, event.mouseButton.y)) {
					ru = r;
					break;
				}
			}
			if (ru != nullptr) {
				if (m_teamHasBall == 1) {
					m_firstTeam->m_hasBall->Pass(ru);
				}
				if (m_teamHasBall == 2) {
					m_secondTeam->m_hasBall->Pass(ru);
				}
			}

		}
	}


	else if (mode == 1) {
		if (event.mouseButton.button == sf::Mouse::Button::Left) {
			for (Rubyman* r : m_firstTeam->m_team) {
				if (r->IsInside(event.mouseButton.x, event.mouseButton.y)) {
					selected = r;
					break;
				}
			}
			for (Rubyman* r : m_secondTeam->m_team) {
				if (r->IsInside(event.mouseButton.x, event.mouseButton.y)) {
					selected = r;
					break;
				}
			}
		}
		else if ((event.mouseButton.button == sf::Mouse::Button::Right)) {
			if (selected != nullptr) {
				selected->GoToPosition(event.mouseButton.x, event.mouseButton.y, 200);
			}
		}
	}
}

void RubyScene::OnUpdate(){
	std::string score = "Equipe 1 (rose) : " + std::to_string(m_firstTeam->GetScore()) + "  || Equipe 2 (noir) : " + std::to_string(m_secondTeam->GetScore());
	Debug::Get()->DrawText(GetWindowWidth() / 2 - 280, 10, score, sf::Color::Black);

	if (m_zoneTeam1->IsColliding(m_ball)) {
		if (m_ball->m_holder) {
			if (m_zoneTeam1->GetTeam() != m_ball->m_holder->GetTeam()) {
				m_secondTeam->IncreaseScore(1);
				std::cout << "Score Team 1 : " << m_firstTeam->GetScore() << " - Score Team 2 : " << m_secondTeam->GetScore() << std::endl;
				Score(2);
				return;
			}
		}
		
		
	}
	else if (m_zoneTeam2->IsColliding(m_ball)) {
		if (m_ball->m_holder) {
			if (m_zoneTeam2->GetTeam() != m_ball->m_holder->GetTeam()) {
				m_firstTeam->IncreaseScore(1);
				std::cout << "Score Team 1 : " << m_firstTeam->GetScore() << " - Score Team 2 : " << m_secondTeam->GetScore() << std::endl;
				Score(1);
				return;
			}
		}
	}


	m_firstTeam->Update();
	m_secondTeam->Update();

	m_teamHasBall = 0;
	if (m_firstTeam->HasBall()) {
		m_teamHasBall = 1;
	}
	else if (m_secondTeam->HasBall()) {
		m_teamHasBall = 2;
	}

}

void RubyScene::Score(int team) {



	if (team == 1) {
		int random = rand()% m_secondTeam->m_team.size();

		m_ball->m_holder->m_ball = nullptr;
		m_secondTeam->m_team[random]->m_ball = m_ball;
		m_ball->m_holder = m_secondTeam->m_team[random];
	}
	else if (team == 2) {
		int random = rand() % m_firstTeam->m_team.size();

		m_ball->m_holder->m_ball = nullptr;
		m_firstTeam->m_team[random]->m_ball = m_ball;
		m_ball->m_holder = m_firstTeam->m_team[random];
	}
	
	for (int i = 0; i < m_firstTeam->m_team.size(); i++) {
		m_firstTeam->m_team[i]->SetPosition(m_firstTeam->m_team[i]->m_initialPosition.x, m_firstTeam->m_team[i]->m_initialPosition.y);
		m_firstTeam->m_team[i]->GoToPosition(m_firstTeam->m_team[i]->GetPosition().x, m_firstTeam->m_team[i]->GetPosition().x, 0);
	}	
	
	for (int i = 0; i < m_secondTeam->m_team.size(); i++) {
		m_secondTeam->m_team[i]->SetPosition(m_secondTeam->m_team[i]->m_initialPosition.x, m_secondTeam->m_team[i]->m_initialPosition.y);
		m_secondTeam->m_team[i]->GoToPosition(m_secondTeam->m_team[i]->GetPosition().x, m_secondTeam->m_team[i]->GetPosition().x, 0);
	}
}

Team* RubyScene::GetTeam(int t){
	if (t == 1) {
		return m_firstTeam;
	}
	if (t == 2) {
		return m_secondTeam;
	}
	return nullptr;
}

Team* RubyScene::GetEnnemyTeam(int t)
{
	if (t == 1) {
		return m_secondTeam;
	}
	else if (t == 2) {
		return m_firstTeam;
	}
	return nullptr;
}

Zone* RubyScene::GetGoalZone(int i) {
	if (i == 1) {
		return m_zoneTeam2;
	}
	if (i == 2) {
		return m_zoneTeam1;
	}
	return nullptr;
}

RubyScene::~RubyScene() {
	delete m_firstTeam;
	delete m_secondTeam;
}

