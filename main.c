#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define MAX 100000

void gen_file(const char *filename, int count, int min, int max) {
	
	FILE *file = fopen(filename, "w");
	
	if (file == NULL) {
		printf("Ошибка открытия файла для записи данных.\n");
		return;
	}

	srand(time(NULL));

	for (int i = 0; i < count; i++) {
		int nums = (rand() % (max - min + 1)) + min;
		fprintf(file, "%d\n", nums);
	}

	fclose(file);
	printf("Сгенерировано и записано %d чисел в диапазоне [%d, %d] в файл '%s'.\n", count, min, max, filename);
}

int* read_file(const char *filename, int* count) {

	FILE *file = fopen(filename, "r");

	if (file == NULL) {
		printf("Файл '%s' не найден.\n", filename);
		return NULL;
	}

	int* numbers = malloc(MAX * sizeof(int));

	if (numbers == NULL) {
		printf("Ошибка при выделении памяти.\n");
		fclose(file);
		return NULL;
	}

	*count = 0;

	while (*count < MAX && fscanf(file, "%d", &numbers[*count]) == 1) {
		(*count)++;
	}

	fclose(file);
	return numbers;
}

void bubble_sort(int *array, int count) {
   
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                int tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;
            }
        }
    }
}

int check (int *array, int count) {

    for (int i = 0; i < count - 1; i++) {
        if (array[i] > array[i + 1]) {
            return 0;
        }
    }
    return 1;
}