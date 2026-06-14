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

static int dx[4] = { 1, 0, -1, 0 };
static int dy[4]= { 0, 1, 0, -1 };

static float GetHeuristic(int x1, int y1, int x2, int y2) {
    return (float)(abs(x1 - x2) + abs(y1 - y2));
}

pathstruct findpath(npc *n, gamemap map, Vector2 target, int ts) {
    pathstruct result = { NULL, 0 };

    int startX = (int)(n->pos.x / ts);
    int startY = (int)(n->pos.y / ts);
    int endX = (int)(target.x / ts);
    int endY = (int)(target.y / ts);

    if (endX < 0 || endX >= map.width || endY < 0 || endY >= map.height) return result;
    if (map.walls[endY * map.width + endX] == 1) return result;

    node* grid = (node*)malloc(sizeof(node) * map.width * map.height);
    for (int y = 0; y < map.height; y++) {
        for (int x = 0; x < map.width; x++) {
            grid[y * map.width + x].x = x;
            grid[y * map.width + x].y = y;
            grid[y * map.width + x].isopen = false;
            grid[y * map.width + x].isclosed = false;
            grid[y * map.width + x].g = FLT_MAX;
            grid[y * map.width + x].parentx = -1;
            grid[y * map.width + x].parenty = -1;
        }
    }

    node* startNode = &grid[startY * map.width + startX];
    startNode->g = 0;
    startNode->h = GetHeuristic(startX, startY, endX, endY);
    startNode->isopen = true;

    bool foundPath = false;

    while (true) {
        node* current = NULL;
        float lowestF = FLT_MAX;

        for (int i = 0; i < map.width * map.height; i++) {
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

            if (nx >= 0 && nx < map.width && ny >= 0 && ny < map.height) {
                if (map.walls[ny * map.width + nx] == 1 || grid[ny * map.width + nx].isclosed) continue;

                float newG = current->g + 1.0f;
                node* neighbor = &grid[ny * map.width + nx];

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
            int nextX = grid[currY * map.width + currX].parentx;
            int nextY = grid[currY * map.width + currX].parenty;
            currX = nextX;
            currY = nextY;
        }

        result.count = count;
        result.path = (Vector2*)malloc(sizeof(Vector2) * count);

        currX = endX;
        currY = endY;
        for (int i = count - 1; i >= 0; i--) {
            result.path[i] = (Vector2){ (float)currX * ts + ts / 2.0f, (float)currY * ts + ts / 2.0f };
            int nextX = grid[currY * map.width + currX].parentx;
            int nextY = grid[currY * map.width + currX].parenty;
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

bool hasLineOfSight(Vector2 fromPos, Vector2 toPos, gamemap map, int ts) {
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
    
    int mapx = (int)(checkX / ts);
    int mapy = (int)(checkY / ts);
    
    if (mapx < 0 || mapx >= map.width || mapy < 0 || mapy >= map.height) {
      return false;
    }
    
    if (map.walls[mapy * map.width + mapx] == 1) {
      return false;
    }
  }
  
  return true;
}

void remnray2circp(int **p, Vector2 targ, int ts, int lim, gamemap map) {
  int i = 0;
  while (p[i] != NULL) {
    Vector2 candidatePos = (Vector2){
      (float)(p[i][0] * ts + ts / 2),
      (float)(p[i][1] * ts + ts / 2)
    };
    
    if (!hasLineOfSight(candidatePos, targ, map, ts)) {
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
