#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "img.h"
#include "UART.h"
#include <stdio.h>


int const cellW = 8; //cell width // coinW= 6
int const cellH = 5; //cell height  // coinH = 3
int const hLineW = 1; //horizontal line width
int const vLineW = 2; // vertical line width
int const numOfCol = 7; 
int const numOfRow = 6;
int const numOfCoinsForEachPlayer = 21; // (numOfCol*numOfRow)/2
int const coinH = 3; //cell height
int const coinW = 6; //cell width
int leftMargin; //empty space on the left of the grid
int topMargin; //empy space on the top of the grid
int fullGridH; //grid height
int fullGridW; //grid width
int turn; //current turn
int lastTurn; //turn in the previous loop
int cellCenX; //half width of the cell
int cellCenY; //half height of the cell
int colCoins[numOfCol]; //number of coins in each column
int colCenter[numOfCol]; //each column center on x axis
int playerPos; //position of the player coin before playing
int winner; // who is the winner
int gameMode; // 0: menu, 1: 2players, 2: 1player vs ai,  3: ai vs ai
int menuCursor;
int i;
int	j;
int willWePlayFirst;
int ai;
int currPlayer;
int opponentPlayerNum;
int isMenuMode;
int menuNum;
int SW1;
int SW2;
int codingMode;
int cellReq;
int triplePos;
int decision;
extern long prevSW1;        // previous value of SW1
extern long prevSW2;        // previous value of SW2
extern char x;
//this structure describes each individual cell
//(x, y) are the center point of a cell
//player = 0 >> cell is empty
//player = 1 >> cell is taken by player one
//player = 2 >> cell is taken by player two
struct cell{
	int x;
	int y;
	int player;
};

//the full grid cells
struct cell theGrid[numOfRow][numOfCol];


//structure that describes the coins
//(x, y) are the center point of a coin
//image: the image of a coin
//draw(): draws a coin at its position
struct coin {
	int x;
	int y;
	const unsigned char *image;
	
	//void (*draw)(struct coin*);
};

//conis for each players
//playersCoins[0]: first player
//playersCoins[1]: second player
struct coin playersCoins[2][numOfCoinsForEachPlayer];

void Delay100ms(unsigned long count);
void PortF_Init(void);
void draw(struct coin* c);
void gameInit();
void DrawGrid();
void update();
//check if there is a winner
//return 1 if player one is the winner
//return 2 if player two is the winner
int isThereAwinner();
int checkTriples();
//=================================================> should Play WithS witch <=======================================
int shouldPlayWithSw();
//=================================================> play In A column <=======================================
int playInAcol();
//should return a valid position
int getAiNextPos();
void outputToTheScreen();
//=================================================> Menu <=======================================
void theMenu();
void splashScreen();
//==========================================> Main <=======================================


