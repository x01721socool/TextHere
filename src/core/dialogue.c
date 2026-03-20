#include "dialogue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
DialogueSystem LoadDialogue(const char *filename){
	DialogueSystem ds={.lines =NULL,
	       		.count=0, .lettercount=0,
			.current=0, .texttimer=0.0f,
			.active=true};
	ds.currentPortrait.id=0;
	FILE *file=fopen(filename,"r");
	if (!file)return ds;
	char buffer[256];
	while(fgets(buffer,sizeof(buffer), file)){
		ds.count++;
		ds.lines=realloc(ds.lines,ds.count * sizeof(char *));
		buffer[strcspn(buffer,"\n")]=0;
		ds.lines[ds.count -1]=strdup(buffer);
	}
	fclose(file);
	if(ds.count>0)ParseDialogueLine(&ds);
	return ds;
}

void UpdateDialogue(DialogueSystem *ds){
	if(!ds->active)return;
	float speed=0.01f;ds->texttimer+=GetFrameTime();
	if(ds->texttimer>=speed){
		if (ds->lettercount<strlen(ds->currentText)){ds->lettercount++;}
		ds->texttimer=0.0f;
	}
	if(IsKeyPressed(KEY_ENTER)){
		if(ds->lettercount<(int)strlen(ds->currentText)){
			ds->lettercount=(int)strlen(ds->currentText);
		}else{
			ds->current++;
			if(ds->current >=ds->count){
				ds->active=false;
			}else{
				ParseDialogueLine(ds);
			}
		}
	}
}
void ParseDialogueLine(DialogueSystem *ds){
	if(ds->current >= ds->count)return;
	char *fullline=strdup(ds->lines[ds->current]);
	char *name =strtok(fullline, ":");
	if (name) strcpy(ds->currentName,name);
	char *emotion=strtok(NULL,":");
	char *text=strtok(NULL,":");
	strcpy(ds->currentText,text!=NULL?text:ds->lines[ds->current]);
	
	if(name&&emotion){
		char path[128];
		sprintf(path, "assets/sprite/dialogue/%s/%s.png",name,emotion);
		if(ds->currentPortrait.id>0)UnloadTexture(ds->currentPortrait);
		ds->currentPortrait=LoadTexture(path);}
	free(fullline);
	ds->lettercount=0;ds->texttimer=0.0f;
}

void DrawDialogue(DialogueSystem *ds){
	if(!ds->active)return;
	DrawRectangle(0,
			0,
			GetScreenWidth(),
			GetScreenHeight(),
			Fade(GRAY,0.4f));
	if (ds->currentPortrait.id>0) {
		DrawTextureEx(ds->currentPortrait,
				(Vector2){GetScreenWidth()-128*2,GetScreenHeight()*3/4-160*2},
				0,
				2.0f,
				WHITE);
	}
	int boxY=GetScreenHeight()/4;
	DrawRectangle(0,
			boxY*3,
			GetScreenWidth(),
			GetScreenHeight(),
			WHITE);
	const char* visibletext=TextSubtext(ds->currentText,
						0,
						ds->lettercount);
	DrawText(visibletext,
		 10,
		 boxY*3+5,
		 20,
		 BLACK);
}
void UnloadDialogue(DialogueSystem *ds){
	for(int i=0;i<ds->count;i++)free(ds->lines[i]);
	free(ds->lines);
}
DialogueMap LoadDialogueMap(const char *levelfolder){
	DialogueMap dm={.blocks=NULL,.count=0};
	char csvpath[64];
	snprintf(csvpath,sizeof(csvpath),"%s/map_dialogue.csv",levelfolder);
	FILE *file = fopen(csvpath,"r");
	if (!file){
		TraceLog(LOG_WARNING,
				"level %s:cant find or level has no dialogue",
				csvpath);
		return dm;
	}
	char line[256];
	while (fgets(line,sizeof(line),file)){
		int x=-1,y=-1;
		char custom[128]="";
		int scanned=sscanf(line,
					"%d,%d,%127[^\n]",
					&x,
					&y,
					custom);
		if (scanned<2) continue;
		dm.count++;
		dm.blocks=realloc(dm.blocks,
					dm.count*sizeof(DialogueBlock));
		DialogueBlock *b=&dm.blocks[dm.count -1];
		b->tilex=x;
		b->tiley=y;
		b->alreadyseen=false;
		b->hasds=false;
		b->ds=(DialogueSystem){0};
		if (strlen(custom)>0)
			snprintf(b->txtpath,
					sizeof(b->txtpath),
					"%s/%s",
					levelfolder,
					custom);
		else
			snprintf(b->txtpath,
					sizeof(b->txtpath),
					"%s/x%dy%d.txt",
					levelfolder,
					x,
					y);
	}
	fclose(file);
	return dm;
}
void FreeDialogueMap(DialogueMap *dm){
	if (!dm) return;
	for (int i=0;i<dm->count;++i){
		if(dm->blocks[i].hasds){
			UnloadDialogue(&dm->blocks[i].ds);
		}
	}
	free(dm->blocks);
	dm->blocks=NULL;
	dm->count=0;
}
void CheckAndStartDialogue(const Vector2 *playerpos,
				int tilesize,
				DialogueMap *dm,
				DialogueBlock **activeblock){
	if (!dm||dm->count==0)return;
	if (*activeblock) return;
	int ptilex=(int)(playerpos->x/tilesize);
	int ptiley=(int)(playerpos->y/tilesize);
	for (int i=0;i<dm->count;++i){
		DialogueBlock *b = &dm->blocks[i];
		if (b->alreadyseen) continue;
		if (b->tilex==ptilex&&b->tiley==ptiley){
			b->ds=LoadDialogue(b->txtpath);
			b->hasds=true;
			b->alreadyseen=true;
			*activeblock=b;
			break;
		}
	}
}
void UpdateAndDrawActiveDialogue(DialogueBlock *activeblock){
	if (!activeblock) return;
	UpdateDialogue(&activeblock->ds);
	DrawDialogue(&activeblock->ds);
	if (!activeblock->ds.active){
		UnloadDialogue(&activeblock->ds);
		activeblock->hasds=false;
	}
}
