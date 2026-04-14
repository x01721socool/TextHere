# Anatomy of Dialogue txts and how to write them

This .md file assumes you have created a ``map_dialogue.csv`` and
``x<xpos>y<ypos>.txt`` file.

As of right now, a Dialogue txt can control two things:

- Dialogue (suprise suprise)

- Camera Position

in every line of the txt it can either be those two. below defines its format and how to use them

## Dialogue

a line of a dialogue must be in this format

``` 
<Character>:<Emotion>:<what to say>
```

- Character

Character is the Character that will be displayed onto the screen and used for
``Emotion``. Characters  are located and must be made at ``assets/sprite/..``
as a directory

- Emotion

Inside a Character Directory lies a few images. these images names are whats
available and will be used as Emotion (ignoring .png). as of right now, they
MUST be made in width of 128 and height of 160. failure to comply may result in either Character image floating, or overlapping or off the screen. an Emotion
image MUST be png, since it will only look for png formats.

- what to say

self explanatory. can be as long as you want as long as it doesnt end in
a new line or include ":".

## Camera Position

to define a camera position, it must be inside this format:

```
CAMERA:<option>:<position (for GOTOPOS)>
```

- Option

As of right now there are two options that
can be written out: ``CENTERPLAYER`` or ``GOTOPOS``

Upon choosing ``CENTERPLAYER`` you will not have to write ``:<pos>`` after
this.

- position

only needed if you chose ``GOTOPOS`` in ``<Option>``. must be in the form 
``<xpos>,<ypos>``


