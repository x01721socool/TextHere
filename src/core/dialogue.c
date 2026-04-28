#define _DEFAULT_SOURCE
#include "dialogue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
DialogueSystem LoadDialogue(const char *filename,Vector2 plypos,Camera2D *cam, const char *levelfolder){
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
	if(ds.count>0)ParseDialogueLine(&ds,cam,plypos,levelfolder);
	return ds;
}

void UpdateDialogue(DialogueSystem *ds,Vector2 plypos, Camera2D *cam,const char *levelfolder){
	if(!ds->active)return;
	float speed=0.01f;ds->texttimer+=GetFrameTime();
	if(ds->texttimer>=speed){
		if (ds->lettercount<(int)(strlen(ds->currentText))){
			ds->lettercount++;}
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
				ParseDialogueLine(ds,cam,plypos,levelfolder);
			}
		}
	}
}
void ParseDialogueLine(DialogueSystem *ds,Camera2D *cam, Vector2 plypos,const char *levelfolder){
	if(ds->current >= ds->count)return;
	char linecpy[256];
	strncpy(linecpy,ds->lines[ds->current], sizeof(linecpy));
	char *name =strtok(linecpy, ":");
	if (!name) return;
	if (strcmp(name,"CAMERA")==0){
		char *option = strtok(NULL,":");
		if (strcmp(option,"CENTERPLAYER")==0){
			cam->target=plypos;
		}
		else if (strcmp(option,"TPTOPOS")==0){
			int x,y;
			char *cords=strtok(NULL,":");
			if (sscanf(cords,"%d,%d",&x,&y)<2){
				TraceLog(LOG_WARNING,"faulty cam cords"
						"!: %s",linecpy);
				cam->target=plypos;
			}
			cam->target=(Vector2){(float)(x*64),(float)(y*64)};
		} else {
				TraceLog(LOG_WARNING,"BAD camera option!: %s",
						linecpy);
		}
		ds->current++;
		ParseDialogueLine(ds,cam,plypos,levelfolder);
		return;
	} else if (strcmp(name,"SCENE")==0){
		char *option = strtok(NULL,":");
		if (strcmp(option,"NONE")!=0){
			char path[256];
			snprintf(path,sizeof(path),"%s/scenes/%s.png",levelfolder,option);
			if (ds->currentScene.id!=0) UnloadTexture(ds->currentScene);
			ds->currentScene=LoadTexture(path);
		}
	} else{
	strcpy(ds->currentName,name);
	char *emotion=strtok(NULL,":");
	char *text=strtok(NULL,":");

	if (emotion) {
		char path[256];
		snprintf(path,sizeof(path),"assets/sprite/"
			"dialogue/%s/%s.png",name,emotion);
		if (ds->currentPortrait.id!=0)
			UnloadTexture(ds->currentPortrait);
		ds->currentPortrait=LoadTexture(path);
	}
	if (text){
		strncpy(ds->currentText,text,
				sizeof(ds->currentText)-1);
	} else {
		strncpy(ds->currentText,"...",sizeof("...")-1);
	}
	ds->lettercount=0;ds->texttimer=0.0f;
}}

void DrawDialogue(DialogueSystem *ds){
	if(!ds->active)return;
	if (ds->currentScene.id>0){
		DrawTextureEx(ds->currentScene,
				(Vector2){0,0},
				0,
				1.0f,
				WHITE);
	}
	if (ds->currentPortrait.id>0) {
		DrawTextureEx(ds->currentPortrait,
				(Vector2){GetScreenWidth()-256,GetScreenHeight()*3/4-320},
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
	if (ds->lines==NULL) return;
	for(int i=0;i<ds->count;i++){
		if (ds->lines[i]!=0){
			free(ds->lines[i]);
			ds->lines[i]=NULL;
		}
	}
	free(ds->lines);
	ds->lines=NULL;
	ds->count=0;
	if (ds->currentScene.id>0){
		UnloadTexture(ds->currentPortrait);
		ds->currentPortrait.id=0;
	}
	if (ds->currentPortrait.id>0){
		UnloadTexture(ds->currentPortrait);
		ds->currentPortrait.id=0;
	}
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
				DialogueBlock **activeblock,
				Camera2D *cam,
				const char *levelfolder){
	if (!dm||dm->count==0)return;
	if (*activeblock) return;
	int ptilex=(int)(playerpos->x/tilesize);
	int ptiley=(int)(playerpos->y/tilesize);
	for (int i=0;i<dm->count;++i){
		DialogueBlock *b = &dm->blocks[i];
		if (b->alreadyseen) continue;
		if (b->tilex==ptilex&&b->tiley==ptiley){
			b->ds=LoadDialogue(b->txtpath,
					(Vector2){playerpos->x,playerpos->y},
					cam,levelfolder);
			b->hasds=true;
			b->alreadyseen=true;
			*activeblock=b;
			break;
		}
	}
}
void UpdateAndDrawActiveDialogue(DialogueBlock *activeblock,Vector2 plypos,
		Camera2D *cam,const char *levelfolder){
	if (!activeblock) return;
	UpdateDialogue(&activeblock->ds,plypos,cam,levelfolder);
	DrawDialogue(&activeblock->ds);
	if (!activeblock->ds.active){
		activeblock->hasds=false;
	}
}
