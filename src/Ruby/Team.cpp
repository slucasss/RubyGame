#include <iostream>

#include "Team.h"
#include "Rubyman.h"
#include "RubyScene.h"

Team::Team(int flag, RubyScene* scene) : m_hasBall(nullptr), m_score(0), m_flag(flag) {
	int nbrLine = 8;
	float lineWidth = scene->GetWindowWidth() / nbrLine;
	for (int i = 0; i < nbrLine; i++) {
		m_lineRepartition[i * lineWidth] = nullptr;
	}


}



void Team::AddRubyman(Rubyman* rubyman){
	m_team.push_back(rubyman);
}

void Team::Update() {
	for (Rubyman* r : m_team) {
		if (r->HasBall()) {
			m_hasBall = r;
			return;
		}
		m_hasBall = nullptr;
	}
	if (m_isPassing == true && m_hasBall != nullptr) {
		m_isPassing = false;
	}
}

void Team::ResetLineRepartition(){
	for (auto l : m_lineRepartition) {
		l.second = nullptr;
	}
}
