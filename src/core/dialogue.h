#ifndef DIALOGUEH
#define DIALOGUEH
#include "raylib.h"
#include <stdbool.h>

typedef struct {
	int count,current;
	bool active;
	char **lines,currentName[32],currentText[256];
	Texture2D currentPortrait;
	float texttimer; int lettercount;
} DialogueSystem;

typedef struct {
	int tilex,tiley;
	char txtpath[128];
	bool alreadyseen,hasds;
	DialogueSystem ds;
} DialogueBlock;

typedef struct {
	DialogueBlock *blocks;
	int count;
}DialogueMap;

DialogueSystem LoadDialogue(const char *filename);
void UpdateDialogue(DialogueSystem *ds);
void DrawDialogue(DialogueSystem *ds);
void UnloadDialogue(DialogueSystem *ds);
void ParseDialogueLine(DialogueSystem *ds);
DialogueMap LoadDialogueMap(const char *levelfolder);
void FreeDialogueMap(DialogueMap *dm);
void CheckAndStartDialogue(const Vector2 *playerpos,
				int tilesize,
				DialogueMap *dm,
				DialogueBlock **activeBlock);
void UpdateAndDrawActiveDialogue(DialogueBlock *activeBlock);
#endif
