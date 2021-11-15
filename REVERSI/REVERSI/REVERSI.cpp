/**
 * @file REVERSI.cpp
 * @author  Michael Reno, Leticia Atonlieu, Mitterand Assagui, Mohamed Rouissi (Team 1C)
 * @brief   The all-in-one logic for a simple text based Reversi game
 * @version 0.1
 * @date 2021-11-15
 *
 * For more info about Reversi, check out https://en.wikipedia.org/wiki/Reversi
 */


#include <iostream>
#include <stdexcept>
#include <array>
#include <vector>
#include <regex>
#include <stdlib.h>
#include <windows.h>

using namespace std;

std::string userInput = "start";  // Store user input
std::string legalInputs[];  // Store all legal inputs
int inputX = 0;  // Store the x-coordinate
int inputY = 0;  // Store the y-coordinate
// Check all 8 surround positions
int surroundingPosDeltas[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, // 3 positions above
									{0, -1}, {0, 1}, // 2 positions on same row
								  {1, -1}, {1, 0}, {1, 1} }; // 3 positions below


// Functions
// Comments are placed on each function.
void startgame(char board[8][8]);
void printBoard(char newBoard[8][8], int inputX_C = -1, int inputY_C = -1, string input = ::userInput);
void gameInfo();
void checkWin(char(&board)[8][8]);
bool checkInput(string input);
std::vector<std::vector<int>> getAvailableMoves(char board[8][8], char player);
std::vector<std::vector<int>> getPLayerLegalMoves(char board[8][8], char player);
void printLegalMoves(char board[8][8], char player);
bool isLegalMove(char board[8][8], std::vector<std::vector<int>> move_list, char row, char col, char player);
void flip(char(board)[8][8], int row, int col, char player);


int main()
{
	char board[8][8] = { ' ' };
	std::vector<std::vector<int>> moveList;
	gameInfo();
	startgame(board);
	//checkInput(userInput);  // Only for debugging purposes.
	while (userInput != "Exit" || userInput != "exit")
	{
		moveList = getAvailableMoves(board, 'P');
		printLegalMoves(board, 'P');
		cout << "Please give your input: ";
		cin >> userInput;
		if (userInput == "Exit" || userInput == "exit") break;
		while (checkInput(userInput) == false && isLegalMove(board, moveList, userInput[0], userInput[1], 'P') == false)
		{
			moveList = getAvailableMoves(board, 'P');
			try
			{
				getPLayerLegalMoves(board, 'P');
				printLegalMoves(board, 'P');
				cout << "The second index of the input: ";
				cout << userInput[1];
				cout << "\n";
				cout << "please enter a legal move: \n";
				cin >> userInput;
			}
			catch (exception e)
			{
				cout << "\nYou don\'t have any legal move at the moment. Computer will move again.\n";
				continue;
			}

		}
		cout << isLegalMove(board, moveList, userInput[0], userInput[1], 'P');

		cout << userInput;
		printBoard(board);

		// Computer's move
		std::vector<std::vector<int>> moveListComputer;
		moveListComputer = getAvailableMoves(board, 'C');
		printLegalMoves(board, 'C');

		try
		{
			int randomInt = rand() % (moveListComputer.size() - 1);
			cout << "\nUsing random move number " << randomInt << endl;
			printBoard(board, moveListComputer.at(randomInt)[0], moveListComputer.at(randomInt)[1]);
		}
		catch (exception e)
		{
			cout << "\nComputer does not have any legal move. Please continue.\n";
			continue;
		}
		

	}
	
	checkWin(board);

}

/**
 * Give a brief explanation on how to start and interact with the CLI game
 * The game will only start after "start" is given as an input.
 */
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


/**
 * Initialize game board with two P blocks and two C blocks in the middle
 * param: board, a 2D char array that stores and replicate the board. Board[y][x].
 */
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

/**
 * Handling user input and parse it to check if the input follows the correct Regex.
 * param: input, a string containing user input
 * return: true, if input is acceptable; false, if not.
 */
bool checkInput(string input) {
	//cin >> input;
	input == ::userInput;
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


/**
 * Print the new board with the input from both player and computer
 * and also call the flip() function to check if a block will be switched.
 * param: newBoard, a 2D array containing the game board.
 * param: inputX_C, an int containing x-coordinate of computer's move. Standard: -1
 * param: inputY_C, an int containing y-coordinate of computer's move. Standard: -1
 * param: input, a string containing player's input according to the regex. Standard: ::userInput
 */
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


/**
 * Go through the whole board and count pieces of both P and C.
 * param: board, a 2D array that replicate the game board.
 * param: actor, a char that defines which player pieces is to be counted.
 * return: int, how many pieces the actor has in the board.
 */
int checkCurrentResult(char board[8][8], char actor) {
	int total = 0;
	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 8; ++j)
			if (board[i][j] == actor)
				total += 1;

	return total;
}


/**
 * Check and print the number of pieces each player has and declare the winner.
 * param: board, a 2D array that replicate the game board.
 */
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


/**
 * Check if the character in the trajectory is the opponent's pieces and then followed by player's piece.
 * Code from https://github.com/drohh/othello
 * param: board, a 2D array that replicate the game board.
 * param: charInPos, a char that declares the character at a position. Can only be P, C or -.
 * param: row, int stating the row coordinate in the board.
 * param: col, int stating the column coordinate in the board.
 * param: trajectory, an int array with all the neighboring coordinates.
 * param: player, a char declaring the character at play.
 * return: true, if opponent's pieces before player's piece is found; false, if not.
 */
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


/**
 * Calculate and return a list of position(s) to be flipped later on
 * param: board, a 2D array that replicate the game board.
 * param: row, int stating the row coordinate in the board.
 * param: col, int stating the column coordinate in the board.
 * param: player, a char declaring the character at play.
 * param: trajectory, an int array with all the neighboring coordinates.
 * param: discs_to_flip, an int 2D vector to store the coordinates of pieces to be flipped.
 * return: a 2D int vector containing coordinates of pieces to be flipped.
 */
std::vector<std::vector<int>> listFlip(char(board)[8][8], int row, int col, char player, int trajectory[], std::vector<std::vector<int>> discs_to_flip)
{
	char otherPlayer = (player == 'P') ? 'C' : 'P';
	// save current position
	row = row + trajectory[0];
	col = col + trajectory[1];
	player = board[col][row];

	// traverse over the opponent's pieces, while saving the positions to the big list to be flipped later
	while (player == otherPlayer) {
		//std::cout << "flipping [" << row << ", " << col << "]\n";  // Only for debugging process
		std::vector<int> disc = { col, row };
		discs_to_flip.push_back(disc);
		row += trajectory[0];
		col += trajectory[1];

		// save next character
		player = board[col][row];
	}
	return discs_to_flip;
}


/**
 * Flip the appropriate pieces after a piece is placed down. (Will be called after verifying that the move isFlippable).
 * param: board, a 2D array that replicate the game board.
 * param: row, an int containing the row coordinate of the piece to be checked.
 * param: col, an int containing the column coordinate of the piece to be checked.
 * param: player, a char declaring the player at play.
 */
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


/**
* Check if placing a piece at [x,y] in the board with flip one (or more) of the opponent's pieces
* Code from https://github.com/drohh/othello
* param: board, a 2D array that replicate the game board.
* param : row, int stating the row coordinate in the board.
* param : col, int stating the column coordinate in the board.
* param : player, a char declaring the character at play.
*/
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


/**
* Check if the coordinate given as input depicts a legal move according to the game's rules or not.
* Code from https://github.com/drohh/othello
* param: board, a 2D array that replicate the game board.
* param: move_list, a 2D int vector that stores all legal moves at the time.
* param: row, int stating the row coordinate in the board.
* param: col, int stating the column coordinate in the board.
* param: player, a char declaring the character at play.
* return: true, if the input is included in the list of legal moves; false, if not.
*/
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


/**
* Get all available moves for the given player at a time.
* Code from https://github.com/drohh/othello
* param: board, a 2D array that replicate the game board.
* param: player, a char declaring the character at play.
* return: a 2D int vector, that stores all available moves to the player at the time.
*/
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

/**
* Get a player's legal moves
* param: board, a 2D array that replicate the game board.
* param: player, a char declaring the character at play.
* return: a 2D int vector containing all legal moves of the player at a time.
*/
std::vector<std::vector<int>> getPLayerLegalMoves(char board[8][8], char player) {
	return getAvailableMoves(board, player);
}


/**
* Print a player's legal moves to the console.
* param: board, a 2D array that replicate the game board.
* param: player, a char declaring the character at play.
*/
void printLegalMoves(char board[8][8], char player) {
	string col = "abcdefgh";
	string input = "";
	string role = (player == 'P') ? "Player" : "Computer";
	std::cout << "\n " << role << " legal moves:\n";
	auto v = getPLayerLegalMoves(board, player);
	for (const auto& vec : v) {
		std::cout << "(" << col.at(vec[0]) << "," << vec[1] << ")  ";
	}
	std::cout << std::endl;
}