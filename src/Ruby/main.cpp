#include "GameManager.h"
#include <SFML/Graphics.hpp>
#include <iostream>

#include "RubyScene.h"

#include <cstdlib>
#include <crtdbg.h>

int main(){
	srand(time(NULL));

	GameManager* pInstance = GameManager::Get();

	pInstance->CreateWindow(1280, 720, "Ruby game", 60, sf::Color(7, 136, 25));

	pInstance->LaunchScene<RubyScene>();

	return 0;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////																		
// 
//		les attaquants doivent etre capable de se demarquer, qu'il soient plus mobile quand ils n'ont plus la balle
// 
// Probleme d'invulnerabilité, quand un joueur entre en contact avec un autre, y a les deux situation de passage de balle qui se font je crois
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

