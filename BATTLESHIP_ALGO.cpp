#include <iostream>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

//Constants
const int BOARD_SIZE = 10;
const char colArr[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',};
int ship_Sizes[5] = {2, 3, 3, 4, 5};
int arrSize = sizeof(ship_Sizes)/sizeof(ship_Sizes[0]);
const char checkShip = 'S';
const char checkWater = '~';
const char checkHit = 'X';
const char checkMiss = 'O';

//Boards
char p_GuessBoard[BOARD_SIZE][BOARD_SIZE];
char player_Board[BOARD_SIZE][BOARD_SIZE];
char c_GuessBoard[BOARD_SIZE][BOARD_SIZE];
char com_Board[BOARD_SIZE][BOARD_SIZE];							

//Function Declarations
void createBoard(char board[BOARD_SIZE][BOARD_SIZE]);
void displayBoard(char board[BOARD_SIZE][BOARD_SIZE]);
bool winTracker(char board [BOARD_SIZE][BOARD_SIZE]);
bool numChecker(string num);
void userInput(int &row, int &col);
bool AttackPhase(int &turn, int &thisPlayer);
char positionOnBoard();
bool inBoundary(int row, int col, char orientation, int shipSize);
bool spaceOccupied(char (&board)[BOARD_SIZE][BOARD_SIZE], int row, int col, char orientation, int shipSize, int &turn, int &thisPlayer);
void placeShip(int ship_Sizes[5], int sizeArr, int &turn, int &thisPlayer, char (&board)[BOARD_SIZE][BOARD_SIZE]);

//Main Function
int main(){
	int enemyTurn, thisPlayer;
	int row, col;
	bool hit;
	while(thisPlayer != 1 && thisPlayer != 2){
		cout << "Choose which player you want to be (1 or 2): " << endl;
		cin >> thisPlayer;	
	}
	if(thisPlayer == 1){
		enemyTurn = 2;
	} else{
		enemyTurn = 1;
	}
	//initialization of player guessing boards
	createBoard(p_GuessBoard);
	createBoard(player_Board);
	displayBoard(player_Board);
	createBoard(c_GuessBoard);
	createBoard(com_Board);
	//insert function for placing ships for both players then display player guess board and maybe opponent's guess board
	placeShip(ship_Sizes, arrSize, thisPlayer, thisPlayer, player_Board);
	placeShip(ship_Sizes, arrSize, enemyTurn, thisPlayer, com_Board);
	
	//Main Game Loop
	while(winTracker(p_GuessBoard) == false && winTracker(c_GuessBoard) == false){
		system("CLS");
		cout << "Computer GuessBoard: " << endl << endl;
		displayBoard(c_GuessBoard);
		cout << "\n\nPlayer GuessBoard:" << endl << endl;
		displayBoard(p_GuessBoard);
		/*cout << "\n\nComputer Board:" << endl << endl;
		displayBoard(com_Board); only for recording and testing purposes*/
		//Player's Turn
		hit = AttackPhase(thisPlayer, thisPlayer);
		if (winTracker(p_GuessBoard) == true){
			cout << "Player win, CPU lost..." << endl;
			break;
		}
		if(hit == true){
			while(hit == true){ //while loop if player got a hit
				system("CLS");
				cout << "Computer GuessBoard:" << endl << endl;
				displayBoard(c_GuessBoard);
				cout << "\n\nPlayer GuessBoard:" << endl << endl;
				displayBoard(p_GuessBoard);
				/*cout << "\n\nComputer Board:" << endl << endl;
				displayBoard(com_Board); only for recording and testing purposes*/
				hit = AttackPhase(thisPlayer, thisPlayer);
				if (winTracker(p_GuessBoard) == true){
					cout << "Player win, CPU lost..." << endl;
					break;
				}
			}
		}
		
		//Enemy's turn
		hit = AttackPhase(enemyTurn, thisPlayer);
		if (winTracker(c_GuessBoard) == true){	
			break;
		}
		if(hit == true){
			while(hit == true){
				hit = AttackPhase(enemyTurn, thisPlayer);
				if (winTracker(c_GuessBoard) == true){
					cout << "CPU win, Player lost..." << endl;
					break;
				}
			}
		}
	}
	return 0;
}

//Functions

//Create Board Function by Alfonzo Miguel C. Ayson
void createBoard(char board[BOARD_SIZE][BOARD_SIZE]){
	
	for(int x = 0; x < BOARD_SIZE; x++){
		for(int y = 0; y < BOARD_SIZE; y++){
			board[x][y] = '~';
		}
	}
}

//Display Board Function by Alfonzo Miguel C. Ayson
void displayBoard(char board[BOARD_SIZE][BOARD_SIZE]){
	cout << "	A	B	C	D	E	F	G	H	I	J \n";
	cout << "_____________________________________________________________________________________\n";
	for(int x = 0; x < BOARD_SIZE; x++){
		printf("%d |	", x+1);
		for(int y = 0; y < BOARD_SIZE; y++){
			if (y == 9){
				printf("%c   |", board[x][y]);	
			} else{
				printf("%c\t", board[x][y]);
			}
		}
		cout << endl;
		cout << "_____________________________________________________________________________________\n";
	} 
}

//Tracker for number of hits each player has per turn by Alfonzo Miguel C. Ayson
bool winTracker(char board [BOARD_SIZE][BOARD_SIZE]){
	int count = 0;
	for(int x = 0; x < BOARD_SIZE; x++){
		for(int y = 0; y < BOARD_SIZE; y++){
			if(board[x][y] == 'X'){
				count++;
			}
		}
	}
	
	if(count == 17){
		return true;
	}
	else{
		return false;
	}
}
//Part of userInput function to check for valid number inputs for row coordinates by Alfonzo Miguel C. Ayson
bool numChecker(string num){
	for (int i = 0; i < num.length(); i++)
		if(isdigit(num[i]) == false)
			return false;
	return true;
}

/*User Input function just to make 
	validity checking easier code translated 
	from Python courtesy of Knowledge Mavens 
	with a few modifications to fit the C++ language, 
	translated to C++ by Alfonzo Miguel C. Ayson*/
void userInput(int &row, int &col){
	string inRowStr;
	int inRow;
	char inCol;
	while (true){
		cout << "Please enter row coordinate (1-10): ";
		cin >> inRowStr;
		if(numChecker(inRowStr) == false){
			while(numChecker(inRowStr) == false){
				cout << "Invalid Input Please Enter a Number for the Row Coordinate (1-10): ";
				cin >> inRowStr;
				if(numChecker(inRowStr) == true){
					break;
				}
			}
		}
		inRow = stoi(inRowStr);
		if (inRow > 0 && inRow <= 10){
			row = --inRow;
			break;
		}
		else{
			cin.clear();
			cin.ignore(123, '\n');
			continue;
		}
	}
	while (true){
		cout << "Please enter column coordinate (A-J): ";
		cin >> inCol;
		/*if(inCol.length() > 1 || numChecker(inCol) == true){
			while(inCol.length() > 1 || numChecker(inCol) == true){
				cout << "Please enter only ONE letter in the range of (A-J) for the column coordinate: ";
				cin >> inColStr;
				if(numChecker(inCol) == false && inCol.length() == 1){
					break;
				}
			}
		}*/
		//strcpy(inColArr, inColStr.c_str());
		inCol = toupper(inCol);
		//inCol = toupper(inCol);
		if (inCol >= 'A' && inCol <= 'J'){
		    for (int x = 0; x < 10; x++){
		        if(colArr[x] == inCol){
		            col = x;
		        }
		    }
			break;
		}
		else{
			cin.clear();
			cin.ignore(123, '\n');
			continue;
		}
	}
}

//Attacking Function by Candelaria Adriel
bool AttackPhase(int &turn, int &thisPlayer){ //Don't test yet
	int row;
	int col;
	bool hit;
	
	//indicating who is attacking
	if(turn == thisPlayer){
		cout<< "ATTACKING PHASE";
		cout << "Player " << thisPlayer << " Enter Coordinates To Be Attacked: \n";
		userInput(row, col);
		if(p_GuessBoard[row][col] == checkMiss || p_GuessBoard[row][col] == checkHit){
			while (p_GuessBoard[row][col] == checkMiss || p_GuessBoard[row][col] == checkHit){
				cout << "Player " << thisPlayer << " Coordinates entered were already previously chosen please enter new coordinates to be attacked: \n";
				userInput(row, col);
				if (p_GuessBoard[row][col] != checkMiss|| p_GuessBoard[row][col] != checkHit){
					break;
				}
			}
		}
		else if(com_Board[row][col] == checkWater){
			p_GuessBoard[row][col] = checkMiss;
			hit = false;
		} else if (com_Board[row][col] == checkShip){
			p_GuessBoard[row][col] = checkHit;
			hit = true;
		}
	} else{ //if it is not the player's turn, then it is the enemy's turn to attack
		srand(time(0)); //randomie row and col input of AI
		row = 1+(rand()%10);
		col = 1+(rand()%10);
		if(c_GuessBoard[row][col] == checkMiss || c_GuessBoard[row][col] == checkHit){
			while (c_GuessBoard[row][col] == checkMiss|| c_GuessBoard[row][col] == checkHit){//keep randomizing row and col values until the coordinate is no longer a marked hit or miss
				srand(time(0));
				row = 1+(rand()%10);
				col = 1+(rand()%10);
				if (c_GuessBoard[row][col] != checkMiss || c_GuessBoard[row][col] != checkHit){
					break;
				}
			}
		}
		if(player_Board[row][col] == checkWater){
			c_GuessBoard[row][col] = checkMiss;
			hit = false;
		} else if (player_Board[row][col] == checkShip){
			c_GuessBoard[row][col] = checkHit;
			hit = true;
		}
	}
	return hit;
}
	
//Position Placement Function by Emmanuel Irgil S. Revista
char positionOnBoard(){
	char pos;
	
	cout << "Where do you want your ship to face?" << endl;
	cout << "[H] - Horizontal" << endl;
	cout << "[V] - Vertical" << endl;
	cin >> pos;
	pos = tolower(pos);
	
	while (pos != 'h' && pos != 'v'){
		cout << "Invalid input of position...." << endl;
		cout << "Where do you want your ship to face?" << endl;
		cout << "[H] - Horizontal" << endl;
		cout << "[V] - Vertical" << endl;
		cin >> pos;
	}
	return pos;
	
}

//function to check whether the postion is within the boundary by Emmanuel Irgil S. Revista	
bool inBoundary(int row, int col, char orientation, int shipSize){
	if(orientation == 'v'){
		if(shipSize + row > 10){
			return false;
		}
		else{
			return true;
		}
	} else{
		if(shipSize + col > 10){
			return false;
		} else{
			return true;
		}	
	}
}

//function to check whether the spaces on the board is already occupied or not by Emmanuel Irgil S. Revista
bool spaceOccupied(char (&board)[BOARD_SIZE][BOARD_SIZE], int row, int col, char orientation, int shipSize){
	bool occupied;

	if(orientation == 'h'){
		for(int i = col; i < col+shipSize; i++){
			if(board[row][i] == checkShip){
				return true;
			} 
		}
	} else{ 
		for(int i = row; i < row+shipSize; i++){
			if(board[i][col] == checkShip){
				return true;
			} 
		}
	}	

	return false;
}

//Function for placing ships on the boards by Alfonzo Miguel C. Ayson
void placeShip(int ship_Sizes[5], int sizeArr, int &turn, int &thisPlayer, char (&board)[BOARD_SIZE][BOARD_SIZE]){
	int row, col;
	char orientation;
	int orientationInt;
	
	for(int x = 0; x < sizeArr; x++){ //for each loop for placing each ship on the board
		while(true){
			if(turn == thisPlayer){
				cout << "Placing ship of size " << ship_Sizes[x] << " please choose your coordinates for placement" << endl;
				userInput(row, col);
				orientation = positionOnBoard();
				cout << orientation << endl << endl;
				if(inBoundary(row, col, orientation, ship_Sizes[x]) == true){
					if(spaceOccupied(board, row, col, orientation, ship_Sizes[x]) == false){
						if(orientation == 'h'){
							for(int i = col; i < col + ship_Sizes[x]; i++){
								board[row][i] = checkShip;
							}
						} else{
							for(int i = row; i < row + ship_Sizes[x]; i++){
								board[i][col] = checkShip;
							}
						}
						break;
					}
				}
			} else{
				srand(time(0));
				row = 1 + (rand()%10);
				col = 1 + (rand()%10);
				srand(time(0));
				orientationInt = 1 + (rand()%2);
				if (orientationInt == 1){
					orientation = 'h';
				} else{
					orientation = 'v';
				}
				
				if (inBoundary(row, col, orientation, ship_Sizes[x]) == true){
					if(spaceOccupied(board, row, col, orientation, ship_Sizes[x]) == false){
						if(orientation == 'h'){
							for(int i = col; i < col + ship_Sizes[x]; i++){
								board[row][i] = checkShip;
							}
						} else{
							for(int i = row; i < row + ship_Sizes[x]; i++){
								board[i][col] = checkShip;
							}
						}
						break;
					}
				}
			}
		}
	}
}
