#ifndef DIALOGUEH
#define DIALOGUEH
#include "raylib.h"
#include <stdbool.h>

typedef struct {
	int count,current;
	bool active;
	char **lines,currentName[32],currentText[256];
	Texture2D currentPortrait;
	Texture2D currentScene;
	float texttimer; int lettercount;
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

DialogueSystem LoadDialogue(const char *filename,Vector2 plypos,Camera2D *cam,const char *levelfolder);
void UpdateDialogue(DialogueSystem *ds,Vector2 plypos,Camera2D *cam,const char *levelfolder);
void DrawDialogue(DialogueSystem *ds);
void UnloadDialogue(DialogueSystem *ds);
void ParseDialogueLine(DialogueSystem *ds, Camera2D *cam,Vector2 plypos,const char *levelfolder);
DialogueMap LoadDialogueMap(const char *levelfolder);
void FreeDialogueMap(DialogueMap *dm);
void CheckAndStartDialogue(const Vector2 *playerpos,
				int tilesize,
				DialogueMap *dm,
				DialogueBlock **activeBlock,
				Camera2D *cam,
				const char *levelfolder);
void UpdateAndDrawActiveDialogue(DialogueBlock *activeBlock,Vector2 plypos,
		Camera2D *cam,const char *levelfolder);
#endif
