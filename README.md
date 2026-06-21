# TextHere (initial game project name)

Raylib-based game. initially planning to be something like Hotline Miami and there is npc but no shooting or basic
mechanic yet so it's still far from finished

### requirements

- raylib

- gcc

- make tool (for linux)

### setup by OS

**windows**

- Get Raylib via Website. make sure the raylib's bin and libs are in ``C:/Raylib``

- Get gcc via scoop

```
winget install scoop
scoop install gcc
```

this method isnt actually tested alot so dunno if it'll work

**Linux (Debian and etc)**

```
sudo apt-get install gcc make libraylib-dev
```

**macos**

```
brew install raylib
```

#build

- if you're on windows, do ``./build.bat``. unless you edit the bat file, there isnt much option there

- if you're on linux, do ``make``. unlike build.bat, theres two options which is ``all`` or ``clear``

## features
- top down character
- type out map name at the start
- certain coordinates start a dialogue
- a dialogue can control camera movement or "Scenes" outside of basic dialogue stuff
- prolly easy to make maps with probably clear documentation

## any questions?

if you're looking for how to mechanic of the game works or etc, please check out the [docs/](docs/) directory, especially [here](docs/wiki.md) for a complete beginner guide to just play the game. you may also contact me if some question doesnt raise an answer to you!

## License
GPL v3
see [LICENSE](LICENSE)

## credits

- **A* pathfinding algorithm**:based on [Shardul Hirolikar's implementation](https://github.com/HimanshuK704/A-Star-Pathfinding-Algorithm-in-C) (MIT License)
- **Game Engine**: [Raylib](https://github.com/raysan5/raylib) by Ramon Santamaria
