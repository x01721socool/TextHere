# how to make your own map

## what a map file should look like:

```
cool-game/
|-...
|-maps/
    |-...
    |-yourmap/
        |-x<xcords>y<ycords>.txt #create more than one if you have more dialogues in map_dialogue.csv
        |-map_dialogue.csv
        |-map_walls.csv
        |-map_ground.csv
```

## setting up ``map_walls.csv`` and ``map_ground.csv``

as of right now, the game is hardcoded on 10x10. so these should be 10x10.
``map_ground.csv`` as of right now is practically useless, so it is optional to add one.
``map_walls.csv`` has two states of existence of each coordinates and it looked
something like this:
```
-1,1,1,...,-1
1,1,1,...,1
...
-1,1,1,...,1
```
``-1`` indicates that a wall shouldn't be made here, whilst vice versa for ``1``.

as of right now, player's spawn position are hardcoded to be at 0,0

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

### the anatomy of dialogue txt and how to write them

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
