//
// Author: Akshay Ravikumar
//


#include <stdbool.h>
#include <stdio.h>

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
  while (board[originalRow][originalCol] == 'U') {
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

bool validRowCol(int row, int col, int Size) {
  if ((row >= 0 && row < Size) && (col >= 0 && col < Size)) {
    return true;
  } else {
    return false;
  }
}

int main(void) {
  int dimension;
  printf("Enter the board dimension: ");
  scanf("%d", &dimension);
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
  char inpColour;
  char rowInp;
  char colInp;
  printf("Enter board configuration:\n");
  getchar();
  scanf("%c%c%c", &inpColour, &rowInp, &colInp);
  while (inpColour != '!') {
    if (inpColour == 'W') {
      board[rowInp - 'a'][colInp - 'a'] = 'W';
    } else if (inpColour == 'B') {
      board[rowInp - 'a'][colInp - 'a'] = 'B';
    }
    getchar();
    scanf("%c%c%c", &inpColour, &rowInp, &colInp);
  }
  printBoard(board, dimension);
  printf("Available moves for W:\n");
  for (int row = 0; row < dimension; row++) {
    for (int col = 0; col < dimension; col++) {
      for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
          if (checkLegalInDirection(board, dimension, row, col, 'W', deltaRow,
                                    deltaCol)) {
            printf("%c%c\n", 'a' + row, 'a' + col);
          }
        }
      }
    }
  }
  printf("Available moves for B:\n");
  for (int row = 0; row < dimension; row++) {
    for (int col = 0; col < dimension; col++) {
      for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
          if (checkLegalInDirection(board, dimension, row, col, 'B', deltaRow,
                                    deltaCol)) {
            printf("%c%c\n", 'a' + row, 'a' + col);
          }
        }
      }
    }
  }
  printf("Enter a move:\n");
  char validColour, validRow, validCol;
  getchar();
  scanf("%c%c%c", &validColour, &validRow, &validCol);
  int countValid = 0;
  bool valid;
  for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
    for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
      if (checkLegalInDirection(board, dimension, validRow - 'a',
                                validCol - 'a', validColour, deltaRow,
                                deltaCol)) {
        countValid++;
      }
    }
  }
  if (countValid > 0) {
    printf("Valid move.\n");
    valid = true;
  } else {
    printf("Invalid move.\n");
    valid = false;
  }
  char rowCopy = validRow;
  char colCopy = validCol;
  for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
    for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
      if (checkLegalInDirection(board, dimension, rowCopy - 'a', colCopy - 'a',
                                validColour, deltaRow, deltaCol)) {
        while (valid) {
          board[validRow - 'a'][validCol - 'a'] = validColour;
          validRow += deltaRow;
          validCol += deltaCol;
          if (board[validRow - 'a'][validCol - 'a'] == validColour) {
            validRow = rowCopy;
            validCol = colCopy;
            break;
          }
        }
      }
    }
  }
  printBoard(board, dimension);
  return 0;
}