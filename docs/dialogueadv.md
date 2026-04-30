# Anatomy of dialogues and writing them (advanced)

this .md is for writing dialogues inside a ''x<xpos>y<ypos>.txt'' file. this .md will also ignore on guiding about the basic dialogue
mechanic, since that is explained in ''docs/guide2map.md''.

as of right now there are two options you could do outside of basic dialogue:

- SCENE

- CAMERA

## CAMERA

CAMERA handles the camera position as the player is going through a dialogue process. a use of this is that you could
direct the camera to be at a different position as the dialogue processes instead of locking onto the player

CAMERA has two different modes: ''CENTERPLAYER'' or ''GOTOPOS''

### CENTERPLAYER

CENTERPLAYER is the simplest option a CAMERA has. it does.. you guessed it - Center the camera to Player

initiating this command can simply be done in this format
```
CAMERA:CENTERPLAYER
```

no arguements are needed in this line

**NOTE**:you do not need to use CENTERPLAYER before ending the dialogue, as it will do that in any way

### GOTOPOS

GOTOPOS teleports the camera to be in a certain position. as of right now it doesnt have an animation to do so so it will
just snap into them

initiating this command can simply be done in this format
```
CAMERA:GOTOPOS:<xpos>,<ypos>
```
two arguements are needed in this line as numbers: ''<xpos>'' and ''<ypos>''

**NOTE 1**:it is possible to do GOTOPOS on a coordinate outside of the map boundary
**NOTE 2**:GOTOPOS does **NOT** lock the camera to the position after the dialogue ended, as it will just snap
back to the player

## SCENE

a scene is typically a 800x600 in px png that appears infront of the gameplay, and behind the dialogue and the Portrait. this can
be used to when there are certain moments in a dialogue that can't be represented in a simple top-down gameplay, but instead
a picture.

the command can be initialized under this format
```
SCENE:<imagename>
```
only one arguement is needed in this: ''<imagename>''

''<imagename>'' is an arguement that defines the name of the image (excluding png) inside the directory ''/scenes'' inside the
desired map directory

when you want to use the command, you must create a directory named "scenes" inside the map directory before putting the desired
scene images inside. the images **MUST** be 800x600px in size, and a png.

**NOTE**:there is no clear option on clearing the image, but one way is to set the ''<imagename>'' arguement to be something that
doesn't exist in the ''/scenes'' directory. that way SCENE can't load a scene because theres no image inside with that
name
