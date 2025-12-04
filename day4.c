#include <stdio.h>
#include <string.h>

int main() {
	FILE *f = fopen("inputs/4.txt", "r");

	char lines[1000][1000];

	int line = 0;
	while (fgets(lines[line], sizeof(lines[0]), f)) {
		line++;
	}

	int rows = line;
	int cols = strlen(lines[0]) - 1;

	printf("rows: %i\n", rows);
	printf("cols: %i\n", cols);
	// for (int i = 0; i<rows; i++) {
	// 	printf("%s", lines[i]);
	// }

	int result = 0;

	int iteration = 0;
	int removed;
	do {
		iteration++;
		printf("iteration %i (%c)\n", iteration, (char)iteration);

		removed = 0;
		for (int row = 0; row < rows; row++) {
			for (int col = 0; col < cols; col++) {
				// Only do this if this is a paper
				// ... or a paper thatgot removed this iteration!
				if (lines[row][col] != '@' && lines[row][col] != iteration) continue;
				// Iterate the cells neighbours
				int papers = 0;
				for (int i = -1; i <= 1 ; i++) {
					for (int j = -1; j <= 1 ; j++) {
						// Exclude self
						if (i == 0 && j == 0) continue;

						// Check matric boudries
						if (row + i < 0 || row + i >= rows) continue;
						if (col + j < 0 || col + j >= cols) continue;

						if (lines[row+i][col+j] == '@' || lines[row+i][col+j] == iteration) {
							papers++;
							//printf("paper on (r:%i,c:%i)\n", row+i, col+j);
						}
						
					}
				}

				//printf("(r:%i,c:%i) %i neighborus\n", row, col, papers);

				if (papers < 4) {
					result++;
					removed++;
					lines[row][col] = iteration;
				}

			}
			// printf("\n");
		}
		printf("removed: %i\n", removed);
	} while (removed != 0);

	printf("%i\n", result);

	fclose(f);

	return 0;
}
