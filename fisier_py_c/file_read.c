#include <stdbool.h>
#include <stdlib.h>

char* read_boolean_from_file(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        printf("eroare la deschiderea fisierului\n");
        exit(EXIT_FAILURE);
    }
    char* result=malloc(6*sizeof(char));
    char buffer[6];
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        fclose(file);
        strcpy(result,buffer);
        return result;
    } else {
        fclose(file);
        printf("eroare la citirea din fisier\n");
        exit(EXIT_FAILURE);
    }
}
