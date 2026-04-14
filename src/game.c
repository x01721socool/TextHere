#include "raylib.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
/*mechanic headers*/
#include "core/player.h"
#include "core/grid.h"
#include "core/dialogue.h"
#define TS 64 /*short for tile size*/

//game.c becomes the motherboard
//of the game and connects all functions/modules
//alltogether

int main(void){
	InitWindow(800,600,"test");gamemap level;
	SetExitKey(KEY_NULL);
	bool levelselected=false;int lettercount=0;
	char premaplevel[33]={0};
	while(!levelselected&&!WindowShouldClose()){
		int key=GetCharPressed();
		while(key>0){
			if((key>=32&&key<=125)&&(lettercount<32))
			{
				premaplevel[lettercount]=(char)key;
				premaplevel[lettercount+1]='\0';
				lettercount++;
			}
			key=GetCharPressed();}
			if(IsKeyPressed(KEY_BACKSPACE)){
				lettercount--;
			        if(lettercount<0)lettercount=0;
				premaplevel[lettercount]='\0';
			}
			if (IsKeyPressed(KEY_ENTER)){
				TraceLog(LOG_INFO,"recieved string:%s",premaplevel);
				char buffer[64];
				snprintf(buffer,sizeof(buffer),"assets/maps/%s",premaplevel);
				level=loadmap(buffer);
				levelselected=true;
			}
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawText(premaplevel,180,280,40,BLACK);
		EndDrawing();
	}
	char levelfolder[64];
	snprintf(levelfolder,sizeof(levelfolder),"assets/maps/%s",premaplevel);
	DialogueMap dlgmap=LoadDialogueMap(levelfolder);
	Player player={.pos={level.sp.x*TS+32,level.sp.y*TS+32},.speed=level.ips,.size=24.0f};
	SetTargetFPS(64);
	Camera2D camera={0};
	camera.zoom=1.0f;camera.rotation=0.0f;
	DialogueBlock *activedialogueblock=NULL;
	while (!WindowShouldClose()) {
		if (activedialogueblock==NULL){
			Pupdate(&player,TS,level);
			camera.target=player.pos;
		}
		CheckAndStartDialogue(&player.pos,TS,&dlgmap,
				&activedialogueblock,
				&camera);
		camera.offset=(Vector2){400.0f,300.0f};
		if (IsKeyPressed(KEY_ESCAPE)){
			levelselected=false;
		}
		BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode2D(camera);
				drawmap(level, TS, camera);
				Pdraw(&player);
			EndMode2D();
			UpdateAndDrawActiveDialogue(activedialogueblock,
					player.pos,
					&camera);
			if (activedialogueblock
				&&!activedialogueblock->ds.active){
				UnloadDialogue(&activedialogueblock->ds);
				activedialogueblock->hasds=false;
				activedialogueblock=NULL;
			}
		EndDrawing();
	}
	unloadmap(&level);
	FreeDialogueMap(&dlgmap);
	CloseWindow();
	return 0;

}
