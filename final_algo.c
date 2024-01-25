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

void print_Board(char[][26], int);
bool position_In_Bounds(int, int, int);
bool check_Legal_In_Direction(char[][26], int, int, int, char, int, int);
int countFlips(char[][26], int, char, int *, int *);
bool gameOver(char[][26], int);
bool validMove(char[][26], int *, int *, int, char);
bool availableLegalMoves(char[][26], int, char);
int countWhite(char[][26], int);
int countBlack(char[][26], int);

void print_Board(char board[][26], int n) {
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

bool position_In_Bounds(int n, int row, int col) {
  if (((row < 0) || (row >= n)) || ((col < 0) || (col >= n))) {
    return false;
  }
  return true;
}

bool check_Legal_In_Direction(char board[][26], int n, int row, int col,
                              char colour, int deltaRow, int deltaCol) {
  int originalRow = row;
  int originalCol = col;
  while (board[originalRow][originalCol] == 'U' &&
         position_In_Bounds(n, originalRow, originalCol)) {
    row += deltaRow;
    col += deltaCol;
    bool check = false;
    if (position_In_Bounds(n, row, col) == true) {
      if (board[row][col] == colour) {
        return false;
      }
    }
    if (deltaRow == 0 && deltaCol == 0) {
      return false;
    }

    while (position_In_Bounds(n, row, col) &&
           position_In_Bounds(n, row + deltaRow, col + deltaCol)) {
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

int countFlips(char board[26][26], int n, char turn, int *row, int *col) {
  int count = 0;
  int rowCopy = *row;
  int colCopy = *col;
  for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
    for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
      if (check_Legal_In_Direction(board, n, *row, *col, turn, deltaRow,
                                   deltaCol)) {
        (*row) += deltaRow;
        (*col) += deltaCol;
        while (board[*row][*col] != turn && board[*row][*col] != 'U' &&
               position_In_Bounds(n, *row, *col)) {
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
      if (check_Legal_In_Direction(board, n, *row, *col, colour, deltaRow,
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

bool availableLegalMoves(char board[26][26], int n, char colour) {
  int count = 0;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
          if (check_Legal_In_Direction(board, n, row, col, colour, deltaRow,
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

/*int makeMove(char board[][26], int n, char turn, int *row, int *col) {
  int largest = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      int count = countFlips(board, n, turn, &i, &j);
      if (count > largest) {
        largest = count;
      }
    }
  }
  char colours[3] = {'B', 'W', 'U'};
  bool check = false;
  char oppColour;
  for (int i = 0; i < 3; i++) {
    if (colours[i] != turn && colours[i] != 'U') {
      oppColour = colours[i];
      break;
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (largest == 0) {
        if ((countBlack(board, n) + countWhite(board, n)) != (n * n)) {
          printf("%c player has no valid move.\n", turn);
          check = true;
          break;
        }
      }

      else {
        if (countFlips(board, n, turn, &i, &j) == largest) {
          check = true;
          *(row) = i;
          *(col) = j;
          break;
        }
      }
    }
    if (check == true) {
      break;
    }
  }
  return 0;
}*/

int makeMove(char board[][26], int dimension, char turn, int *row, int *col) {
  int boardScorePosition[26][26];
  for (int i = 0; i < 26; i++) {
    for (int j = 0; j < 26; j++) {
      boardScorePosition[i][j] = 0;
    }
  }

  for (int i = 0; i < dimension; i++) {
    for (int j = 0; j < dimension; j++) {
      if (i == ((dimension - 1) / 2) - 1 || i == ((dimension + 1) / 2) + 1 ||
          j == ((dimension - 1) / 2) - 1 || j == ((dimension + 1) / 2) + 1) {
        boardScorePosition[i][j] = -2;
      }

      if (i == 1 || i == dimension - 2 || j == 1 || j == dimension - 2) {
        boardScorePosition[i][j] = -1;
      }

      if (i == 0 || i == dimension - 1 || j == 0 || j == dimension - 1) {
        boardScorePosition[i][j] = 1;
      }

      if ((i == 0 && j == 0) || (i == 0 && j == dimension - 1) ||
          (i == dimension - 1 && j == 0) ||
          (i == dimension - 1 && j == dimension - 1)) {
        boardScorePosition[i][j] = 2;
      }

      if ((i == 0 && j == 1) || (i == 1 && j == 0) || (i == 1 && j == 1) ||
          (i == 0 && j == dimension - 2) || (i == 1 && j == dimension - 1) ||
          (i == 1 && j == dimension - 2) || (i == dimension - 2 && j == 0) ||
          (i == dimension - 1 && j == 1) || (i == dimension - 2 && j == 1) ||
          (i == dimension - 2 && j == dimension - 2) ||
          (i == dimension - 2 && j == dimension - 1) ||
          (i == dimension - 1 && j == dimension - 2)) {
        boardScorePosition[i][j] = -3;
      }
    }
  }
  int largest = -3;
  for (int i = 0; i < dimension; i++) {
    for (int j = 0; j < dimension; j++) {
      for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
          if (check_Legal_In_Direction(board, dimension, i, j, turn, deltaRow,
                                       deltaCol)) {
            if (boardScorePosition[i][j] > largest) {
              largest = boardScorePosition[i][j];
            }
          }
        }
      }
    }
  }
  int bestMoveCount = 0;
  for (int i = 0; i < dimension; i++) {
    for (int j = 0; j < dimension; j++) {
      for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
        for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
          if (check_Legal_In_Direction(board, dimension, i, j, turn, deltaRow,
                                       deltaCol)) {
            if (boardScorePosition[i][j] == largest) {
              bestMoveCount++;
            }
          }
        }
      }
    }
  }
  if (bestMoveCount == 1) {
    for (int i = 0; i < dimension; i++) {
      for (int j = 0; j < dimension; j++) {
        for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
          for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if (check_Legal_In_Direction(board, dimension, i, j, turn, deltaRow,
                                         deltaCol)) {
              if (boardScorePosition[i][j] == largest) {
                *(row) = i;
                *(col) = j;
                break;
              }
            }
          }
        }
      }
    }
  } else {
    int largestCount = 0;
    for (int i = 0; i < dimension; i++) {
      for (int j = 0; j < dimension; j++) {
        for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
          for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if (check_Legal_In_Direction(board, dimension, i, j, turn, deltaRow,
                                         deltaCol)) {
              if (boardScorePosition[i][j] == largest) {
                if (countFlips(board, dimension, turn, &i, &j) > largestCount) {
                  largestCount = countFlips(board, dimension, turn, &i, &j);
                }
              }
            }
          }
        }
      }
    }
    for (int i = 0; i < dimension; i++) {
      for (int j = 0; j < dimension; j++) {
        for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
          for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
            if (check_Legal_In_Direction(board, dimension, i, j, turn, deltaRow,
                                         deltaCol)) {
              if (boardScorePosition[i][j] == largest) {
                if (countFlips(board, dimension, turn, &i, &j) ==
                    largestCount) {
                  *(row) = i;
                  *(col) = j;
                  break;
                }
              }
            }
          }
        }
      }
    }
  }
  return 0;
}

// DO NOT REMOVE THE FOLLOWING LINE
// #endif
// DO NOT REMOVE THE ABOVE LINE
//*******************************************************
