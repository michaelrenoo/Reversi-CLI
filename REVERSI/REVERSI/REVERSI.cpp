#include <iostream>
#include <stdexcept>
#include <array>
#include <vector>
#include <regex>
#include <stdlib.h>
#include <windows.h>

using namespace std;

std::string userInput = "start";
std::string legalInputs[];
int inputX = 0;
int inputY = 0;
// Check all 8 surround positions
int surroundingPosDeltas[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, // 3 positions above
									{0, -1}, {0, 1}, // 2 positions on same row
								  {1, -1}, {1, 0}, {1, 1} }; // 3 positions below
//const bool PLAY_AI = true; // set to true if you want to play 

void startgame(char board[8][8]);
void printBoard(char newBoard[8][8], int inputX_C = -1, int inputY_C = -1, string input = ::userInput);
void gameInfo();
void checkWin(char(&board)[8][8]);
bool checkInput(string input);
std::vector<std::vector<int>> getAvailableMoves(char board[8][8], char player);
std::vector<std::vector<int>> getPLayerLegalMoves(char board[8][8]);
void printLegalMoves(char board[8][8], char player);
bool isLegalMove(char board[8][8], std::vector<std::vector<int>> move_list, char row, char col, char player);
int rowCheck(char board[8][8], int row, int col, char disc);
void flip(char(board)[8][8], int row, int col, char player);


int main()
{
	char board[8][8] = { ' ' };
	std::vector<std::vector<int>> moveList;
	gameInfo();
	startgame(board);
	//checkInput(userInput);
	while (userInput != "Exit" || userInput != "exit")
	{
		moveList = getAvailableMoves(board, 'P');
		printLegalMoves(board, 'P');
		cout << "Please give your input: ";
		while (checkInput(userInput) == false && isLegalMove(board, moveList, userInput[0], userInput[1], 'P') == false)
		{
			moveList = getAvailableMoves(board, 'P');
			getPLayerLegalMoves(board);
			printLegalMoves(board, 'P');
			cout << "The second index of the input: ";
			cout << userInput[1];
			cout << "\n";
			cout << "please enter a legal move: \n";
			cin >> userInput;

		}
		cout << isLegalMove(board, moveList, userInput[0], userInput[1], 'P');

		cout << userInput;
		printBoard(board);

		// Computer's move
		std::vector<std::vector<int>> moveListComputer;
		moveListComputer = getAvailableMoves(board, 'C');
		int randomInt = rand() % (moveListComputer.size() - 1);
		printBoard(board, moveListComputer.at(randomInt)[0], moveListComputer.at(randomInt)[1]);
	}
	
	checkWin(board);

}

void gameInfo() {
	//cout << "Welcome to a text based Reversi game!" << endl;
	//cout << "To learn more about the game, visit https://en.wikipedia.org/wiki/Reversi ." << endl;
	//Sleep(1000);
	//cout << "The game will be played on a 8x8 grid going from a through h horizontally and 1 to 8 vertically" << endl;
	//cout << "YOU will always move first, REVERSIng all the computer's tiles marked 'C' with your tiles, marked 'P'" << endl;
	//Sleep(2000);
	//cout << "In each round, you can give your input by giving the coordinate of the grid, on which you would like to put the tile." << endl;
	//cout << "An input looks like this: 'b4'. If your input is invalid, you will be prompted to give a new input." << endl;
	//Sleep(1000);
	//cout << "To exit the game, simply type in 'exit' whenever you're prompted to give any input." << endl;
	//cout << "If you want to save your game to brag to your friends or load a really awesome game, simply input 'save' or 'load'." << endl;
	//cout << "You will be prompted to give the name of the saved file, which is to be found on the same directory as the game." << endl;
	//Sleep(1000);
	//std::cout << '\n';
	//cout << "Welcome to a text based Reversi game!" << endl;
	//Sleep(1000);
	//std::cout << '\n';
	//cout << "Alright! Ready to start the game? Simply type in 'start' to start REVERSIng to victory!" << endl;

	//std::string word;
	//std::cin >> word;
	////Keep asking if the user is ready to start until he enters 'star'
	//while (word != "START" && word != "start")
	//{

	//	std::cout << "Invalid input: " << word << " Please type 'start' to start Reversi" << std::endl;
	//	std::cin >> word;
	//}

}

// Initialize Game Board 
void startgame(char board[8][8])
{
	std::cout << "   A  B  C  D  E  F  G  H\n";
	for (int i = 0; i < 8; i++)
	{
		std::cout << (i) << "  ";
		for (int j = 0; j < 8; j++)
		{
			board[i][j] = ' -';
			board[3][3] = ' P';
			board[3][4] = ' C';
			board[4][3] = ' C';
			board[4][4] = ' P';

			std::cout << board[i][j] << "  ";
		}
		std::cout << '\n';
	}

}
bool checkInput(string input) {
	cin >> input;
	int inputOkay = 0;  // To check whether user input is acceptable or not. 2 to be acceptable
	if (input == "start" || input == "Start") {
		cout << "True";
		::userInput = input;
		return true;
	}
	else if (input == "exit" || input == "Exit") {
		cout << "True";
		::userInput = input;
		return true;
	}
	else if (input == "save" || input == "Save") {
		cout << "True";
		::userInput = input;
		return true;
	}
	else if (input == "load" || input == "Load") {
		cout << "True";
		::userInput = input;
		return true;
	}
	else {
		char row[] = "abcdefgh";
		char column[] = "12345678";
		for (int i = 0; i < strlen(row); i++) {
			if (input[0] == row[i] || input[1] == column[i]) {  // Check if input is combination of a through h and 1 to 8
				inputOkay++;
				cout << "True";
			}

		}
		if (inputOkay == 2) {
			cout << "True";
			::userInput = input;
			return true;
		}
		cout << "False";
	}

	return false;
}


// Print the new board after move
void printBoard(char newBoard[8][8], int inputX_C, int inputY_C, string input)
{
	std::cout << '\n' << "New Board" << '\n';
	std::cout << "   A  B  C  D  E  F  G  H\n";

	if (inputX_C == -1 || inputY_C == -1)
	{
		// Print user input out on the board
		inputX = string("abcdefgh").find(input[0]);
		inputY = string("01234567").find(input[1]);
		newBoard[inputY][inputX] = 'P';
		flip(newBoard, inputX, inputY, 'P');
	} else
	{
		newBoard[inputY_C][inputX_C] = 'C';
		flip(newBoard, inputX_C, inputY_C, 'C');
	}


	for (int i = 0; i < 8; i++)
	{
		std::cout << (i) << "  ";
		for (int j = 0; j < 8; j++)
		{
			std::cout << newBoard[i][j] << "  ";
		}
		std::cout << '\n';

	}
}


// go through whole board, and count pieces of passed-in player
int checkCurrentResult(char board[8][8], char actor) {
	int total = 0;
	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 8; ++j)
			if (board[i][j] == actor)
				total += 1;

	return total;
}

//  Check and print the checkCurrentResult and the winner
void checkWin(char(&board)[8][8]) {
	int player_total = checkCurrentResult(board, 'P');
	int Computer_total = checkCurrentResult(board, 'C');

	std::cout << "Player total: " << player_total << '\n';
	std::cout << "Computer total: " << Computer_total << '\n';
	if (player_total == Computer_total) {
		std::cout << "TIE GAME\n";
		return;
	}

	std::cout << ((player_total > Computer_total) ? "Player" : "Computer") << " wins!\n";

}

// Check if the character in the trajectory is the opponent's pieces followed by player's piece
// Code from https://github.com/drohh/othello
bool checkTrajectory(char board[8][8], char charInPos, int row, int col, int trajectory[], char player)
{
	char otherPlayer = (player == 'P') ? 'C' : 'P';
	if (charInPos == otherPlayer) {
		// save spot's row and col #
		int curr_row = row + trajectory[0];
		int curr_col = col + trajectory[1];

		//continue along this delta trajectory until you stop seeing the opponent's pieces
		while (charInPos == otherPlayer) {
			curr_row += trajectory[0];
			curr_col += trajectory[1];

			// check to see if new position is off board
			if (curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_row < 0)
				break;

			// save the next character
			charInPos = board[curr_row][curr_col];
		}

		// if the player's piece is seen after one (+more) of the opponent's pieces, the original move is a flippable one
		if (charInPos == player)
			return true;
	}
	return false;
}

// Return a list of position to be flipped
std::vector<std::vector<int>> listFlip(char(board)[8][8], int row, int col, char player, int trajectory[], std::vector<std::vector<int>> discs_to_flip)
{
	char otherPlayer = (player == 'P') ? 'C' : 'P';
	// save current position
	row = row + trajectory[0];
	col = col + trajectory[1];
	player = board[col][row];

	// traverse over the opponent's pieces, while saving the positions to the big list to be flipped later
	while (player == otherPlayer) {
		std::cout << "flipping [" << row << ", " << col << "]\n";
		std::vector<int> disc = { col, row };
		discs_to_flip.push_back(disc);
		row += trajectory[0];
		col += trajectory[1];

		// save next character
		player = board[col][row];
	}
	return discs_to_flip;
}

// TODO: refactor this function into separate functions!
// flips appropriate pieces after a disc is placed down (called after verifying the move isFlippable)
void flip(char(board)[8][8], int row, int col, char player) {
	// declare a list of positions of discs that will be flipped
	// e.g. {{0,1}, {0,2}} means disc at location board[0][1] & board[0][2] will be flipped
	std::vector<std::vector<int>> discs_to_flip;

	char otherPlayer = (player == 'P') ? 'C' : 'P';

	// for every delta representing a neighboring position...
	for (auto deltas : surroundingPosDeltas) {
		// save what row/col currently on
		int curr_row = row + deltas[0];
		int curr_col = col + deltas[1];

		// ignore if this goes off of the board
		if (curr_row > 7 || curr_row < 0 || curr_col > 7 || curr_col < 0)
			continue;

		// save character in this position
		char char_in_pos = board[curr_col][curr_row];

		// use this variable to save whether or not a line of pieces should be flipped
		bool flip_this_direction = false;

		// if we found out we should be flipping...
		if (checkTrajectory(board, char_in_pos, row, col, deltas, player)) {
			discs_to_flip = listFlip(board, row, col, player, deltas, discs_to_flip);
		}
	}

	// after we've collecting the row/col of all discs to flipped, flip them to the current player's color/character
	for (auto pos : discs_to_flip)
		board[pos[0]][pos[1]] = player;
}


// Check if placing a piece at [x,y] in the board with flip one (or more) of the opponent's pieces
// Code from https://github.com/drohh/othello
bool isFlippable(char board[8][8], int row, int col, char player) {
	char otherPlayer = (player == 'P') ? 'C' : 'P';

	// for every delta of the surrounding positions
	for (auto deltas : surroundingPosDeltas) {

		// skip if the position is off of game board
		if (row + deltas[0] > 7 || row + deltas[0] < 0 || col + deltas[1] > 7 || col + deltas[1] < 0) {
			continue;
		}

		char char_in_pos = board[row + deltas[0]][col + deltas[1]]; // grab the character in that spot

		if (checkTrajectory(board, char_in_pos, row, col, deltas, player)) return true;
	}
	// if no flippable spot is found after checking all surrounding positions, the original move is not a flippable one
	return false;
}

bool isLegalMove(char board[8][8], std::vector<std::vector<int>> move_list, char row, char col, char player) {
	std::string rows = "abcdefgh";
	std::string cols = "01234567";
	int row_int = rows.find(row);
	int col_int = cols.find(col);
	std::vector<int> proposedMove = {  row_int, col_int};
	//This error should NOT occur, as the regex pattern validates the user's input
	if (row_int > 7 || row_int < 0 || col_int > 7 || col_int < 0)
		//throw std::range_error{ "isLegalMove()" };
		return false;


	// Make sure position is empty
	if (board[row_int][col_int] != '-') {
		cout << row_int << " and " << col_int << " are not legal.\n";
		return false;
	}

	if (std::find(move_list.begin(), move_list.end(), proposedMove) != move_list.end()) {
		cout << endl << row_int << " and " << col_int << " are available.\n";
		return true;
	}
	//cout << row_int << " and " << col_int << " are not legal.\n";
	return false;
}

std::vector<std::vector<int>> getAvailableMoves(char board[8][8], char player) {

	std::vector<std::vector<int>> move_list;

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {

			// Verify if the Spot is empty
			if (board[i][j] == '-') {

				// check to see if placing a piece there will flip one (+more) of the opponent's pieces
				if (isFlippable(board, i, j, player)) {

					// if so, create a 2-element vector representative of the move and push it to the big move list
					std::vector<int> move = { j, i };
					move_list.push_back(move);
				}

			}
		}
	}


	return move_list;
}

int rowCheck(char board[8][8], int row, int col, char disc) {
	int discFlipped = 0;

	if (board[row][col] != '- ')
		return 0;

	board[row][col] = disc;

	char opponentDisc;
	if (disc == 'P')
		opponentDisc = 'C';
	else
		opponentDisc = 'P';
	//Check are there discs on the right side to flip
	int disc_pos = -1;
	for (int c = col + 1; c < 8 && board[row][c] != ' ' && disc_pos == -1; c++)
	{
		if (board[row][c] == disc)
			disc_pos = c;

	}

	//Check are there discs on the left side to flip
	disc_pos = -1;
	for (int c = col - 1; c >= 0 && board[row][c] != ' ' && disc_pos == -1; c--)
	{
		if (board[row][c] == disc) {
			disc_pos = c;

		}
	}


}

std::vector<std::vector<int>> getPLayerLegalMoves(char board[8][8]) {
	return getAvailableMoves(board, 'P');
}

void printLegalMoves(char board[8][8], char player) {
	string col = "abcdefgh";
	string input = "";
	if (player == 'P') {
		std::cout << "\n Player legal moves:\n";
		auto v = getPLayerLegalMoves(board);
		for (const auto& vec : v) {
			std::cout << "(" << col.at(vec[0]) << "," << vec[1] << ")  ";
			//input.push_back(col.at(vec[0]));
			//input.push_back(vec[1]);
			//::legalInputs->append(input);
			//input = "";
		}
		std::cout << std::endl;
	}
}