# how to make your own map

## what a map file should look like:

```
cool-game/
|-...
|-assets/
    |-maps/
        |-...
        |-yourmap/
            |-x<xcords>y<ycords>.txt #create more than one if you have more dialogues in map_dialogue.csv
            |-map_dialogue.csv
            |-map_walls.csv
            |-map_render.csv
            |-info.txt
```

## setting up ``map_walls.csv`` and ``map_render.csv``

you must first plan how big the map will be. an interger off may cause a disturbance on
the game. this information will later be used on ``info.txt``
you may notice that ``map_ground.csv`` is replaced to ``map_render.csv``. this is due to also
add render compactabilty for walls. configuring this will be mentioned later.

# ``map_walls.csv``

``map_walls.csv`` has two states of existence of each coordinates and it looked
something like this:
```
-1,1,1,...,-1
1,1,1,...,1
...
-1,1,1,...,1
```
``-1`` indicates that a wall shouldn't be made here, whilst vice versa for ``1``.

# ``map_render.csv``

look at the image ``assets/spritesheet.png``. pick a tile you would like to use and find its x and y
position ** in terms of grid, not pixels **. note:grid axises begin in 0. calculate their id by 
this equation

```ypos*(spritesheet width in terms of grid)+xpos```

go back to map_render.csv, and use that id in that coordinate. map_render MUST be have the same rows
and collumns as what you'll set the size of the map in info.txt

note that you can do all this in Tiled with the ``assets/sprite/fh.tsx`` as a spritesheet, though
documentation wont be given for this due to lack of experience with it.

## setting up the information about the map (``info.txt``)

the ``info.txt`` **stores the required information about the map**. a line must **have a key
and a value, separated by a colon**

```
#example
key1:value1
key2:valuex,valuey
key3:...
```

as of right now **each map has 3 keys that can be modified via** ``info.txt``

### available keys for ``info.txt``:

- ``ips`` :**Inital Player Speed**: the amount of speed a player have upon the initial state of the game.
value MUST be a FLOAT number

- ``sp`` :**Spawn Position**: a Vector2D value that determines the position inside tha map of where the 
player will load in. the value MUST be in the form of ``<x>,<y>``, where x and y are FLOATS ranging 
from 0 to the map limit corresponding to their axis.

- ``size`` :**Size of the map**:a two-interger value that defines the size of the map. the value MUST be in
the form of ``<w>,<h>`` where w and h are INTERGERS representing EXACTLY the size of the map corresponding
to their axis. an interger off to these values may lead a slight corruption inside the game.

## creating dialogues

### making a csv file for it

first, create a ``map_dialogue.csv`` on your map directory. inside should look something like this:
```
<xpos1>,<ypos1>
<xpos2>,<ypos2>
...
```
as it can be seen, each line defines a new  dialogue position. and each line has two properties it mus have: ``xpos`` and ``ypos``
example:
```
2,3 # <-- define the existence of a dialogue block at 2,3
0,0 # <-- define the existence of another dialogue block at 0,0
```
### creating txt's for each dialogues

once you created ``map_dialogue.csv``, you must create a dialogue txt for every position from ``map_dialogue.csv`` so that the game can read what dialogue to put at that position

naming the txt in a specific way is very crucial for the game to know which txts to read first-hand.
for every dialogue position, there must be a txt file named ``x<xpos>y<ypos>.txt``.
if inside ``map_dialogue.csv`` has the position ``1,5``, then the name of the txt must be
``x1y5.txt`` and so forth.

### writing basic dialogue

unfortunately for this .md i could only fit the basic dialogue guide since others are pretty long. to check out how to do stuffs like
''CAMERA'' or ''SCENE'', check out ''docs/dialogueadv.md''

inside every txt file reside lines of dialogue. every line indicates every dialogue turn. they must
look something like this
```
<character>:<emotion>:<what to say>
```
- ``<character>``: inside ``cool-game/assets/sprite`` resides a ``dialogue`` directory, in which has
lists of characters. so ``<character>`` is the **directory name inside
 the directory ``cool-game/assets/sprite`` of the character** you wanted to use.

-``<emotion>``: if you take a look inside one of the character's ``cool-game/assets/sprite/dialogue``
you will see images with exactly the size of 128x160. for every ``<character>`` ``<emotion>`` is
**the image name inside the ``cool-game/assets/sprite/dialogue/<character>`` that you wanted to use**.
they must be exactly **128 pixels in width** and **160 pixels in height** or else they may overlap or barely
touched the dialogue box

-``<what to say>``: self explanatory

failure to locate ``<character>`` or ``<emotion>`` won't bring a much problem since it will continue
starting the game, with the only exception that there wont be any image

Knowing this, you can create your own character for dialogues by following:

```
cool-game/
|-...
|-assets/
    |-maps/
    |-sprites/
        |-...
        |-dialoges/
            |-...
            |-yourcharacter
                |-<emotions>.png
```
