# AI (c++ - LightEngine(SFML))

A Rugby simulation made in 2 weeks, developped with a homemade state machine and realistic behaviour

![Language](https://img.shields.io/badge/language-C%2B%2B-blue)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey)
![Status](https://img.shields.io/badge/status-Completed-green)


## Features 
- Different behaviours depending on the environnement 
- Customizable teams (in team.txt)



## Controls 
- P -> Pause
- D -> Draw debug mode
- S -> Change control mode:
    - First mode:
        - Left click -> Move ball
        - Right click -> Force pass to a player
    - Second mode:
      - Left click -> Select a player
      - Right click -> Move selected player

## Screenshots

### State diagram 

![image info](pictures/StateDiagram.png)


### pass and dodge algorithm
![image info](pictures/Pass.gif)


### contact and dodge
![image info](pictures/Contact.gif)




## Installation

1. Clone the repository:
   git clone https://github.com/slucasss/RubyGame.git

3. Get the .sln for visual studio
```bash
  .\SolutionGenerator.exe -make ./
```

3. Open it in visual studio
                                    
4. Compile

5. Run
