#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define MAX 26

//board initialization
int  getSize(int n);
char initializeBoard(char board[MAX][MAX], int size);
char printBoard(char board[MAX][MAX], int n);
char boardConfig(char board[MAX][MAX]);

//computer ai
void computerMove(char board[MAX][MAX], int n, char computer);
int  checkScore(char board[MAX][MAX], int n, char colour, int row, int col, int deltaRow, int deltaCol);
void availableMoves(char board[MAX][MAX], int n);       //NOT NECESSARY

//validation
bool positionInBounds(int n, int row, int col);
bool checkLegalInDirection(char board[MAX][MAX], int n, int row, int col, char colour, int deltaRow, int deltaCol);
bool moveValidation(char board[MAX][MAX], int n, char row, char col, char colour);

//tile flipping
void flipTiles(char board[MAX][MAX], char colour, int row, int col, int deltaRow, int deltaCol);

int main(){
    int n = 0;
//    char colour = '0',row = '0',col = '0';
    char board[MAX][MAX] = {};
    char computer = 'B';
    
    //gets board size and initializes board
    n = getSize(n);
    initializeBoard(board,n);
    printBoard(board,n);
    boardConfig(board);
    printBoard(board, n);
    
    //asks what colour the computer plays !NOT ACCEPTING USER INPUT??!
//    printf("Computer plays (B/W): ");
//    scanf("%c", &computer);
//    printBoard(board, n);
    
      //BLACK ALWAYS MOVES FIRST  
    
      //  alternate turns

//    this tests the checkScore function (currently works)
//    printf("Sum = %d\n", checkScore(board, n, 'W', 0, 0, 1, 0) + checkScore(board, n, 'W', 0, 0, 0, 1));
//    printf("Down = %d\n",checkScore(board, n, 'W', 0, 0, 1, 0));
//    printf("Right = %d\n",checkScore(board, n, 'W', 0, 0, 0, 1));
//    printf("%d\n",checkScore(board, n, 'W', 0, 0, 1, -1));

    computerMove(board,n,computer);
    printBoard(board,n);

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
        int i = row-97;int j = col-97;
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

void availableMoves(char board[MAX][MAX], int n){       //NOT NEEDED - REUSE INSTEAD FOR AI
        
    //for white
    printf("Available moves for W:\n");
    for(int i = 0;i<n;i++){
        for(int j = 0;j<n;j++){
            //implement for loop 
            if(checkLegalInDirection(board, n, i, j, 'W', 1, 0)) printf("%c%c\n",i+97,j+97); //DOWN
            if(checkLegalInDirection(board, n, i, j, 'W', -1, 0)) printf("%c%c\n",i+97,j+97); //UP
            if(checkLegalInDirection(board, n, i, j, 'W', 0, 1)) printf("%c%c\n",i+97,j+97); //RIGHT
            if(checkLegalInDirection(board, n, i, j, 'W', 0,-1)) printf("%c%c\n",i+97,j+97); //LEFT
            
            if(checkLegalInDirection(board, n, i, j, 'W', -1, -1)) printf("%c%c\n",i+97,j+97); //NW
            if(checkLegalInDirection(board, n, i, j, 'W', 1, 1)) printf("%c%c\n",i+97,j+97); //SE
            if(checkLegalInDirection(board, n, i, j, 'W', 1, -1)) printf("%c%c\n",i+97,j+97); //SW
            if(checkLegalInDirection(board, n, i, j, 'W', -1, 1)) printf("%c%c\n",i+97,j+97); //NE
        }
    }
    
    //for black    
    printf("Available moves for B:\n");
    for(int i = 0;i<n;i++){
        for(int j = 0;j<n;j++){
            //implement for loop later
            if(checkLegalInDirection(board, n, i, j, 'B', 1, 0))printf("%c%c\n",i+97,j+97); // DOWN 
            if(checkLegalInDirection(board, n, i, j, 'B', -1, 0)) printf("%c%c\n",i+97,j+97); //UP
            if(checkLegalInDirection(board, n, i, j, 'B', 0, 1)) printf("%c%c\n",i+97,j+97); //RIGHT
            if(checkLegalInDirection(board, n, i, j, 'B', 0,-1)) printf("%c%c\n",i+97,j+97); //LEFT
            
            if(checkLegalInDirection(board, n, i, j, 'B', -1, -1)) printf("%c%c\n",i+97,j+97); //NW
            if(checkLegalInDirection(board, n, i, j, 'B', 1, 1)) printf("%c%c\n",i+97,j+97); //SE
            if(checkLegalInDirection(board, n, i, j, 'B', 1, -1)) printf("%c%c\n",i+97,j+97); //SW
            if(checkLegalInDirection(board, n, i, j, 'B', -1, 1)) printf("%c%c\n",i+97,j+97); //NE
        }
    }
}

bool checkLegalInDirection(char board[MAX][MAX], int n, int row, int col, char colour, int deltaRow, int deltaCol){     //KEEP  
    
    //checking if the position has already been played (invalid)
    if (board[row][col] !='U') return false;
    
    
    //checking if adjacent one is the same colour (invalid)
    if(board[row+deltaRow][col+deltaCol] == colour) return false;
    
    
    //checking if adjacent one is empty (invalid)
    if(board[row+deltaRow][col+deltaCol] == 'U') return false;
    
    //checking if out of bounds (invalid)
    if(!positionInBounds(n,row+deltaRow,col+deltaCol)) return false;
        
    //checking if there is eventually an existing colour as the one being played down the line (valid)
    while(positionInBounds(n,row + deltaRow,col + deltaCol) && board[row + deltaRow][col + deltaCol] != colour){         //checks to see if within board
        
        if(deltaRow!=0){
            if(deltaRow<0) deltaRow--;
            else deltaRow++;
        }
        
        if(deltaCol!=0){
            if(deltaCol<0) deltaCol--;
            else deltaCol++;
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

bool moveValidation(char board[MAX][MAX], int n, char row, char col, char colour){      //USE FOR HUMAN MOVES
    
    bool valid = false;
        
    //single move validation (change to move validation)
    printf("Enter a move: \n");
    scanf(" %c%c%c", &colour,&row,&col);
    
    if(checkLegalInDirection(board, n, row - 97, col - 97, colour, 1, 0)) {
        flipTiles(board, colour, row - 97,col - 97,1,0); 
        valid = true;
    }
    if(checkLegalInDirection(board, n, row - 97, col - 97, colour, -1, 0)) {
        flipTiles(board, colour, row - 97 ,col - 97,-1,0); 
        valid = true;
    }
    if(checkLegalInDirection(board, n, row - 97, col - 97, colour, 0, 1)) {
        flipTiles(board, colour, row - 97 ,col - 97,0,1); 
        valid = true;
    }
    if(checkLegalInDirection(board, n, row - 97, col - 97, colour, 0, -1)) {
        flipTiles(board, colour, row - 97 ,col - 97,0,-1); 
        valid = true;
    }
    
    if(checkLegalInDirection(board, n, row - 97, col - 97, colour, -1, -1)) {
        flipTiles(board, colour, row - 97 ,col - 97,-1,-1); 
        valid = true;

    }
    if(checkLegalInDirection(board, n, row - 97, col - 97, colour, 1, 1)) {
        flipTiles(board, colour, row - 97 ,col - 97,1,1); 
        valid = true;
    }
    if(checkLegalInDirection(board, n, row - 97, col - 97, colour, 1, -1)) {
        flipTiles(board, colour, row - 97 ,col - 97,1,-1); 
        valid = true;
    }
    if(checkLegalInDirection(board, n, row - 97, col - 97, colour, -1, 1)) {
        flipTiles(board, colour, row - 97 ,col - 97,-1,1); 
        valid = true;
    }
      
    if(valid){
        board[row-97][col-97] = colour;
        printf("Valid Move.\n");
        return true;
    }
    else {
        printf("Invalid move.\n");
        return false;
    }
//    exit program if invalid move played (automatic loss)    
}

void flipTiles(char board[MAX][MAX], char colour, int row, int col, int deltaRow, int deltaCol){    //KEEP
       
    while(board[row+deltaRow][col+deltaCol] != colour){
        
        board[row+deltaRow][col+deltaCol] = colour;
                
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

int checkScore(char board[MAX][MAX], int n, char colour, int row, int col, int deltaRow, int deltaCol){ //same as flipTile function
    
    int score = 0;
    
    //this actually causes an error... don't return 0. instead, just check for move validity before calling this function
//    if (!checkLegalInDirection(board,n,colour,row,col,deltaRow,deltaCol)){
//        return 0;
//    }
    
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

void computerMove(char board[MAX][MAX], int n, char computer){
    
    printf("computer plays\n");
    
    bool valid = false;     
    int highScore = 0, score = 0; //highest score of board and of cell
    int rowPlay = 0, colPlay = 0; //this refers to the row, col of the highest scoring cell    

//  check to see if each position is valid, and set valid to true if yes
    for(int i = 0;i<n;i++){
        for(int j = 0;j<n;j++){
            //implement for loop 
            if(checkLegalInDirection(board, n, i, j, computer, 1, 0)){
                score += checkScore(board, n, computer, i, j, 1, 0);
                valid = true;  //DOWN
            }
            if(checkLegalInDirection(board, n, i, j, computer, -1, 0)){
                score += checkScore(board, n, computer, i, j, -1, 0);
                valid = true;  //UP
            }  
            if(checkLegalInDirection(board, n, i, j, computer, 0, 1)){
                score += checkScore(board, n, computer, i, j, 0, 1);
                valid = true;  //RIGHT
            } 
            if(checkLegalInDirection(board, n, i, j, computer, 0,-1)){
                score += checkScore(board, n, computer, i, j, 0, -1);
                valid = true;  //LEFT
            }
            
            if(checkLegalInDirection(board, n, i, j, computer, -1, -1)){
                score += checkScore(board, n, computer, i, j, -1, -1);
                valid = true;  //NW
            }
            if(checkLegalInDirection(board, n, i, j, computer, 1, 1)){
                score += checkScore(board, n, computer, i, j, 1, 1);
                valid = true;  //SE
            }
            if(checkLegalInDirection(board, n, i, j, computer, 1, -1)){
                score += checkScore(board, n, computer, i, j, 1, -1);
                valid = true;  //SW
            }
            if(checkLegalInDirection(board, n, i, j, computer, -1, 1)){
                score += checkScore(board, n, computer, i, j, -1, 1);
                valid = true;  //NE
            }

            if(valid){           
                //now it knows the highest scoring cell and its position on the board
                if (score > highScore){
                    highScore = score;
                    rowPlay = i;
                    colPlay = j;
                }
            }
        }
    }
    
    //now just flip tiles, if it is legal direction
    if(checkLegalInDirection(board, n, rowPlay, colPlay, computer, 1, 0)){
        flipTiles(board, computer, rowPlay, colPlay, 1, 0);    
    }
    if(checkLegalInDirection(board, n, rowPlay, colPlay, computer, -1, 0)){
        flipTiles(board,computer,rowPlay, colPlay, -1, 0);
    }  
    if(checkLegalInDirection(board, n, rowPlay, colPlay, computer, 0, 1)){
        flipTiles(board,computer,rowPlay,colPlay, 0, 1);
    } 
    if(checkLegalInDirection(board, n, rowPlay, colPlay, computer, 0,-1)){
        flipTiles(board, computer, rowPlay, colPlay,0, -1 );
    }
    if(checkLegalInDirection(board, n, rowPlay, colPlay, computer, -1, -1)){
        flipTiles(board, computer, rowPlay, colPlay,-1, -1 );
    }
    if(checkLegalInDirection(board, n, rowPlay, colPlay, computer, 1, 1)){
        flipTiles(board, computer, rowPlay, colPlay,1, 1 );
    }
    if(checkLegalInDirection(board, n, rowPlay, colPlay, computer, 1, -1)){
        flipTiles(board, computer, rowPlay, colPlay, 1,-1);
    }
    if(checkLegalInDirection(board, n, rowPlay, colPlay, computer, -1, 1)){
        flipTiles(board, computer, rowPlay, colPlay, -1,1);
    }
    board[rowPlay][colPlay] = computer;
    
}

