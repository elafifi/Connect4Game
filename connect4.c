
#include "connect4.h"

// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// SSI0Fss       (SCE, pin 3) connected to PA3
// Reset         (RST, pin 4) connected to PA7
// Data/Command  (D/C, pin 5) connected to PA6
// SSI0Tx        (DN,  pin 6) connected to PA5
// SSI0Clk       (SCLK, pin 7) connected to PA2
// back light    (LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total

//===========================================> draw <==========================================

void draw(struct coin* c){
	Nokia5110_PrintBMP((*c).x - coinW/2, (*c).y + (coinH/2), (*c).image, 0);
	
}



//===========================================> gameInit <==========================================

void gameInit(){
	
	willWePlayFirst = 1;
	isMenuMode = 1;
	menuNum = 0;
	codingMode = 0;
		
	//the grid dimintions
	fullGridW = (cellW * numOfCol + vLineW*(numOfCol+1));
	fullGridH = (cellH*numOfRow +  hLineW*numOfRow);
	
	
	//get margins
	leftMargin = (SCREENW - fullGridW)/2;
	topMargin = SCREENH - fullGridH;
	
	//individual cell center
	cellCenX = vLineW + (cellW/2);
	cellCenY = hLineW + (cellH/2);
	
	playerPos = 0;
	
	//initialize the grid
	for(i = 0; i < numOfRow; i++){
		for(j = 0; j < numOfCol; j++){
			//calculate center position of each cell
			theGrid[numOfRow - 1 - i][j].x = leftMargin + j*(cellW + vLineW) + cellCenX;
			theGrid[numOfRow - 1 - i][j].y = SCREENH - 1 - (i*(cellH + hLineW) + cellCenY);
			
			//player = 0 mean the cell is empty
			theGrid[numOfRow - 1 - i][j].player = 0;
		}
	}
	
	//initialize the turn
	turn = 0;
	lastTurn = 0;
	
	
	//initialize players coins
	
	for(i = 0; i < numOfCoinsForEachPlayer; i++){
		
		//locate the coins on the top left of the grid and in the center of the 1st column
		//player one
		playersCoins[0][i].x = leftMargin + cellCenX;
		playersCoins[0][i].y = SCREENH - 1 - fullGridH - 3;
		//player two
		playersCoins[1][i].x = leftMargin + cellCenX;
		playersCoins[1][i].y = SCREENH - 1 - fullGridH - 3;
		
		
		//set the img for each player
		//player one
		playersCoins[0][i].image = pl1coin;
		//player two
		playersCoins[1][i].image = pl2coin;
		
		
	}
	
	//each column contains 0 coins at the begining
	for(i = 0; i < numOfCol; i++){
		colCoins[i] = 0;
		
		colCenter[i] = leftMargin + i*(cellW + vLineW) + cellCenX;
	}
	
	gameMode = 0;
}




//===========================================> DrawGrid <==========================================


void DrawGrid(){
	
	//draw vertical lines
	int yPos = SCREENH - 1;
	int xPos = 0;
	int i = 0;
	while( i <= numOfCol){
		xPos = i*(cellW + vLineW);
		Nokia5110_PrintBMP(xPos + leftMargin,  yPos, vvLine, 0);
		i++;
	}
	
	
	//draw vertical lines
	yPos = 0;
	xPos = leftMargin;
	i = 0;
	while( i < numOfRow){
		yPos = SCREENH - ( 1 +  i*(cellH + hLineW));
		Nokia5110_PrintBMP(xPos, yPos, hLine, 0);
		
		i++;
	}
}
//===========================================> update <==========================================

void update(){
	Nokia5110_ClearBuffer();
	
	
	//show grid
	DrawGrid();
	
	//show current player
	if(!winner)
		draw(&playersCoins[currPlayer][turn/2]);
	
	
	//show the coins inside the grid
	
	for(i = 0; i < turn; i++){
		draw(&playersCoins[i%2][i/2]);
	}
	
	
	Nokia5110_DisplayBuffer();
	

}


//===========================================> isThereAwinner <==========================================


//check if there is a winner
//return 1 if player one is the winner
//return 2 if player two is the winner
int isThereAwinner(){
	int status = 0;
	for(i = 0; i < numOfRow; i++){
		for(j = 0; j < numOfCol; j++){
			//check vertically
			if(i + 3 < numOfRow){
				if(
					theGrid[i][j].player == theGrid[i+1][j].player &&
					theGrid[i+1][j].player == theGrid[i+2][j].player &&
					theGrid[i+2][j].player == theGrid[i+3][j].player &&
					theGrid[i][j].player != 0
					){
						status = theGrid[i][j].player;
						break;
					}
			}
			
			//horizontally
			if(j + 3 < numOfCol){
				if(
					theGrid[i][j].player == theGrid[i][j+1].player &&
					theGrid[i][j+1].player == theGrid[i][j+2].player &&
					theGrid[i][j+2].player == theGrid[i][j+3].player &&
					theGrid[i][j].player != 0
					){
						status = theGrid[i][j].player;
						break;
					}
			}
			
			//diagonally right
			if(i + 3 < numOfRow && j + 3 < numOfCol){
				if(
					theGrid[i][j].player == theGrid[i+1][j+1].player &&
					theGrid[i+1][j+1].player == theGrid[i+2][j+2].player &&
					theGrid[i+2][j+2].player == theGrid[i+3][j+3].player &&
					theGrid[i][j].player != 0
					){
						status = theGrid[i][j].player;
						break;
					}
			}
			
			//diagonally left
			if(i + 3 < numOfRow && j - 3 >= 0){
				if(
					theGrid[i][j].player == theGrid[i+1][j-1].player &&
					theGrid[i+1][j-1].player == theGrid[i+2][j-2].player &&
					theGrid[i+2][j-2].player == theGrid[i+3][j-3].player &&
					theGrid[i][j].player != 0
					){
						status = theGrid[i][j].player;
						break;
					}
			}
		}
		
		if(status != 0)
			break;
	}
	
	
	return status ;
}

//===========================================> isThereAwinner <==========================================


int checkTriples(){
		cellReq=-1;
		for(i = 0; i < numOfRow; i++){
			for(j = 0; j < numOfCol; j++){
				//check vertically
				if(i + 2 < numOfRow){
					if(
						theGrid[i][j].player == theGrid[i+1][j].player &&
						theGrid[i+1][j].player == theGrid[i+2][j].player &&
						theGrid[i-1][j].player == 0 &&
						theGrid[i][j].player != 0 
						){
							cellReq = j ;
							if(theGrid[i][j].player == ai)
								break;
						}
				}
				
				//horizontal ends 
				if(		//right side
							j + 2 < numOfCol &&
							theGrid[i][j].player == theGrid[i][j+1].player &&
							theGrid[i][j+1].player == theGrid[i][j+2].player &&
							theGrid[i][j+3].player == 0 &&
							theGrid[i][j].player != 0 &&
							((i == 5 ) || (colCoins[j+3] == numOfRow - i - 1))
						){							
								cellReq = j+3;
								if(theGrid[i][j].player == ai)
									break;
							}
				if(		//left side
							j - 1 >= 0 &&
							theGrid[i][j].player == theGrid[i][j+1].player &&
							theGrid[i][j+1].player == theGrid[i][j+2].player &&
							theGrid[i][j-1].player == 0 &&
							theGrid[i][j].player != 0 &&
							((i == 5 ) || (colCoins[j-1] == numOfRow - i - 1 ))

						){
							cellReq = j-1;
							if(theGrid[i][j].player == ai)
									break;
							
							}
				//horizontal middle left   
				if(
						theGrid[i][j].player == theGrid[i][j+2].player &&
						theGrid[i][j+2].player == theGrid[i][j+3].player &&
						theGrid[i][j+1].player == 0  &&
						theGrid[i][j].player != 0 
						){if( 	//left side
								(i == 5 ) || (colCoins[j+1] == numOfRow - i - 1)
							)
								{
									cellReq = j+1;
									if(theGrid[i][j].player == ai)									
										break;
									
								}
						}
				//horizontal middle right
				if(
						theGrid[i][j].player == theGrid[i][j+1].player &&
						theGrid[i][j+1].player == theGrid[i][j+3].player &&
						theGrid[i][j+2].player == 0  &&
						theGrid[i][j].player != 0 
						){if( 	//right side
								(i == 5 ) || (colCoins[j+2] == numOfRow - i - 1)
							)
								{
									cellReq = j+2;
									if(theGrid[i][j].player == ai)
										break;
								}
						}
				//diagonally right
				if(i + 2 < numOfRow && j + 2 < numOfCol){
					if(	//after diagonal
						  j+3 < numOfCol &&
							i+3 < numOfRow &&
							theGrid[i][j].player == theGrid[i+1][j+1].player &&
							theGrid[i+1][j+1].player == theGrid[i+2][j+2].player &&
							theGrid[i][j].player != 0 &&
							theGrid[i+3][j+3].player == 0 &&
							colCoins[j+3] == numOfRow - i -4 
							
						)
						{
							cellReq = j+3;
							if(theGrid[i][j].player == ai)
										break;
						}
						if(	//before diagonal
 								j-1 >= 0 &&
								i-1 >= 0 &&
								theGrid[i][j].player == theGrid[i+1][j+1].player &&
								theGrid[i+1][j+1].player == theGrid[i+2][j+2].player &&
								theGrid[i][j].player != 0 &&
								theGrid[i-1][j-1].player == 0 &&
								colCoins[j-1] == numOfRow - i
							)
							{
										cellReq = j-1;
										if(theGrid[i][j].player == ai)
										break;
								
									}
							}
						
			
			
			//diagonally left
			if(i + 2 < numOfRow && j - 2 >= 0){
					if(	//after diagonal
							j-3 >= 0 &&
							i+3 < numOfRow &&
							theGrid[i][j].player == theGrid[i+1][j-1].player &&
							theGrid[i+1][j-1].player == theGrid[i-2][j-2].player &&
							theGrid[i][j].player != 0 &&
							theGrid[i+3][j-3].player == 0 &&
							colCoins[j-3] == numOfRow - i -4 
						)
						{
							cellReq = j-3;
							if(theGrid[i][j].player == ai)
										break;
						}
						if(	//before diagonal
 								j+1 < numOfCol &&
								i-1 >= 0 &&
								theGrid[i][j].player == theGrid[i+1][j-1].player &&
								theGrid[i+1][j-1].player == theGrid[i-2][j-2].player &&
								theGrid[i][j].player != 0 &&
								theGrid[i-1][j+1].player == 0 &&
								colCoins[j+1] == numOfRow - i
							)
							{
										cellReq = j+1;
										if(theGrid[i][j].player == ai)
											break;
									}
							}
							//diagonal right middle left   
				if(
						i + 3 < numOfRow && j + 3 < numOfCol &&
						theGrid[i][j].player == theGrid[i+2][j+2].player &&
						theGrid[i+2][j+2].player == theGrid[i+3][j+3].player &&
						theGrid[i+1][j+1].player == 0  &&
						theGrid[i][j].player != 0 &&
						colCoins[j+1] == numOfRow - i	- 2
						){
									cellReq = j+1;
									if(theGrid[i][j].player == ai)									
										break;
						}
				//diagonal right middle right
				if(
						i + 3 < numOfRow && j + 3 < numOfCol &&
						theGrid[i][j].player == theGrid[i+1][j+1].player &&
						theGrid[i+1][j+1].player == theGrid[i+3][j+3].player &&
						theGrid[i+2][j+2].player == 0  &&
						theGrid[i][j].player != 0 &&
						colCoins[j+2] == numOfRow - i	-3
						)
						{
							cellReq = j+2;
							if(theGrid[i][j].player == ai)
								break;
						}

						//diagonal left middle right
				if(
						(i + 3 < numOfRow )&& (j - 3 >= 0 )&&
						theGrid[i][j].player == theGrid[i+2][j-2].player &&
						theGrid[i+2][j-2].player == theGrid[i+3][j-3].player &&
						theGrid[i+1][j-1].player == 0  &&
						theGrid[i][j].player != 0 &&
						colCoins[j-1] == numOfRow - i	- 2
						){
									cellReq = j-1;
									if(theGrid[i][j].player == ai)									
										break;
						}
				//diagonal left middle left
				if(
						(i + 3 < numOfRow )&& (j - 3 >= 0 )&&
						theGrid[i][j].player == theGrid[i+1][j-1].player &&
						theGrid[i+1][j-1].player == theGrid[i+3][j-3].player &&
						theGrid[i+2][j-2].player == 0  &&
						theGrid[i][j].player != 0 &&
						colCoins[j-2] == numOfRow - i	-3
						)
						{
							cellReq = j-2;
							if(theGrid[i][j].player == ai)
								break;
						}
						
						}
					}

	if (cellReq > -1)
		return cellReq;
	else 
		return -1;
}

//=================================================> should Play WithS witch <=======================================
int shouldPlayWithSw(){
	if(
		gameMode == 1 || //if p1 vs p2
		(gameMode == 2 && opponentPlayerNum != currPlayer) // if p1 vs ai : p1 only should play with the swiches;
		)
	
		return 1;
	else
		return 0;
}

//=================================================> play In A column <=======================================


int playInAcol(){
	if(colCoins[playerPos] < numOfRow){
		theGrid[numOfRow - 1 - colCoins[playerPos]][playerPos].player = currPlayer + 1;
		playersCoins[currPlayer][turn/2].y = theGrid[numOfRow - 1 - colCoins[playerPos]][playerPos].y;
		colCoins[playerPos]++;
		turn++;
		return 1;
	}else
		return 0;
	
}


//===========================================> isThereAwinner <==========================================

//should return a valid position
int getAiNextPos(){
	triplePos = checkTriples();
	if(triplePos != -1){
				decision = triplePos;
	}
	else if(colCoins[3] != 6){
					decision = 3;
					triplePos=-1;
	}
	else{
		do
			decision = rand()%7;
		while (decision == 3);
	}
	
	return decision;
}
/*
void outputToTheScreen(int x, int y, char s[], int clear){
	if(clear)
		Nokia5110_Clear();
	Nokia5110_SetCursor(x, y);
	Nokia5110_OutString(s);
}
*/
//=================================================> Menu <=======================================

void theMenu(){
		if(menuNum == 0){
				GPIO_PORTF_DATA_R = 0x06;
				Nokia5110_Clear();
				Nokia5110_SetCursor(4, 0);
				Nokia5110_OutString("MENU");
				Nokia5110_SetCursor(2, 2);
				Nokia5110_OutString("P1 vs P2");
				Nokia5110_SetCursor(2, 3);
				Nokia5110_OutString("P1 vs AI");			
				Nokia5110_SetCursor(2, 4);
				Nokia5110_OutString("AI vs AI");

				
				Nokia5110_SetCursor( 0 , menuCursor + 2);
				Nokia5110_OutString(">>");

        //=================================================================================================			
				//wait for an input
					while(SW1 && SW2){
						SW1 = GPIO_PORTF_DATA_R&0x10;
						SW2 = GPIO_PORTF_DATA_R&0x01;
					};
				//move down in menu	
				if(!SW1){
					while(!SW1){SW1 = GPIO_PORTF_DATA_R&0x10;}
					menuCursor = (menuCursor + 1) % 3;
					GPIO_PORTF_DATA_R = 0x00;
					if(!codingMode)  Delay100ms(5);
				}
				//choose selection
				else if(!SW2){
					while(!SW2){SW2 = GPIO_PORTF_DATA_R&0x01;}
					gameMode = menuCursor + 1 ;
//					menuNum = 1 ;
					isMenuMode = 0;
					menuCursor = 0;
				}
				Nokia5110_SetCursor( 0 ,menuCursor + 2);
				Nokia5110_OutString(">>"); 
					}
}

//==========================================> splashScreen <=======================================

void splashScreen(){
	//==display logo
		Nokia5110_ClearBuffer();
		//Nokia5110_DisplayBuffer();
		Nokia5110_PrintBMP(10,  SCREENH-10, logo1, 0);
		Nokia5110_DisplayBuffer();
		Delay100ms(10);
		Nokia5110_ClearBuffer();
		Nokia5110_Clear();
		//==== team members
		Nokia5110_SetCursor(0,0);
		Nokia5110_OutString("Team Members");
		Delay100ms(5);
    Nokia5110_SetCursor(3,1);
		Nokia5110_OutString("1-Elafifi");
    Delay100ms(5);		
		Nokia5110_SetCursor(3,2);
		Nokia5110_OutString("2-Fry");
		Delay100ms(5);
    Nokia5110_SetCursor(3,3);
		Nokia5110_OutString("3-Sameh");
    Delay100ms(5);		
    Nokia5110_SetCursor(3,4);
		Nokia5110_OutString("4-Osama");		
		
		Delay100ms(8);
		Nokia5110_Clear();
	}
//==========================================> Main <=======================================

long prevSW1 = 0;        // previous value of SW1
long prevSW2 = 0;        // previous value of SW2
int main(void){
	UART1_Init();
  TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
  Random_Init(1);
  Nokia5110_Init();
  Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();      // draw buffer
	PortF_Init();
	
	gameInit();
	
	//UART1_OutChar((char)50);
	
	gameMode = 0;
	menuCursor = 0;
		
	willWePlayFirst = 1; // 0 === > first player /// 1 ==> second player
	ai = willWePlayFirst;
	
	
	if(!codingMode){
		splashScreen();
	}

	
	
  while(1){
		Nokia5110_ClearBuffer();
		SW1 = GPIO_PORTF_DATA_R&0x10;
		SW2 = GPIO_PORTF_DATA_R&0x01;
		
		if(isMenuMode){
			theMenu();
		}//menu code end
		
		
		else { //start play

			currPlayer = turn%2;
			opponentPlayerNum = willWePlayFirst;
		
			if(turn > lastTurn){
				playerPos = 0;
				lastTurn = turn;
			}
			winner = isThereAwinner();
			update();
			
			Nokia5110_SetCursor(1, 0);
			if(winner){
				//Nokia5110_Clear();
				if(winner == 1){
					
					Nokia5110_OutString("P1 wins");
				}
				else{
					Nokia5110_OutString("P2 wins");
				}
				break;
			}else if(turn == 42){ // winner == 0
					Nokia5110_OutString("Tie");
				break;
			}

			
				
				//when playing with switches
				if(shouldPlayWithSw())
				{
					//wait for an input
					while( SW1 && SW2){
						SW1 = GPIO_PORTF_DATA_R&0x10;
						SW2 = GPIO_PORTF_DATA_R&0x01;
					};
				
				
				
					//SW1 on release, move to the next position
					if(!SW1){
						
						//wait untill SW1 is released
						while(!SW1){SW1 = GPIO_PORTF_DATA_R&0x10;}
						playerPos = (playerPos + 1)%numOfCol;
						
						//turn = 0; turn%2 = 0; first player >> turn/2 = 0; first coin
						//turn = 1; turn%2 = 1; second player >> turn/2 = 0; first coin
						//turn = 2; turn%2 = 0; first player >> turn/2 = 1; second coin
						//turn = 3; turn%2 = 1; second player >> turn/2 = 1; second coin
						playersCoins[currPlayer][turn/2].x = colCenter[playerPos];
					}
					//SW2 on release, place the coin in the column if possible
					if(!SW2){
						//wait untill SW2 is released
						while(!SW2){SW2 = GPIO_PORTF_DATA_R&0x01;}
						

						
						playInAcol();
					}
					
				}else
				{//for the ai
					if(
						((gameMode == 2 && opponentPlayerNum == currPlayer) || //p1 vs ai
							(gameMode == 3)) // ai vs ai
						){
						if(!codingMode)
							Delay100ms(1);
						
						playerPos = getAiNextPos();
						playersCoins[currPlayer][turn/2].x = colCenter[playerPos];
						update();
						
						playInAcol();
						if(!codingMode)
							Delay100ms(2);
					}
				}
				
				
			}//end second main if
		
			
		
  }
}


void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}

void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}