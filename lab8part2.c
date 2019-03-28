/* Ian Kwok-Tao Chan 1005211263
 * DESCRIPTION: Program that uses a 'greedy algorithm' to identify and 
 * play the best scoring position in a game of Reversi. 
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

//validation
bool positionInBounds(int n, int row, int col);
bool checkLegalInDirection(char board[MAX][MAX], int n, int row, int col, char colour, int deltaRow, int deltaCol);
bool moveValidation(char board[MAX][MAX], int n, int row, int col, char colour, char computer); //WILL LIKELY BE REMOVED, DUE TO ABSENCE OF HUMAN ERRORS (ASSUMING ALWAYS VALID)

//tile flipping
void flipTiles(char board[MAX][MAX], int n, char colour, int row, int col, int deltaRow, int deltaCol);
void validateAndFlip(char board[MAX][MAX], int n, char colour, int row, int col);

//end game
bool checkGameState(char board[MAX][MAX] ,int n);
bool availableMoves(char board[MAX][MAX], int n, char player); 
void winCounter(char board[MAX][MAX], int n);

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
    
    //IF MY AI IS BLACK
    if (computer == 'B'){
        colour = 'W';
        do {
                    
            //MY AI'S TURN
            //checks if my ai has available moves 
            if(availableMoves(board,n,computer)){
                computerMove(board,n,computer);
                printBoard(board,n);
            } 
            //if no available moves, check if game is over
            //if game over is true, then check for who won
            //if not, print no available moves
            else if(!availableMoves(board,n,computer)){
                if (checkGameState(board, n)) winCounter(board,n); //don't actually need this line
                else printf("%c player has no available move.\n", computer);
            }
            
            //OPPONENT'S TURN
            //checks if opponent has available moves
            if(availableMoves(board,n,colour)){
                findSmarterMove(board, n, colour, &row, &col);
                printf("Testing AI places %c at %c%c.\n", colour, row + 'a', col + 'a');
                validateAndFlip(board, n, colour, row, col);
                board[row][col]=colour;        
                printBoard(board,n);
            } 
            else if(!availableMoves(board,n,colour)){
                if (checkGameState(board,n)) winCounter(board,n); //don't actually need this line
                else printf("%c player has no available move.\n", colour);
            }
            
        } while (!checkGameState(board,n));
    } 
    
    
    //IF MY AI IS WHITE
    if (computer == 'W'){
        colour = 'B';
        do {
            
            //OPPONENT'S TURN
            //checks if opponent has available moves 
            if(availableMoves(board,n,colour)){
                //REPLACEMENT LINES OF CODE
                findSmarterMove(board, n, colour, &row, &col);
                printf("Testing AI places %c at %c%c.\n", colour, row + 'a', col + 'a');
                validateAndFlip(board, n, colour, row, col);
                board[row][col]=colour;
                printBoard(board,n);
            } 
            //if no available moves, check if game is over
            //if game over is true, then check for who won
            //if not, print no available moves
            else if(!availableMoves(board,n,colour)) {
                if(checkGameState(board,n)) winCounter(board,n); //don't actually need this line
                else printf("%c player has no available move.\n", colour);
            }
            
            //MY AI'S TURN
            //checks if my ai has available moves 
            if(availableMoves(board,n,computer)){
                computerMove(board,n,computer);
                printBoard(board,n);
            } 
            else if(!availableMoves(board,n,computer)) { 
                if (checkGameState(board,n)) winCounter(board,n); //don't actually need this line
                else printf("%c player has no available move.\n", computer);
            }        
            
        } while(!checkGameState(board,n));
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

char boardConfig(char board[MAX][MAX]){     //NOT NEEDED BUT KEEP FOR TESTING
    
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

bool moveValidation(char board[MAX][MAX], int n, int row, int col, char colour, char computer){      //PREVIOUSLY USED FOR HUMAN MOVES, WILL LIKELY BE REMOVED
    
    //originally function that took in chars (from main)
    //this function validates human moves
    //this function is not called upon in any other place
    //likely be removed from program, assuming that the other AI makes valid moves    
    
    bool valid = false;
        
    //single move validation (change to move validation)
//    printf("Enter a move: \n");
//    scanf(" %c%c%c", &colour,&row,&col);
    
    if(checkLegalInDirection(board, n, row, col, colour, 1, 0)) {
        flipTiles(board, n, colour, row, col, 1, 0); 
        valid = true;
    }
    if(checkLegalInDirection(board, n, row, col, colour, -1, 0)) {
        flipTiles(board, n, colour, row , col, -1, 0); 
        valid = true;
    }
    if(checkLegalInDirection(board, n, row, col, colour, 0, 1)) {
        flipTiles(board, n, colour, row , col, 0, 1); 
        valid = true;
    }
    if(checkLegalInDirection(board, n, row, col, colour, 0, -1)) {
        flipTiles(board, n, colour, row , col, 0, -1); 
        valid = true;
    }
    
    if(checkLegalInDirection(board, n, row, col, colour, -1, -1)) {
        flipTiles(board, n, colour, row, col, -1, -1); 
        valid = true;

    }
    if(checkLegalInDirection(board, n, row, col, colour, 1, 1)) {
        flipTiles(board, n, colour, row, col, 1, 1); 
        valid = true;
    }
    if(checkLegalInDirection(board, n, row, col, colour, 1, -1)) {
        flipTiles(board, n, colour, row ,col, 1, -1); 
        valid = true;
    }
    if(checkLegalInDirection(board, n, row, col, colour, -1, 1)) {
        flipTiles(board, n, colour, row ,col, -1, 1); 
        valid = true;
    }
      
    if(valid){
        board[row][col] = colour;
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

void validateAndFlip(char board[MAX][MAX], int n, char colour, int row, int col){
    
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if (checkLegalInDirection(board, n, row, col, colour, i, j)) {
                flipTiles(board, n, colour, row, col, i, j); 
            }
        }
    }
    
//    if(checkLegalInDirection(board, n, row, col, colour, 1, 0)){
//        flipTiles(board, n, colour, row, col, 1, 0);    
//    }
//    if(checkLegalInDirection(board, n, row, col, colour, -1, 0)){
//        flipTiles(board, n, colour, row, col, -1, 0);
//    }  
//    if(checkLegalInDirection(board, n, row, col, colour, 0, 1)){
//        flipTiles(board, n, colour,row,col, 0, 1);
//    } 
//    if(checkLegalInDirection(board, n, row, col, colour, 0,-1)){
//        flipTiles(board, n, colour, row, col,0, -1);
//    }
//    if(checkLegalInDirection(board, n, row, col, colour, -1, -1)){
//        flipTiles(board, n, colour, row, col,-1, -1 );
//    }
//    if(checkLegalInDirection(board, n, row, col, colour, 1, 1)){
//        flipTiles(board, n, colour, row, col,1, 1 );
//    }
//    if(checkLegalInDirection(board, n, row, col, colour, 1, -1)){
//        flipTiles(board, n, colour, row, col, 1,-1);
//    }
//    if(checkLegalInDirection(board, n, row, col, colour, -1, 1)){
//        flipTiles(board, n, colour, row, col, -1,1);
//    }

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

    //first, check for hotspots (optimized for 8x8)
    
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            //top left
            if (checkLegalInDirection(board, n, 0, 0, computer, i, j)) {
                board[0][0] = computer;
                break;
            }
            //top right
            if (checkLegalInDirection(board, n, 0, 7, computer, i, j)) {
                board[0][7] = computer;
                break;
            }
            //bottom left
            if (checkLegalInDirection(board, n, 7, 0, computer, i, j)) {
                board[0][0] = computer;
                break;
            }
            //bottom right
            if (checkLegalInDirection(board, n, 7, 7, computer, i, j)) {
                board[0][0] = computer;
                break;
            }
        }
    }


    //check to see if each position is valid. By decrementing for loop from n, I check lowest column and lowest row last!
    for(int i = n-1;i>=0;i--){
        for(int j = n-1;j>=0;j--){
            //implement for loop 
            if(checkLegalInDirection(board, n, i, j, computer, 1, 0)){
                score += checkScore(board, n, computer, i, j, 1, 0);
//                printf("score1 = %d at position %d,%d\n", score, i, j); //DOWN
            }
            if(checkLegalInDirection(board, n, i, j, computer, -1, 0)){
                score += checkScore(board, n, computer, i, j, -1, 0);
//                printf("score2 = %d at position %d,%d\n", score, i, j);  //UP
            }  
            if(checkLegalInDirection(board, n, i, j, computer, 0, 1)){
                score += checkScore(board, n, computer, i, j, 0, 1);
//                printf("score3 = %d at position %d,%d\n", score, i, j);  //RIGHT
            } 
            if(checkLegalInDirection(board, n, i, j, computer, 0,-1)){
                score += checkScore(board, n, computer, i, j, 0, -1);
//                printf("score4 = %d at position %d,%d\n", score, i, j);  //LEFT
            }
            
            if(checkLegalInDirection(board, n, i, j, computer, -1, -1)){
                score += checkScore(board, n, computer, i, j, -1, -1);
//                printf("score5 = %d at position %d,%d\n", score, i, j);  //NW
            }
            if(checkLegalInDirection(board, n, i, j, computer, 1, 1)){
                score += checkScore(board, n, computer, i, j, 1, 1);
//                printf("score6 = %d at position %d,%d\n", score, i, j);  //SE
            }
            if(checkLegalInDirection(board, n, i, j, computer, 1, -1)){
                score += checkScore(board, n, computer, i, j, 1, -1);
//                printf("score7 = %d at position %d,%d\n", score, i, j);  //SW
            }
            if(checkLegalInDirection(board, n, i, j, computer, -1, 1)){
                score += checkScore(board, n, computer, i, j, -1, 1);
//                printf("score8 = %d at position %d,%d\n", score, i, j); //NE
            }         
            
            /*Now it knows the highest scoring cell and its position on the board. It keeps a current running 
            * highscore of the best position on the board. I must reset score to 0 otherwise on the next loop, 
            * it will just add to the prior score. Also must remember the row and col of the best position*/
            
            if (score >= highScore) {
                highScore = score;  
                score = 0;          
                rowPlay = i;        
                colPlay = j;
            }
        }
    }
    
//    printf("Sum = %d\n", highScore);

    //now just flip tiles, if it is in legal direction
    validateAndFlip(board, n, computer, rowPlay, colPlay);
    printf("Ian places %c at %c%c.\n", computer, rowPlay + OFFSET, colPlay + OFFSET);
    board[rowPlay][colPlay] = computer;
//    printf("total score is: %d\n", highScore);
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
    
    //this function checks the current game state. False for game still running, true for game over
    
    //for white
    for(int a = 0; a < n; a++){
        for(int b = 0; b < n; b++){
            
            for(int i = -1; i <= 1; i++){
                for(int j = -1; j <= 1; j++){
                    
                    if (checkLegalInDirection(board, n, a, b, 'W', i, j)) return false; 
          
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
                    
                    if (checkLegalInDirection(board, n, a, b, 'B', i, j)) return false; 
          
                }
            }
        }
    }
   
    return true;
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
    } else if(black==white) {
        printf("Draw!\n");
    }
    
}
