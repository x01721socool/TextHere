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

// 4-directional movement (Up, Down, Left, Right)
static int dx[4] = { 1, 0, -1, 0 };
static int dy[4]= { 0, 1, 0, -1 };

// Heuristic function: Manhattan distance (good for 4-way grids)
static float GetHeuristic(int x1, int y1, int x2, int y2) {
    return (float)(abs(x1 - x2) + abs(y1 - y2));
}

pathstruct findpath(npc *n, gamemap map, Vector2 target, int ts) {
    pathstruct result = { NULL, 0 };

    // 1. Convert world positions to grid coordinates
    int startX = (int)(n->pos.x / ts);
    int startY = (int)(n->pos.y / ts);
    int endX = (int)(target.x / ts);
    int endY = (int)(target.y / ts);

    // Bounds check
    if (endX < 0 || endX >= map.width || endY < 0 || endY >= map.height) return result;
    if (map.walls[endY * map.width + endX] == 1) return result; // Target is a wall

    // 2. Initialize the grid
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

    // 3. Setup start node
    node* startNode = &grid[startY * map.width + startX];
    startNode->g = 0;
    startNode->h = GetHeuristic(startX, startY, endX, endY);
    startNode->isopen = true;

    bool foundPath = false;

    // 4. Main A* Loop
    while (true) {
        // Find the open node with the lowest F cost (F = G + H)
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

        if (current == NULL) break; // No path found

        if (current->x == endX && current->y == endY) {
            foundPath = true;
            break;
        }

        // Move current to closed set
        current->isopen = false;
        current->isclosed = true;

        // Check neighbors
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

    // 5. Reconstruct Path
    if (foundPath) {
        // First, count nodes for allocation
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

        // Fill path backwards from end to start
        currX = endX;
        currY = endY;
        for (int i = count - 1; i >= 0; i--) {
            // Convert grid coords back to world coords (centered on tile)
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

int **intsoverrads(int radius,int pos[2]) {
  int *p=malloc((size_t)(radius*2+1)*sizeof(int));
  for (int i=0;i<=radius;i++){
    p[i]=pos[0]-i;
  }
  for (int k=radius+1;k<=radius*2;k++){
    p[k]=pos[0]+k-radius;
  }
  int **z=malloc((size_t)(radius*4+2)*sizeof(int*));
  for (int j=0;j<radius*4+2;j++) {
    z[j]=malloc(2*sizeof(int));
    z[j][0]=p[j%(radius*2+1)];
    z[j][1]=(int)sqrt(powf((float)radius,2.0f)-powf((float)p[j%(radius*2+1)],2.0f))*((j>radius)?-1:1);
    if (z[j][1]==-2147483648) {
      p[j%(radius*2+1)]=0;
      z[j][1]=(int)sqrt(powf((float)radius,2.0f)-powf((float)p[j%(radius*2+1)],2.0f))*((j>radius)?-1:1);
    }
  }
  free(p);
  z[radius*4+2]=NULL;
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
    if (map.walls[map.width*p[i][0]+p[i][1]]==1){
      p[i]=NULL;
      int c=i+1;
      while(p[c+1]!=NULL){
        p[c]=p[c+1];
        c++;
      }
    }
  i++;
  }
  p[countfirstp(p)]=NULL;
}
void remnegp(int **p){
  int i=0;
  while (p[i]!=NULL){
    if (p[i][0]<0||p[i][1]<0){
      p[i]=NULL;
      int c=i+1;
      while (p[c+1]!=NULL){
        p[c]=p[c+1];
        c++;
      }
    }
    i++;
  }
  p[countfirstp(p)]=NULL;
}
void remnray2circp(int **p,Vector2 targ,int ts,int lim) {
  int i=0;
  while (p[i]!=NULL) {
    Vector2 opos=(Vector2){(float)(p[i][0]*ts+32),
      (float)(p[i][1]*ts+32)};
    ray r={.lengthlim=lim,.hitwall=false,.hitcirc=false};
    ray2circ(&r,opos,targ,lim,ts);
    if (!r.hitcirc) {
      p[i]=NULL;
      int c=i+1;
      while (p[c+1]!=NULL){
        p[c]=p[c+1];
        c++;
      }
    }
    i++;
  }
  p[countfirstp(p)]=NULL;
}
