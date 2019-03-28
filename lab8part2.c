/* Ian Kwok-Tao Chan 1005211263
 * DESCRIPTION: Program that uses a 'greedy algorithm' to identify and
 * play the best scoring position in a game of Reversi.
 *
 * CURRENT STATUS: Configured so that the user plays against my AI, rather
 * than against the provided AI. Therefore, remember to revert back to
 * int for col and row when playing against provided AI.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "lab8part2lib.h"
#define MAX 26
#define OFFSET 97

//board initialization and printing
int  getSize(int n);
char initializeBoard(char board[MAX][MAX], int size);
char printBoard(char board[MAX][MAX], int n);
char boardConfig(char board[MAX][MAX]); //KEEP FOR TESTING AND DEBUGGING

//computer ai
void computerMove(char board[MAX][MAX], int n, char computer);
int  checkScore(char board[MAX][MAX], int n, char colour, int row, int col, int deltaRow, int deltaCol);
bool avoidPosition(char board[MAX][MAX], int n, int row, int col, char colour);
bool occupied(char board[MAX][MAX], int row, int col, char colour);

//validation
bool positionInBounds(int n, int row, int col);
bool checkLegalInDirection(char board[MAX][MAX], int n, int row, int col, char colour, int deltaRow, int deltaCol);
bool moveValidation(char board[MAX][MAX], int n, char row, char col, char colour, char computer); //KEEP FOR TESTING AND DEBUGGING

//tile flipping
void flipTiles(char board[MAX][MAX], int n, char colour, int row, int col, int deltaRow, int deltaCol);
bool validateAndFlip(char board[MAX][MAX], int n, char colour, int row, int col);

//end game
bool checkGameState(char board[MAX][MAX] ,int n);
bool availableMoves(char board[MAX][MAX], int n, char player);
void winCounter(char board[MAX][MAX], int n);
bool checkBoardStatus(char board[MAX][MAX], int n);

int main(){
    int n = 0;
    char colour = '0';
    int row = 0, col = 0;
    char board[MAX][MAX] = {};
    char computer = '0';
  
    //gets board size and initializes board
    n = getSize(n);
    initializeBoard(board,n);
  
    //prompts which colour my AI will play
    printf("Ian plays (B/W): ");
    scanf(" %c", &computer);
    printBoard(board,n);
  
    int go;
  
    //CURRENTLY PLAYING ANNOUNCEMENT
    printf("You are currently playing against the ");
//    printf("smarter AI\n");
    printf("smartest AI\n");
    printf("Enter any key to begin \n");
    scanf("%d", &go);
  
    bool over = false; //game loop
  
    //IF MY AI IS BLACK ---------------------------------------------------
    if (computer == 'B'){
        colour = 'W';
        while(!over) {
                  
            //MY AI'S TURN ---------------------------------------------------
            //checks if my ai has available moves
            if(availableMoves(board,n,computer)){
                computerMove(board,n,computer);
                printBoard(board,n);
            }
          
            //if no other moves exist for either player, end game. Otherwise continue.
            else if(!availableMoves(board, n, computer)) {
                if (checkGameState(board, n)) {
//                    winCounter(board, n);
                    over = true;
//                    return 0;
                }
              
                if (availableMoves(board, n, colour)){
                    printf("%c player has no available move.\n", computer);
//                    winCounter(board, n);
//                    return 0;
                }         
            }
                    
            //OPPONENT'S TURN ---------------------------------------------------
            //checks if opponent has available moves
            if(availableMoves(board, n, colour)){
              
//            printf("Enter move for colour %c (RowCol): ", colour);
//            scanf(" %c%c", &row, &col);
//            if (!moveValidation(board, n, row, col, colour,computer)) return 0;
          
            //COMMENTED OUT SO I CAN PLAY AGAINST MY OWN AI
                findSmartestMove(board, n, colour, &row, &col);
                printf("Testing AI places %c at %c%c.\n", colour, row + 'a', col + 'a');
                validateAndFlip(board, n, colour, row, col);
                board[row][col]=colour;
                printBoard(board,n);
            }
                     
            //if no other moves exist for either player, end game. Otherwise continue.
            else if(!availableMoves(board, n, colour)) {
                if (checkGameState(board, n)) {
                    over = true;
//                    winCounter(board,n);
                }
              
                if (availableMoves(board, n, computer)){
                    printf("%c player has no available move.\n", colour);
//                    winCounter(board,n);
//                    return 0;
                }      
            }
        }
    }
  
    //IF MY AI IS WHITE ---------------------------------------------------
    if (computer == 'W'){
        colour = 'B';
        while(!over) {
          
            //OPPONENT'S TURN ---------------------------------------------------
            //checks if opponent has available moves
            if(availableMoves(board,n,colour)){
          
//            printf("Enter move for colour %c (RowCol): ", colour);
//            scanf(" %c%c", &row, &col);
//            if (!moveValidation(board, n, row, col, colour,computer)) return 0;
          
            //COMMENTED OUT SO I CAN PLAY AGAINST MY OWN AI
                findSmartestMove(board, n, colour, &row, &col);
                printf("Testing AI places %c at %c%c.\n", colour, row + 'a', col + 'a');
                validateAndFlip(board, n, colour, row, col);
                board[row][col]=colour;
                printBoard(board,n);
            }
          
            //if no other moves exist for either player, end game. Otherwise continue.
            else if(!availableMoves(board, n, colour)) {
                if (checkGameState(board, n)) {
                    over = true;
//                    winCounter(board,n);
                }
              
                if (availableMoves(board, n, computer)){
                    printf("%c player has no available move.\n", colour);
//                    winCounter(board,n);
//                    return 0;
                }      
            }
          
            //MY AI'S TURN ---------------------------------------------------
            //checks if my ai has available moves
            if(availableMoves(board,n,computer)){
                computerMove(board,n,computer);
                printBoard(board,n);
            }
          
            //if no other moves exist for either player, end game. Otherwise continue.
            else if(!availableMoves(board, n, computer)) {
                if (checkGameState(board, n)) {
//                    winCounter(board, n);
                    over = true;
//                    return 0;
                }
              
                if (availableMoves(board, n, colour)){
                    printf("%c player has no available move.\n", computer);
//                    winCounter(board, n);
//                    return 0;
                }         
            }
        }
    }
  
    winCounter(board,n);
  
    return 0;
  
}

int getSize(int n){     //KEEP
  
    //gets board size
    printf("Enter the board dimension: ");
    scanf("%d", &n);
  
    return n; 
}

char initializeBoard(char board[MAX][MAX], int n){      //KEEP

    //initializes the U array
    for(int i = 0; i<n; i++){
        for(int j = 0;j<n; j++){
            board[i][j] = 'U';
            if(((i+1 == n/2) && (j+1 == n/2)) || ((i+1 == n/2+1) && (j+1 == n/2 + 1))) board[i][j] = 'W';
            if(((i+1 == n/2) && (j+1 == n/2+1)) || ((i+1 == n/2+1) && (j+1 == n/2))) board[i][j] = 'B';
        }
    }
    return board[MAX][MAX];
}

char boardConfig(char board[MAX][MAX]){     //KEEP FOR TESTING AND DEBUGGING
  
    //sets the initial board configuration
    char colour, row, col;
    bool cont = true;
    printf("Enter board configuration: \n");
    while(cont){
        scanf(" %c%c%c", &colour, &row, &col);
        int i = row - OFFSET;int j = col - OFFSET;
        board[i][j] = colour;
        if (colour == '!' && i == -64 && j == -64) cont = false;
    }
    return board[MAX][MAX];
  
}

char printBoard(char board[MAX][MAX], int n){       //KEEP

    //prints the first row
    printf("  ");
    for(int i = 0;i<n;i++){
        printf("%c", 'a' + i);
    }
    printf("\n");
  
    //prints the board array and the first column
    for(int i = 0; i < n; i++){
        printf("%c ", 'a' + i);
        for(int j = 0;j <n;j++){
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
    return 0;
  
}

bool positionInBounds(int n, int row, int col){     //KEEP
    if (row > n-1 || col > n -1 || row < 0 || col < 0) {
        return false;
    } else {
        return true;
    }
}

bool checkLegalInDirection(char board[MAX][MAX], int n, int row, int col, char colour, int deltaRow, int deltaCol){     //KEEP
  
    //this function will now be used to validate a position of play for my ai
  
    //checking if the position has already been played (invalid)
    if (board[row][col] !='U') return false;
  
    //checking if adjacent one is the same colour (invalid)
    if(board[row+deltaRow][col+deltaCol] == colour) return false;
  
    //checking if adjacent one is empty (invalid)
    if(board[row+deltaRow][col+deltaCol] == 'U') return false;
  
    //checking if out of bounds (invalid)
    if(!positionInBounds(n,row+deltaRow,col+deltaCol)) return false;
      
    //checking if there is eventually an existing colour as the one being played down the line (valid)
    while(board[row + deltaRow][col + deltaCol] != colour){         //as long as the next tile is not the player's tile
      
        if(deltaRow!=0){
            if(deltaRow<0) deltaRow--;
            else deltaRow++;
        }
      
        if(deltaCol!=0){
            if(deltaCol<0) deltaCol--;
            else deltaCol++;
        }
      
        if(!positionInBounds(n,row+deltaRow,col+deltaCol)){         //checks to see if within board
            return false;
        }
      
        if(board[row + deltaRow][col + deltaCol] == colour){        //aligns with the correct colour
            return true;                              
        }
        if (board[row + deltaRow][col + deltaCol] == 'U'){          //not a capture
            return false;                             
        }
    }
    return false;
}

bool moveValidation(char board[MAX][MAX], int n, char row, char col, char colour, char computer){      //KEEP FOR TESTING AND DEBUGGING
  
    //originally function that took in chars (from main)
    //this function validates human moves
    //this function is not called upon in any other place
    //likely be removed from program, assuming that the other AI makes valid moves  
  
    bool valid = false;
  
    if(checkLegalInDirection(board, n, row-OFFSET, col-OFFSET, colour, 1, 0)) {
        flipTiles(board, n, colour, row-OFFSET, col-OFFSET, 1, 0);
        valid = true;
    }
    if(checkLegalInDirection(board, n, row-OFFSET, col-OFFSET, colour, -1, 0)) {
        flipTiles(board, n, colour, row-OFFSET , col-OFFSET, -1, 0);
        valid = true;
    }
    if(checkLegalInDirection(board, n, row-OFFSET, col-OFFSET, colour, 0, 1)) {
        flipTiles(board, n, colour, row-OFFSET , col-OFFSET, 0, 1);
        valid = true;
    }
    if(checkLegalInDirection(board, n, row-OFFSET, col-OFFSET, colour, 0, -1)) {
        flipTiles(board, n, colour, row-OFFSET , col-OFFSET, 0, -1);
        valid = true;
    }
  
    if(checkLegalInDirection(board, n, row-OFFSET, col-OFFSET, colour, -1, -1)) {
        flipTiles(board, n, colour, row-OFFSET, col-OFFSET, -1, -1);
        valid = true;

    }
    if(checkLegalInDirection(board, n, row-OFFSET, col-OFFSET, colour, 1, 1)) {
        flipTiles(board, n, colour, row-OFFSET, col-OFFSET, 1, 1);
        valid = true;
    }
    if(checkLegalInDirection(board, n, row-OFFSET, col-OFFSET, colour, 1, -1)) {
        flipTiles(board, n, colour, row-OFFSET ,col-OFFSET, 1, -1);
        valid = true;
    }
    if(checkLegalInDirection(board, n, row-OFFSET, col-OFFSET, colour, -1, 1)) {
        flipTiles(board, n, colour, row-OFFSET ,col-OFFSET, -1, 1);
        valid = true;
    }
    
    if(valid){
        board[row-OFFSET][col-OFFSET] = colour;
        return true;
    }
    else {
        printf("Invalid move.\n%c player wins.\n", computer);
        return false;
    }
//    exit program if invalid move played (automatic loss)  
}

void flipTiles(char board[MAX][MAX], int n, char colour, int row, int col, int deltaRow, int deltaCol){    //KEEP
  
    //function to flip tiles
  
    while(board[row+deltaRow][col+deltaCol] != colour && positionInBounds(n, row+deltaRow, col+deltaCol)){
      
        //flips the tiles to colour in question
        board[row+deltaRow][col+deltaCol] = colour;
      
        //logic: if the delta value is not 0, then increment or decrement as necessary
      
        if(deltaRow!=0){
            if(deltaRow<0) deltaRow--;
            else deltaRow++;
        }
      
        if(deltaCol!=0){
            if(deltaCol<0) deltaCol--;
            else deltaCol++;
        }
    }
}

bool validateAndFlip(char board[MAX][MAX], int n, char colour, int row, int col){
   
    //evaluates if the position is valid. If so, then flip for each valid direction.
   
    bool flipped = false;
   
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if (checkLegalInDirection(board, n, row, col, colour, i, j)) {
                flipTiles(board, n, colour, row, col, i, j);
                flipped = true;
            }
        }
    }
   
    if(flipped) return true;
    else return false;
}

int checkScore(char board[MAX][MAX], int n, char colour, int row, int col, int deltaRow, int deltaCol){     //same as flipTile function
  
    //same as flipTile function, but increments a score counter instead (greedy algorithm)
  
    int score = 0;

    if (!checkLegalInDirection(board,n,row,col,colour,deltaRow,deltaCol)){
        return 0;
    }
  
    while(board[row+deltaRow][col+deltaCol] != colour){
             
        score++;
      
        if(deltaRow!=0){
            if(deltaRow<0) deltaRow--;
            else deltaRow++;
        }
      
        if(deltaCol!=0){
            if(deltaCol<0) deltaCol--;
            else deltaCol++;
        }
    }
  
    return score;
}

void computerMove(char board[MAX][MAX], int n, char computer){      //KEEP (MAIN AI COMPONENT)
   
    //ai component
 
    int highScore = 0, score = 0; //highest score of board and of cell
    int rowPlay = 0, colPlay = 0; //this refers to the row, col of the highest scoring cell  
    bool played = false; //refers to whether move has been played

    //STRATEGIES: first check for corners
   
    //first, check for hotspots which I should prioritize (optimized for 8x8)
    //the following code runs the directional checks for each corner, to check if they are playable
    //interestingly, my ai plays better when optimized for corners as WHITE and not BLACK
   
    //the loop will check each corner and if valid, play it and exit loop
    if (!played && computer == 'W') {
        for(int i = -1; i < 2; i++){
            for(int j = -1; j < 2; j++){                          
//                printf("start - ");
                //top left
                if (checkLegalInDirection(board, n, 0, 0, computer, i, j) && !occupied(board, 0, 0, computer) && !played) {
                    rowPlay = 0; colPlay = 0;
                    printf("Corner 1 activated\n");
                    played = true;
                }
                //top right
                if (checkLegalInDirection(board, n, 0, 7, computer, i, j) && !occupied(board, 0, 7, computer) && !played) {
                    rowPlay = 0; colPlay = 7;
                    printf("Corner 2 activated\n");
                    played = true;
                }
                //bottom left
                if (checkLegalInDirection(board, n, 7, 0, computer, i, j) && !occupied(board, 7, 0, computer) && !played) {
                    rowPlay = 7; colPlay = 0;
                    printf("Corner 3 activated\n");
                    played  = true;
                }
                //bottom right
                if (checkLegalInDirection(board, n, 7, 7, computer, i, j) && !occupied(board, 7, 7, computer) && !played) {
                    rowPlay = 7; colPlay = 7;
                    printf("Corner 4 activated\n");
                    played = true;
                }
//                printf(" end\n");

                //if a position has been played, break
                if(played) break;
              
            }
        }
    }

    //GREEDY ALGORITHM: check to see if each position is valid and calculate score accordingly
    //STRATEGIES: check to see if corrners have been played. If yes, then play the surrounding corners.
   
    if (!played) {
        printf("Playing greedy - avoid spots\n");
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
              
                //optimized if my ai plays white
                //this one works extremely well for both smarter and smartest (large margins of victory)
                if(computer == 'W'){
                    //directional checks using for loops
                    for(int a = -1; a <= 1; a++){
                        for(int b = -1; b <= 1; b++){              

                            if(checkLegalInDirection(board, n, i, j, computer, a, b) && !avoidPosition(board, n, i, j, computer)){
                                score += checkScore(board, n, computer, i, j, a, b);
                                played = true;
                            }
                  
                            /*Now it knows the highest scoring cell and its position on the board. It keeps a current running
                            * highscore of the best position on the board. I must reset score to 0 otherwise on the next loop,
                            * it will just add to the prior score. Also must remember the row and col of the best position*/

                            if (score >= highScore){
            //                    printf("%d,%d\n", rowPlay, colPlay);
                                highScore = score;
                                score = 0;        
                                rowPlay = i;      
                                colPlay = j;
                            }
      
                        }
                    }
                }
              
              
                //optimized if my ai plays black
                //not really optimized, I have to rethink the way I'm keeping track of the score! (results in loss against hardest)
                if(computer == 'B'){
                    //directional checks using for loops
                    for(int a = -1; a <= 1; a++){
                        for(int b = -1; b <= 1; b++){              

                            if(checkLegalInDirection(board, n, i, j, computer, a, b) && !avoidPosition(board, n, i, j, computer)){
                                score += checkScore(board, n, computer, i, j, a, b);
                                played = true;
                            }
                           
                            /*Now it knows the highest scoring cell and its position on the board. It keeps a current running
                            * highscore of the best position on the board. I must reset score to 0 otherwise on the next loop,
                            * it will just add to the prior score. Also must remember the row and col of the best position*/
                           
                            if (score >= highScore){
                                highScore = score;
                                score = 0;        
                                rowPlay = i;      
                                colPlay = j;
                            }
                       
                        }
                    }

      
                }
            }
        }

    }
   
    //if no other move is available, I must eventually play something else (can't avoid)
   
    if (!played) {
        printf("Playing greedy - can't avoid\n");
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
              
                //optimized if my ai plays white
                //this one works extremely well for both smarter and smartest (large margins of victory)
                if(computer == 'W'){
                    //directional checks using for loops
                    for(int a = -1; a <= 1; a++){
                        for(int b = -1; b <= 1; b++){              

                            if(checkLegalInDirection(board, n, i, j, computer, a, b)){
                                score += checkScore(board, n, computer, i, j, a, b);
                                played = true;
                            }
                  
                            /*Now it knows the highest scoring cell and its position on the board. It keeps a current running
                            * highscore of the best position on the board. I must reset score to 0 otherwise on the next loop,
                            * it will just add to the prior score. Also must remember the row and col of the best position*/

                            if (score >= highScore){
            //                    printf("%d,%d\n", rowPlay, colPlay);
                                highScore = score;
                                score = 0;        
                                rowPlay = i;      
                                colPlay = j;
                            }
      
                        }
                    }
                }
              
              
                //optimized if my ai plays black
                //not really optimized, I have to rethink the way I'm keeping track of the score! (results in loss against hardest)
                if(computer == 'B'){
                    //directional checks using for loops
                    for(int a = -1; a <= 1; a++){
                        for(int b = -1; b <= 1; b++){              

                            if(checkLegalInDirection(board, n, i, j, computer, a, b)){
                                score += checkScore(board, n, computer, i, j, a, b);
                                played = true;
                            }
                           
                            /*Now it knows the highest scoring cell and its position on the board. It keeps a current running
                            * highscore of the best position on the board. I must reset score to 0 otherwise on the next loop,
                            * it will just add to the prior score. Also must remember the row and col of the best position*/
                           
                            if (score >= highScore){
                                highScore = score;
                                score = 0;        
                                rowPlay = i;      
                                colPlay = j;
                            }
                       
                        }
                    }     
                }
            }
        }
    }
   
    //now just flip tiles, if it is in legal direction
    if (validateAndFlip(board, n, computer, rowPlay, colPlay)) {
        printf("Ian places %c at %c%c.\n", computer, rowPlay + OFFSET, colPlay + OFFSET);
        printf("%d,%d\n", rowPlay, colPlay);
        board[rowPlay][colPlay] = computer;
    }
}  

bool availableMoves(char board[MAX][MAX], int n, char player){       //KEEP
  
    //function that evaluates if there are existing moves for a particular colour
  
    //tests for either White or Black
    for(int a = 0; a < n; a++){
        for(int b = 0; b < n; b++){
          
            for(int i = -1; i <= 1; i++){
                for(int j = -1; j <= 1; j++){
                  
                    if (checkLegalInDirection(board, n, a, b, player, i, j)) return true;
        
                }
            }
        }
    }
  
    return false;
}

bool checkGameState(char board[MAX][MAX], int n){       //REUSING CODE FROM AVAILABLE MOVES FUNCTION
  
    bool over = true;
  
    //this function checks the current game state. False for game still running, true for game over
  
    //the following loops evaluate each position on the board to test if they are playable
    //for both colours
  
    //for white
    for(int a = 0; a < n; a++){
        for(int b = 0; b < n; b++){
            //direction for loops
            for(int i = -1; i <= 1; i++){
                for(int j = -1; j <= 1; j++){
                  
                    if (checkLegalInDirection(board, n, a, b, 'W', i, j)) over = false;
        
                }
            }
        }
    }
  
    //for black
    for(int a = 0; a < n; a++){
        for(int b = 0; b < n; b++){
            //direction for loops
            for(int i = -1; i <= 1; i++){
                for(int j = -1; j <= 1; j++){
                  
                    if (checkLegalInDirection(board, n, a, b, 'B', i, j)) over = false;
        
                }
            }
        }
    }
 
    if (over == true) {
        return true;
    }
    else return false;
}

bool checkBoardStatus(char board[MAX][MAX], int n){     //DESTROY DIS SHIT
  
    //returns true for full board over, false otherwise
  
    for(int i = 0;i<n;i++){
        for(int j = 0;j<n;j++){
            if (board[i][j] == 'U') return true;
        }
    }
  
    return false;
}

void winCounter(char board[MAX][MAX], int n){
  
    //this function counts the number of tiles for each colour and returns the winner
  
    int black = 0;
    int white = 0;
    char winner = 0;
  
    for(int i = 0;i<n;i++){
        for(int j = 0;j<n;j++){
            if (board[i][j] == 'W') white++;
            if (board[i][j] == 'B') black++;
        }
    }
  
    if(black!=white){
        if(black>white) winner = 'B';
        if(white>black) winner = 'W';
        printf("%c player wins.\n", winner);
        printf("Black: %d\nWhite: %d\n", black, white);
    } else if(black==white) {
        printf("Draw!\n");
    }
  
}

bool avoidPosition(char board[MAX][MAX], int n, int row, int col, char colour) {
  
    //function that evaluates positions to avoid. where true is avoid!
    //NOTE: I WILL HAVE TO CHANGE THIS FUNCTION HERE TO ACCOMODATE IF THE SPECIFIC CORNER HAS BEEN PLYAED - done
  
    //these two statements check for upper corner avoid hotspots
  
    //this is top left
    if((row == 0 && col == 1) || (row == 1 && (col == 0 || col == 1))) {
        if(occupied(board, 0, 0, colour)) return false;
        return true;
    }
    //this is top right
    if((row == 0 && col == 1) || (row == 1 && (col == 6 || col == 7))) {
        if (occupied(board, 0, 7, colour)) return false;
        return true;
    }
  
    //these two statements check for lower corner avoid hotspots
  
    //this is bottom left
    if((row == 7 && col == 1) || (row == 6 && (col == 0 || col == 1))) {
        if (occupied(board, 7, 0, colour)) return false;
        return true;
    }
    //this is bottom right
    if((row == 7 && col == 6) || (row == 6 && (col == 6 || col == 7))) {
        if (occupied(board, 7, 7, colour)) return false;
        return true;
    }
  
    else return false;
}

bool occupied(char board[MAX][MAX], int row, int col, char colour){
  
    //if played, return true else return false
  
    //top left
    if (board[row][col] == colour) {
        return true;
    }
    else return false;
}


