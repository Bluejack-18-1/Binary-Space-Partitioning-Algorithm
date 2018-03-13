#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#define MIN_LENGTH 20
#define MAP_SIZE 50

struct room{ //room containing room data
	//coordinate of top right nodes
	int x;
	int y;
	//room size
	int width;
	int depth; //depth or height
	//subroom
	struct room* leftRoom;
	struct room* rightRoom;
}*root;

struct tile{ //tile for floor
	bool isWall;
	bool isDoorH;
	bool isDoorV;
}map[MAP_SIZE][MAP_SIZE];

struct room* makeRoom(int x, int y, int width, int depth); //prototype

void partition(struct room* newRoom){ // splitting room newRoom into two subRoom
	int door;
	if(newRoom->width >= newRoom->depth && newRoom->width > MIN_LENGTH*2){ // deciding if room should be divided vertically or horizontally
		int x; // room divided vertically ex [   ] -> [ | ]
		do{
			x = rand() % newRoom->width; //deciding where the room will be split using random
		}while(
				x < MIN_LENGTH || //validating that the new sub room is not smaller than the minimum length
				newRoom->width-x < MIN_LENGTH || //the same but for other room
				map[newRoom->y][newRoom->x+x-1].isDoorH || //validating that there is no door at the end of the newly generated wall
				map[newRoom->y+newRoom->depth-1][newRoom->x+x-1].isDoorH //the same but for the other end of the wall        x---------x
				//making sure that x is not door
			);
		do{
			door = rand() % newRoom->depth;
			//randomizing door for the newly generated wall
		}while(door % (newRoom->depth - 1) == 0); //validating that the door is not at the end of the newly generated wall(not inside the wall)
		map[newRoom->y+door][newRoom->x+x-1].isDoorV = true; //placing door
		newRoom->leftRoom = makeRoom(newRoom->x, newRoom->y, x, newRoom->depth); //making room for the left one [x| ] or up one
		newRoom->rightRoom = makeRoom(newRoom->x+x -1, newRoom->y, newRoom->width - x +1 , newRoom->depth); //making room for the right one [ |x] or down one
	}
	else if(newRoom->depth > MIN_LENGTH*2){ //do the same if horizontally
		int y;
		do{
			y = rand() % newRoom->depth;
		}while(y < MIN_LENGTH || newRoom->depth-y < MIN_LENGTH || map[newRoom->y+y-1][newRoom->x].isDoorV || map[newRoom->y+y-1][newRoom->x+newRoom->width-1].isDoorV);
		do{
			door = rand() % newRoom->width;
		}while(door % (newRoom->width - 1) == 0);
		map[newRoom->y+y-1][newRoom->x+door].isDoorH = true;
		newRoom->leftRoom = makeRoom(newRoom->x, newRoom->y, newRoom->width, y);
		newRoom->rightRoom = makeRoom(newRoom->x, newRoom->y + y -1 , newRoom->width, newRoom->depth - y + 1);
	}
}

struct room* makeRoom(int x, int y, int width, int depth){ //creating room
	//memory allocation for newRoom pointer
	struct room* newRoom = (room*) malloc(sizeof(room));
	
	//initializing newRoom data
	newRoom->x = x;
	newRoom->y = y;
	newRoom->width = width;
	newRoom->depth = depth;
	newRoom->leftRoom = newRoom->rightRoom = NULL;
	
	partition(newRoom); //spliting room
	
	return newRoom;
}

void makeWall(struct room* curr){ // making wall just like printing in binary tree
	if(curr){ //validating curr exist
		for(int i = 0; i < curr->depth; i++){ //printing wall using isWall
			for(int j = 0; j < curr->width; j++){
				if(i % (curr->depth-1) == 0 || j % (curr->width-1) == 0){
					map[i+curr->y][j+curr->x].isWall = true;
				}
			}
		}
		makeWall(curr->leftRoom); //print left
		makeWall(curr->rightRoom); //print right
	}
}

bool makeWall(struct room* curr, int level){ //for simulation, ignore it
//basically printing with depth restriction
	if(curr && level >= 0){
		for(int i = 0; i < curr->depth; i++){
			for(int j = 0; j < curr->width; j++){
				if(i % (curr->depth-1) == 0 || j % (curr->width-1) == 0){
					map[i+curr->y][j+curr->x].isWall = true;
				}
			}
		}
		level--;
		bool x = makeWall(curr->leftRoom, level);
		bool y = makeWall(curr->rightRoom, level);
		return x || y;
	}
}

void printMapStimulation(){ //printMap for Stimulation
	for(int i = 0; i < MAP_SIZE; i++){
		for(int j = 0; j < MAP_SIZE; j++){ //difference with normal one is that the door is space
			if(map[j][i].isDoorV){
				printf(" ");
			}
			else if(map[j][i].isDoorH){
				printf(" ");
			}
			else if(map[j][i].isWall){
				printf("#");
			}
			else{
				printf(" ");
			}
		}
		printf("\n");
	}
}

void printMap(){ // print map
	for(int i = 0; i < MAP_SIZE; i++){
		for(int j = 0; j < MAP_SIZE; j++){
			if(map[j][i].isDoorV){ //door
				printf("-");
			}
			else if(map[j][i].isDoorH){ //door
				printf("|");
			}
			else if(map[j][i].isWall){ //wall
				printf("#");
			}
			else{
				printf(" "); //space
			}
		}
		printf("\n");
	}
}

void resetWall(){ //reset all to false. for stimulation
	for(int i = 0; i < MAP_SIZE; i++){
		for(int j = 0; j < MAP_SIZE; j++){
			map[j][i].isWall = false;
		}
	}
}

void freeAll(struct room* curr){ //freeing data
	if(curr){
		freeAll(curr->leftRoom);
		freeAll(curr->rightRoom);
		free(curr);
	}
}

void stimulation(){ //stimulation for splitting step by step
	for(int i = 0; i < 10; i++){
		resetWall();
		if(!makeWall(root, i))
			break;
		printMapStimulation();
		getchar();
		system("CLS");
	}
}

int main(){ //main function
	srand(time(NULL)); //initialize random
	root = makeRoom(0, 0, MAP_SIZE, MAP_SIZE); //make first room and the child with split in the function
	//for stimulation
	stimulation(); 
	
	//creating wall
	makeWall(root);
	
	printf("DONE!\n");
	//printing map
	printMap();
	
	//freeing data
	freeAll(root);
	
	//done
	getchar();
	return 0;
}
