#include <stdio.h>
#include <stdlib.h>  // For exit() function

int main(int argc, char *argv[]) {
    char* filename = "/home/sniikz/Desktop/driverimplementation/6.changes/driver";
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open file %s\n", filename);
        exit(1);
    }

    // Check if a command line argument was provided
    if (argc < 2) {
        printf("Usage: %s <text to write>\n", argv[0]);
        exit(1);
    }

    // Write the command line argument to the file
    fprintf(file, "%s\n", argv[1]);

    fclose(file);
    printf("Successfully wrote to file %s\n", filename);
    return 0;
}
