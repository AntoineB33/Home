#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_3d_list(char*** list, int layers, int rows, int columns) {
    // Process the 3D list
    for (int i = 0; i < layers; i++) {
        printf("Layer %d:\n", i);
        for (int j = 0; j < rows; j++) {
            for (int k = 0; k < columns; k++) {
                printf("%s ", list[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Expected serialized 3D list as argument\n");
        return 1;
    }

    // Deserialize the 3D list
    char* serialized = argv[1];
    char*** list3D = NULL;
    int layers = 0, rows = 0, columns = 0;

    // Copy serialized string to a modifiable buffer
    char* buffer = _strdup(serialized);
    char* context_layer = NULL;
    char* context_row = NULL;
    char* context_col = NULL;

    // First pass to count layers, rows, and columns
    char* layerToken = strtok_s(buffer, "|||", &context_layer);
    while (layerToken != NULL) {
        layers++;
        char* rowToken = strtok_s(layerToken, "||", &context_row);
        int row_count = 0;
        while (rowToken != NULL) {
            rows++;
            char* columnToken = strtok_s(rowToken, "|", &context_col);
            int col_count = 0;
            while (columnToken != NULL) {
                columns++;
                col_count++;
                columnToken = strtok_s(NULL, "|", &context_col);
            }
            row_count++;
            rowToken = strtok_s(NULL, "||", &context_row);
        }
        layerToken = strtok_s(NULL, "|||", &context_layer);
    }

    // Reset counts for allocating memory
    rows /= layers;
    columns /= (layers * rows);

    // Reset the buffer for the second pass
    strcpy_s(buffer, strlen(serialized) + 1, serialized);
    context_layer = NULL;

    list3D = malloc(layers * sizeof(char**));

    for (int i = 0; i < layers; i++) {
        list3D[i] = malloc(rows * sizeof(char*));
        char* rowToken = strtok_s(i == 0 ? buffer : NULL, "||", &context_row);
        for (int j = 0; j < rows; j++) {
            list3D[i][j] = malloc(columns * sizeof(char*));
            char* columnToken = strtok_s(j == 0 ? rowToken : NULL, "|", &context_col);
            for (int k = 0; k < columns; k++) {
                list3D[i][j][k] = _strdup(columnToken);
                columnToken = strtok_s(NULL, "|", &context_col);
            }
            rowToken = strtok_s(NULL, "||", &context_row);
        }
        strtok_s(NULL, "|||", &context_layer); // Advance to the next layer
    }

    // Call your function to process the 3D list
    process_3d_list(list3D, layers, rows, columns);

    // Free the allocated memory
    for (int i = 0; i < layers; i++) {
        for (int j = 0; j < rows; j++) {
            for (int k = 0; k < columns; k++) {
                free(list3D[i][j][k]);
            }
            free(list3D[i][j]);
        }
        free(list3D[i]);
    }
    free(list3D);
    free(buffer);

    return 0;
}
