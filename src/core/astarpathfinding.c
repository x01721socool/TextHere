/*
 * A* Pathfinding Algorithm
 * Based on: https://github.com/HimanshuK704/A-Star-Pathfinding-Algorithm-in-C
 * Original Author: Shardul Hirolikar
 * License: MIT License
 * 
 * Modified for TextHere project
 */
#include "basicnpc.h"
#include "astarpathfinding.h"
#include "raymath.h"
#include "raycast.h"
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "../game.h"

static int dx[4] = { 1, 0, -1, 0 };
static int dy[4]= { 0, 1, 0, -1 };

static float GetHeuristic(int x1, int y1, int x2, int y2) {
  return (float)(abs(x1 - x2) + abs(y1 - y2));
}

pathstruct findpath(npc *n, Vector2 target,game games) {
  pathstruct result = { NULL, 0 };

  int startX = (int)(n->pos.x /games.ts);
  int startY = (int)(n->pos.y /games.ts);
  int endX = (int)(target.x /games.ts);
  int endY = (int)(target.y /games.ts);

  if (endX < 0 || endX >= games.level.width || endY < 0 || endY >= games.level.height) return result;
  if (games.level.walls[endY * games.level.width + endX] == 1) return result;

  node* grid = (node*)malloc(sizeof(node) * games.level.width * games.level.height);
  for (int y = 0; y < games.level.height; y++) {
    for (int x = 0; x < games.level.width; x++) {
      grid[y * games.level.width + x].x = x;
      grid[y * games.level.width + x].y = y;
      grid[y * games.level.width + x].isopen = false;
      grid[y * games.level.width + x].isclosed = false;
      grid[y * games.level.width + x].g = FLT_MAX;
      grid[y * games.level.width + x].parentx = -1;
      grid[y * games.level.width + x].parenty = -1;
    }
  }

  node* startNode = &grid[startY * games.level.width + startX];
  startNode->g = 0;
  startNode->h = GetHeuristic(startX, startY, endX, endY);
  startNode->isopen = true;

  bool foundPath = false;

  while (true) {
    node* current = NULL;
    float lowestF = FLT_MAX;

    for (int i = 0; i < games.level.width * games.level.height; i++) {
      if (grid[i].isopen) {
        float f = grid[i].g + grid[i].h;
        if (f < lowestF) {
          lowestF = f;
          current = &grid[i];
        }
      }
    }

    if (current == NULL) break;

    if (current->x == endX && current->y == endY) {
      foundPath = true;
      break;
    }

    current->isopen = false;
    current->isclosed = true;

    for (int i = 0; i < 4; i++) {
      int nx = current->x + dx[i];
      int ny = current->y + dy[i];

      if (nx >= 0 && nx < games.level.width && ny >= 0 && ny < games.level.height) {
        if (games.level.walls[ny * games.level.width + nx] == 1 || grid[ny * games.level.width + nx].isclosed) continue;

        float newG = current->g + 1.0f;
        node* neighbor = &grid[ny * games.level.width + nx];

        if (newG < neighbor->g) {
          neighbor->parentx = current->x;
          neighbor->parenty = current->y;
          neighbor->g = newG;
          neighbor->h = GetHeuristic(nx, ny, endX, endY);
          neighbor->isopen = true;
        }
      }
    }
  }

  if (foundPath) {
    int count = 0;
    int currX = endX;
    int currY = endY;
    while (currX != -1 && currY != -1) {
      count++;
      int nextX = grid[currY * games.level.width + currX].parentx;
      int nextY = grid[currY * games.level.width + currX].parenty;
      currX = nextX;
      currY = nextY;
    }

    result.count = count;
    result.path = (Vector2*)malloc(sizeof(Vector2) * count);

    currX = endX;
    currY = endY;
    for (int i = count - 1; i >= 0; i--) {
      result.path[i] = (Vector2){ (float)currX *games.ts + games.ts / 2.0f, (float)currY *games.ts +games.ts / 2.0f };
      int nextX = grid[currY * games.level.width + currX].parentx;
      int nextY = grid[currY * games.level.width + currX].parenty;
      currX = nextX;
      currY = nextY;
    }
  }

  free(grid);
  return result;
}

int **intsoverrads(int radius, int pos[2]) {
  int *xcoords = malloc((size_t)(radius * 2 + 1) * sizeof(int));
  for (int i = 0; i <= radius * 2; i++) {
    xcoords[i] = pos[0] - radius + i;
  }

  int totalPoints = 2 * (radius * 2 + 1);
  int **z = malloc((size_t)(totalPoints + 1) * sizeof(int*));

  for (int j = 0; j < totalPoints; j++) {
    z[j] = malloc(2 * sizeof(int));
    int x = xcoords[j % (radius * 2 + 1)];
    z[j][0] = x;

    int dx = x - pos[0];
    float discriminant = (float)(radius * radius - dx * dx);

    if (discriminant < 0) discriminant = 0;

    int y_abs = (int)sqrtf(discriminant);
    z[j][1] = (j < radius * 2 + 1) ? y_abs + pos[1] : -y_abs + pos[1];
  }

  free(xcoords);
  z[totalPoints] = NULL;
  return z;
}

int countfirstp(int **p){
  int k=0;
  while (p[k]!=NULL){
    k++;
  }
  return k;
}

void remdupfirstp(int **p) {
  int **z=p;
  int i=0;
  while (z[i]!=NULL) {
    if (z[i]==z[i+1]&&z[i+1]!=NULL){
      z[i+1]=NULL;
      int c=i+1;
      while (z[c+1]!=NULL){
        z[c]=z[c+1];
        c++;
      }
    }
    i++;
  }
  p[countfirstp(z)]=NULL;
  p=z;
}

void remblockedp(int **p,gamemap map) {
  int i=0;
  while (p[i]!=NULL) {
    if (!(p[i][1]<0||p[i][1]>=map.height||p[i][0]<0||p[i][0]>=map.width)&&map.walls[map.width*p[i][1]+p[i][0]]==1){
      free(p[i]);
      int c=i;
      while(p[c+1]!=NULL){
        p[c]=p[c+1];
        c++;
      }
      p[c]=NULL;
    } else {
      i++;
    }
  }
}

void remnegp(int **p){
  int i=0;
  while (p[i]!=NULL){
    if (p[i][0]<0||p[i][1]<0){
      free(p[i]);
      int c=i;
      while (p[c+1]!=NULL){
        p[c]=p[c+1];
        c++;
      }
      p[c]=NULL;
    } else {
      i++;
    }
  }
}

bool hasLineOfSight(Vector2 fromPos, Vector2 toPos, game games) {
  float dx = toPos.x - fromPos.x;
  float dy = toPos.y - fromPos.y;
  float dist = sqrtf(dx * dx + dy * dy);

  if (dist < 0.01f) return true;

  float angle = atan2f(dy, dx);
  float stepX = cosf(angle);
  float stepY = sinf(angle);

  for (float step = 0; step < dist; step += 1.0f) {
    float checkX = fromPos.x + stepX * step;
    float checkY = fromPos.y + stepY * step;

    int mapx = (int)(checkX / games.ts);
    int mapy = (int)(checkY / games.ts);

    if (mapx < 0 || mapx >= games.level.width || mapy < 0 || mapy >= games.level.height) {
      return false;
    }

    if (games.level.walls[mapy * games.level.width + mapx] == 1) {
      return false;
    }
  }

  return true;
}

void remnray2circp(int **p, Vector2 targ, int lim,game games) {
  int i = 0;
  while (p[i] != NULL) {
    Vector2 candidatePos = (Vector2){
      (float)(p[i][0] * games.ts +games.ts / 2),
        (float)(p[i][1] * games.ts + games.ts / 2)
    };

    if (!hasLineOfSight(candidatePos, targ, games)) {
      free(p[i]);
      int c = i;
      while (p[c + 1] != NULL) {
        p[c] = p[c + 1];
        c++;
      }
      p[c] = NULL;
    } else {
      i++;
    }
  }
  p[countfirstp(p)] = NULL;
}
