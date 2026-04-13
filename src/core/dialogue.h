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

DialogueSystem LoadDialogue(const char *filename,Vector2 plypos,Camera2D *cam);
void UpdateDialogue(DialogueSystem *ds,Vector2 plypos,Camera2D *cam);
void DrawDialogue(DialogueSystem *ds);
void UnloadDialogue(DialogueSystem *ds);
void ParseDialogueLine(DialogueSystem *ds, Camera2D *cam,Vector2 plypos);
DialogueMap LoadDialogueMap(const char *levelfolder);
void FreeDialogueMap(DialogueMap *dm);
void CheckAndStartDialogue(const Vector2 *playerpos,
				int tilesize,
				DialogueMap *dm,
				DialogueBlock **activeBlock,
				Camera2D *cam);
void UpdateAndDrawActiveDialogue(DialogueBlock *activeBlock,Vector2 plypos,
		Camera2D *cam);
#endif
