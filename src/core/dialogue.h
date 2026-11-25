#ifndef DIALOGUEH
#define DIALOGUEH
#include "raylib.h"
#include <stdbool.h>

typedef struct game game;
typedef struct {
  int count,current;
  bool active;
  char **lines,currentName[32],currentText[256];
  Texture2D currentPortrait;
  Texture2D currentScene;
  float texttimer; int lettercount;
  Sound talking;
} DialogueSystem;

typedef struct {
  int tilex,tiley;
  char txtpath[256];
  bool alreadyseen,hasds;
  DialogueSystem ds;
} DialogueBlock;

typedef struct {
  DialogueBlock *blocks;
  int count;
}DialogueMap;

DialogueSystem LoadDialogue(const char *filename,game *g,const char *levelfolder);
void UpdateDialogue(game *g,const char *levelfolder);
void DrawDialogue(DialogueSystem *ds);
void UnloadDialogue(DialogueSystem *ds);
void ParseDialogueLine(game *g,const char *levelfolder);
DialogueMap LoadDialogueMap(const char *levelfolder);
void FreeDialogueMap(DialogueMap *dm);
void CheckAndStartDialogue(
    game *g,
    const char *levelfolder);
void UpdateAndDrawActiveDialogue(game *g,const char *levelfolder);
#endif
