/**
 * @file reversi.c
 * @author <-- Your name here-->
 * @brief This file is used for APS105 Lab 8. 2023W version
 * @date 2023-03-14
 *
 */
// DO NOT REMOVE THE FOLLOWING LINE
#if !defined(TESTER_P1) && !defined(TESTER_P2)
// DO NOT REMOVE THE ABOVE LINE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// DO NOT REMOVE THE FOLLOWING LINE
#endif
// DO NOT REMOVE THE ABOVE LINE

void printBoard(char board[][26], int n) {
  char alpha = 'a';
  char originalAlpha = alpha;
  printf("  ");
  for (int row = 0; row < n; row++) {
    printf("%c", alpha);
    alpha++;
  }
  printf("\n");
  for (int row = 0; row < n; row++) {
    printf("%c ", originalAlpha);
    originalAlpha++;
    for (int col = 0; col < n; col++) {
      printf("%c", board[row][col]);
    }
    printf("\n");
  }
}

bool positionInBounds(int n, int row, int col) {
  if (((row < 0) || (row >= n)) || ((col < 0) || (col >= n))) {
    return false;
  }
  return true;
}

bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol) {
  int originalRow = row;
  int originalCol = col;
  while (board[originalRow][originalCol] == 'U' &&
         positionInBounds(n, originalRow, originalCol)) {
    row += deltaRow;
    col += deltaCol;
    bool check = false;
    if (positionInBounds(n, row, col) == true) {
      if (board[row][col] == colour) {
        return false;
      }
    }
    if (deltaRow == 0 && deltaCol == 0) {
      return false;
    }

    while (positionInBounds(n, row, col) &&
           positionInBounds(n, row + deltaRow, col + deltaCol)) {
      if (board[row][col] != colour && board[row][col] != 'U') {
        row += deltaRow;
        col += deltaCol;
        if (board[row][col] != 'U') {
          if (board[row][col] == colour) {
            check = true;
            return check;
          }
        }
      } else {
        return false;
      }
    }

    return check;
  }
  return false;
}

int makeMove(char board[26][26], int n, char turn, int *row, int *col) {
  int count = 0;
  int rowCopy = *row;
  int colCopy = *col;
  for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
    for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
      if (checkLegalInDirection(board, n, *row, *col, turn, deltaRow,
                                deltaCol)) {
        (*row) += deltaRow;
        (*col) += deltaCol;
        while (board[*row][*col] != turn && board[*row][*col] != 'U' &&
               positionInBounds(n, *row, *col)) {
          count++;
          (*row) += deltaRow;
          (*col) += deltaCol;
        }
        *row = rowCopy;
        *col = colCopy;
      }
    }
  }

  return count;
}

bool gameOver(char board[26][26], int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (board[i][j] == 'U') return false;
    }
  }
  return true;
}

bool validMove(char board[26][26], int *row, int *col, int n, char colour) {
  int countValid = 0;
  bool valid = true;
  for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
    for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
      if (checkLegalInDirection(board, n, *row, *col, colour, deltaRow,
                                deltaCol)) {
        countValid++;
      }
    }
  }
  if (countValid > 0) {
    valid = true;
  } else {
    valid = false;
  }
  return valid;
}

void swapColours(char board[26][26], char *validRow, char *validCol, int n,
                 char colour) {
  char rowCopy = *validRow;
  char colCopy = *validCol;
  int rowCopyChar = rowCopy - 'a';
  int colCopyChar = colCopy - 'a';
  bool valid = validMove(board, &rowCopyChar, &colCopyChar, n, colour);
  for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
    for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
      if (checkLegalInDirection(board, n, rowCopy - 'a', colCopy - 'a', colour,
                                deltaRow, deltaCol)) {
        while (valid) {
          board[(*validRow) - 'a'][(*validCol) - 'a'] = colour;
          *(validRow) += deltaRow;
          *(validCol) += deltaCol;
          if (board[(*validRow) - 'a'][(*validCol) - 'a'] == colour) {
            *validRow = rowCopy;
            *validCol = colCopy;
            board[(*validRow) - 'a'][(*validCol) - 'a'] = 'U';
            break;
          }
        }
      }
    }
  }
  if (valid) {
    board[rowCopyChar][colCopyChar] = colour;
  }
}

bool availableLegalMoves(char board[26][26], int n, char colour) {
  int count = 0;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
          if (checkLegalInDirection(board, n, row, col, colour, deltaRow,
                                    deltaCol)) {
            count++;
          }
        }
      }
    }
  }
  if (count == 0) {
    return false;
  }
  return true;
}

int countWhite(char board[26][26], int n) {
  int count = 0;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == 'W') {
        count++;
      }
    }
  }
  return count;
}

int countBlack(char board[26][26], int n) {
  int count = 0;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == 'B') {
        count++;
      }
    }
  }
  return count;
}

void computerMove(char board[26][26], int dimension, char compColour,
                  bool *availableValidMoveComp) {
  int largest = 0;
  for (int row = 0; row < dimension; row++) {
    for (int col = 0; col < dimension; col++) {
      int count = makeMove(board, dimension, compColour, &row, &col);
      if (count > largest) {
        largest = count;
      }
    }
  }
  char colours[3] = {'B', 'W', 'U'};
  bool check = false;
  char oppColour;
  for (int i = 0; i < 3; i++) {
    if (colours[i] != compColour && colours[i] != 'U') {
      oppColour = colours[i];
      break;
    }
  }
  for (int row = 0; row < dimension; row++) {
    for (int col = 0; col < dimension; col++) {
      if (largest == 0) {
        if ((countBlack(board, dimension) + countWhite(board, dimension)) !=
            (dimension * dimension)) {
          printf("%c player has no valid move.\n", compColour);
          check = true;
          (*availableValidMoveComp) = false;
          break;
        }
      }
      // if (row == 0 || col == 0 || row == dimension - 1 ||
      //     col == dimension - 1) {
      //   if (validMove(board, &row, &col, dimension, compColour)) {
      //     check = true;
      //     printf("Computer places %c at %c%c.\n", compColour, row + 'a',
      //            col + 'a');
      //     char newRow = row + 'a';
      //     char newCol = col + 'a';
      //     swapColours(board, &newRow, &newCol, dimension, compColour);
      //     printBoard(board, dimension);
      //     break;
      //   }
      // }

      // else if ((row == 0 && col == 0) || (row == 0 && col == dimension - 1)
      // ||
      //          (row == dimension - 1 && col == 0) ||
      //          (row == dimension - 1 && col == dimension - 1)) {
      //   for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
      //     for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
      //       if (checkLegalInDirection(board, dimension, row, col, compColour,
      //                                 deltaRow, deltaCol)) {
      //         check = true;
      //         printf("Computer places %c at %c%c.\n", compColour, row + 'a',
      //                col + 'a');
      //         char newRow = row + 'a';
      //         char newCol = col + 'a';
      //         swapColours(board, &newRow, &newCol, dimension, compColour);
      //         printBoard(board, dimension);
      //         break;
      //       }
      //     }
      //   }
      //   if (check == true) {
      //     break;
      //   }
      // }

      else {
        if (makeMove(board, dimension, compColour, &row, &col) == largest) {
          check = true;
          printf("Computer places %c at %c%c.\n", compColour, row + 'a',
                 col + 'a');
          char newRow = row + 'a';
          char newCol = col + 'a';
          swapColours(board, &newRow, &newCol, dimension, compColour);
          printBoard(board, dimension);
          break;
        }
      }
    }
    if (check == true) {
      break;
    }
  }
}

//*******************************************************
// Note: Please only put your main function below
// DO NOT REMOVE THE FOLLOWING LINE
#ifndef TESTER_P2
// DO NOT REMOVE THE ABOVE LINE
int main(void) {
  // Write your own main function here
  int dimension;
  printf("Enter the board dimension: ");
  scanf("%d", &dimension);
  char compColour;
  printf("Computer plays (B/W): ");
  getchar();
  scanf("%c", &compColour);
  char board[26][26];
  for (int row = 0; row <= 25; row++) {
    for (int col = 0; col <= 25; col++) {
      board[row][col] = 'U';
      if ((row == col) && ((row == (dimension - 1) / 2) ||
                           (row == ((dimension - 1) / 2) + 1))) {
        board[row][col] = 'W';
      } else if ((row + col == (dimension - 1)) &&
                 ((row == (dimension - 1) / 2) ||
                  (row == ((dimension - 1) / 2) + 1))) {
        board[row][col] = 'B';
      }
    }
  }

  printBoard(board, dimension);
  if (compColour == 'W') {
    bool check = false;
    bool availableValidMovesComp = true;
    bool availableValidMovesB = true;
    while (!gameOver(board, dimension)) {
      if (availableLegalMoves(board, dimension, 'B')) {
        char inpRow, inpCol;
        printf("Enter move for colour B (RowCol): ");
        getchar();
        scanf("%c%c", &inpRow, &inpCol);
        int inpRowInt = inpRow - 'a';
        int inpColInt = inpCol - 'a';
        if (!validMove(board, &inpRowInt, &inpColInt, dimension, 'B')) {
          printf("Invalid move.\n");
          printf("%c player wins.", compColour);
          check = true;
          break;
        }
        swapColours(board, &inpRow, &inpCol, dimension, 'B');
        printBoard(board, dimension);
        computerMove(board, dimension, compColour, &availableValidMovesComp);

      } else {
        printf("B has no valid move.\n");
        availableValidMovesB = false;
        computerMove(board, dimension, compColour, &availableValidMovesComp);
      }
    }
    if (check == false ||
        (availableValidMovesComp == false && availableValidMovesB == false)) {
      if (countBlack(board, dimension) > countWhite(board, dimension)) {
        printf("B player wins.\n");
      } else if (countBlack(board, dimension) < countWhite(board, dimension)) {
        printf("W player wins.\n");
      } else if (countBlack(board, dimension) == countWhite(board, dimension)) {
        printf("Draw!\n");
      }
    }
  } else if (compColour == 'B') {
    bool check = false;
    bool availableValidMovesComp = true;
    bool availableValidMovesW = true;
    while (!gameOver(board, dimension)) {
      computerMove(board, dimension, compColour, &availableValidMovesComp);
      if (availableLegalMoves(board, dimension, 'W')) {
        char inpRow, inpCol;
        printf("Enter move for colour W (RowCol): ");
        getchar();
        scanf("%c%c", &inpRow, &inpCol);
        int inpRowInt = inpRow - 'a';
        int inpColInt = inpCol - 'a';
        if (!validMove(board, &inpRowInt, &inpColInt, dimension, 'W')) {
          printf("Invalid move.\n");
          printf("%c player wins.", compColour);
          check = true;
          break;
        }
        swapColours(board, &inpRow, &inpCol, dimension, 'W');
        printBoard(board, dimension);
      }
    }
    if (check == false) {
      if (countBlack(board, dimension) > countWhite(board, dimension)) {
        printf("B player wins.\n");
      } else if (countBlack(board, dimension) < countWhite(board, dimension)) {
        printf("W player wins.\n");
      } else {
        printf("Draw!\n");
      }
    }
    
  }
  return 0;
}

// DO NOT REMOVE THE FOLLOWING LINE
#endif
// DO NOT REMOVE THE ABOVE LINE
//*******************************************************