# Game

very tuff game idk

### requirements

- raylib

- gcc or clang

- make tool

### setup by OS

**windows**

-must be running from powershell (cmd is possible idk havent tried it out)

- get scoop to get gcc and raylib

```
winget install scoop
scoop install gcc raylib
```

**Linux (Debian and etc)**

```
bash
sudo apt-get install gcc make libraylib-dev
```

**macos**

```
bash
brew install raylib
```

### building

```
#build
make #or ./build.bat if ur on windows

#run the game
./build/game #add the .exe for windows

#or just use shortcuts as
make run #build and run in instant
make clean #clean build artifacts
```

## features
- top down character
- type out map name at the start
- certain cooords initiate a dialogue
- prolly easy to make maps

## any questions?

if you're looking for how to mechanic of the game works or etc, please check out the ``docs/`` directory. you may also contact me if some question doesnt raise an answer to you!

## License

see [LICENSE]
