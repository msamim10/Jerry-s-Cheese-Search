#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int recursive_search(char maze[][50], int rows, int cols, int x, int y,
                     FILE *output_file, char *m, int *a, int *b, int *ab) {
// This function returns 1 if the recursive search led to a cheese otherwise it returns 0 
  if (maze[x][y] == 'C') {
    maze[x][y] = 'M';
    return 1;
  }

//This function Indicates coordinates that have been visited by the '.' character.
  if (maze[x][y] == '|' || maze[x][y] == '.') {
    return 0;
  }

  maze[x][y] = '.';
  int dx[] = {-1, 0, 1, 0};
  int dy[] = {0, 1, 0, -1};
  char direction[] = {'N', 'E', 'S', 'W'};

  for (int i = 0; i < 4; i++) {
    int new_x = x + dx[i];
    int new_y = y + dy[i];
    if (new_x >= 0 && new_x < rows && new_y >= 0 && new_y < cols) {
      if (recursive_search(maze, rows, cols, new_x, new_y, output_file, m, a, b,
                           &(*ab))) {
        int t = *ab;
        m[t] = direction[i];
        a[t] = new_y + 1;
        b[t] = rows - new_x;
        *ab = t + 1;
        int tt = *ab;
        return 1;
      }
    }
  }

  //Once Jerry finds a cheese, this function updates his final location in the maze.
  maze[x][y] = ' ';
  return 0;
}

char *dir(char ch) {
  if (ch == 'N') return "north";
  if (ch == 'S') return "south";
  if (ch == 'E') return "east";
  return "west";
}

// This function keeps track of every (x, y) coordinate that Jerry has visited on his search
void iterative_search(char maze[][50], int rows, int cols, char *directions,
                      int direction_count, FILE *output_file) {
  char m[direction_count];
  int a[direction_count];
  int b[direction_count];
  int ab = 0;
  int found = 0;
  // Find Jerry's initial position (marked by 'M')
  int x, y;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (maze[i][j] == 'M') {
        x = i;
        y = j;
        break;
      }
    }
  }

  // Iterate through the directions and update Jerry's position accordingly
  for (int i = 0; i < direction_count; i++) {
    int new_x = x, new_y = y;

    if (directions[i] == 'N') {
      new_x--;
    } else if (directions[i] == 'E') {
      new_y++;
    } else if (directions[i] == 'S') {
      new_x++;
    } else if (directions[i] == 'W') {
      new_y--;
    }

    // Check if the new position is valid and not a wall
    if (new_x >= 0 && new_x < rows && new_y >= 0 && new_y < cols &&
        maze[new_x][new_y] != '|') {
      m[ab] = directions[i];
      a[ab] = new_y + 1;
      b[ab] = rows - new_x;
      ++ab;

      maze[x][y] = '.';
      x = new_x;
      y = new_y;

      if (maze[new_x][new_y] == 'C') {
        found = 1;
        maze[x][y] = '.';
        x = new_x;
        y = new_y;
        break;
      } else {
      }
    }
  }
  // Update Jerry's final position
  maze[x][y] = 'M';

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      fputc(maze[i][j], output_file);
    }
    fputc('\n', output_file);
  }

  for (int i = 0; i < ab; i++) {
    char *d = dir(m[i]);
    fprintf(output_file, "Jerry moved %s (%d, %d)\n", d, a[i], b[i]);
  }

  if (found) {
    fprintf(output_file, "Jerry found the cheese!");
  }
}

// Show error message if files are not found
int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf(
        "Error - two input files were not provided as command line "
        "arguments\n");
    return 1;
  }
  // Open the input files and verify they can be read.
  FILE *input_file1 = fopen(argv[1], "r");
  FILE *input_file2 = fopen(argv[2], "r");


  if (input_file1 == NULL) {
    printf("Error - the %s file could not be opened\n", argv[1]);
    return 1;
  }

  if (input_file2 == NULL) {
    printf("Error - the %s file could not be opened\n", argv[2]);
    return 1;
  }

  // Read the maze from the first input file and store it in a 2D array.
  char maze[50][50];
  int rows = 0, cols = 0;

  while (fgets(maze[rows], sizeof(maze[0]), input_file1)) {
    rows++;
  }

  cols = strlen(maze[0]) - 1;

  // Read the cardinal directions from the second input file and store them in
  // an array or list.
  char directions[50];
  int direction_count = 0;

  char ch;
  while (fscanf(input_file2, "%c", &ch) != EOF) {
    if (ch == '\n') continue;
    directions[direction_count] = ch;
    // printf("%c\n", ch);
    direction_count++;
  }

  // Write the updated maze to the first output file.
  FILE *output_file1 = fopen("iterative_labryinth.out", "w");
  FILE *output_file2 = fopen("recursive_labryinth.out", "w");

  // Call the iterative_search function with the maze and the cardinal directions.
  iterative_search(maze, rows, cols, directions, direction_count, output_file1);


  // Reset the maze for the recursive search
  fclose(input_file1);
  input_file1 = fopen(argv[1], "r");
  rows = 0;
  cols = 0;

  while (fgets(maze[rows], sizeof(maze[0]), input_file1)) {
    rows++;
  }

  cols = strlen(maze[0]) - 1;


  int start_x, start_y;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (maze[i][j] == 'M') {
        start_x = i;
        start_y = j;
        break;
      }
    }
  }

  int totalCell = rows * cols;
  char m[totalCell];
  int a[totalCell];
  int b[totalCell];
  int ab = 0;

  // Call the recursive_search function with the maze.
  int result = recursive_search(maze, rows, cols, start_x, start_y,
                                output_file2, m, a, b, &ab);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      fputc(maze[i][j], output_file2);
    }
    fputc('\n', output_file2);
  }

  for (int i = ab - 1; i >= 0; i--) {
    char *d = dir(m[i]);
    fprintf(output_file2, "Jerry moved %s (%d, %d)\n", d, a[i], b[i]);
  }

  if (result) {
    fprintf(output_file2, "Jerry found the cheese!");
  }

// Show the outputs are successfully outputted  
  printf("Success - two output files iterative_labryinth.out and\n");
  printf("recursive_labryinth.out produced\n");

  // Close the input and output files.
  fclose(input_file1);
  fclose(input_file2);
  fclose(output_file1);
  fclose(output_file2);

  return 0;
}

