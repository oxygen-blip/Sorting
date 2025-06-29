#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <windows.h>
#include <locale.h>

#define MAX 100000

void gen_file(const char *filename, int count, int min, int max) {
	
	FILE *file = fopen(filename, "w");
	
	if (file == NULL) {
		printf("Ошибка открытия файла для записи данных.\n");
		return;
	}

	srand(time(NULL));

	for (int i = 0; i < count; i++) {
		long big_rand = ((long)rand() << 15) | rand();
		int nums = (int)(min + big_rand % (max - min + 1));
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

void bubble_sort(int *array, int count, int *swap_count) {
   
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                int tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;
				(*swap_count)++;

            }
        }
    }
}

void rev_bubble_sort(int *array, int count, int *swap_count){

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (array[j] < array[j + 1]) {
                int tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;
                (*swap_count)++;
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

int check1 (int *array, int count) {

    for (int i = 0; i < count - 1; i++) {
        if (array[i] < array[i + 1]) {
            return 0;
        }
    }
    return 1;
}

int main() {

	SetConsoleCP(65001); 
	SetConsoleOutputCP(65001); 
	setlocale(LC_ALL, ".UTF8");

	int choice;
	int* numbers = NULL;
	int count;

	do{
		printf("\nМеню:\n");
        printf("1. Сгенерировать числа и записать в файл\n");
        printf("2. Отсортировать числа из файла\n");
        printf("3. Проверить сортировку чисел\n");
        printf("4. Сохранить отсортированные числа\n");
        printf("0. Выход\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);

		switch (choice) {
			case 1: {
				char filename[256];
				int num_count;
				int num_min;
				int num_max;

				printf("Введите название файла: ");
				scanf("%255s", &filename);
				printf("Введите количество чисел для генерации (до %d): ", MAX);
				scanf("%d", &num_count);
				printf("Введите диапазон генерации(через пробел): ");
				scanf("%d %d", &num_min, &num_max);
				if (num_count > 0 && num_count <= MAX) {
					gen_file(filename, num_count, num_min, num_max);
				} else {
					printf("Недопустимое количество чисел!\n");
				}
				break;
				
			}
			case 2: {
				if (numbers != NULL) {
					free(numbers);
				}
				char filename[256];
				printf("Введите название файла: ");
				scanf("%255s", filename);
				numbers = read_file(filename, &count);
				if (numbers != NULL && count > 0) {

					int swap_count = 0;

                    printf("Выберите тип сортировки:\n");
                    printf("1. По возрастанию\n");
                    printf("2. По убыванию\n");
                    int sort_type;
                    scanf("%d", &sort_type);

					clock_t start = clock();

					if (sort_type == 1){
						bubble_sort(numbers, count, &swap_count);
						printf("\nСортировка завершена за %.4f секунд.\n", (double)(clock() - start)/CLOCKS_PER_SEC);
						printf("Кол-во операций: %d\n", swap_count);
					}
					else if (sort_type == 2){
						rev_bubble_sort(numbers, count, &swap_count);
						printf("\nСортировка завершена за %.4f секунд.\n", (double)(clock() - start)/CLOCKS_PER_SEC);
						printf("Кол-во операций: %d\n", swap_count);
					}
				}
				break;
			}
			case 3: {
				if (numbers == NULL) {
					char filename[256];
					printf("Введите название файла: ");
					scanf("%255s", filename);
					numbers = read_file(filename, &count);
				}
				if (numbers != NULL && count > 0) {
                    if (check(numbers, count)) {
                        printf("\nЧисла отсортированы по возрастанию.\n");
                    } else if (check1(numbers, count)) {
                        printf("\nЧисла отсортированы по убыванию.\n");
                    } else{
						printf("\nЧисла не отсортированы.\n");
					}
                }
                break;

			}
			case 4: {
				if (numbers == NULL || count <= 0) {
					char filename[256];
					printf ("Введите назвиние файла для чтения: ");
					scanf("%255s", filename);
					numbers = read_file(filename, &count);
				}
				if (numbers != NULL && count > 0) {
					int sort = check(numbers,count);
					int rev_sort = check1(numbers,count);

					if (!sort && !rev_sort){
						printf("Числа не отсортированы! Хотите отсортировать перед сохранением?\n ");
						printf("Введите:\n1 - чтобы отсортировать по возрастанию\n2 - чтобы отсортировать по убыванию\nВаш выбор:");
						int s_choice;
						scanf("%d", &s_choice);

						int swap_count = 0;
						clock_t start = clock();
						
						if (s_choice == 1){
							bubble_sort(numbers, count, &swap_count);
							printf("\nСортировка завершена за %.4f секунд.\n", (double)(clock() - start)/CLOCKS_PER_SEC);
							printf("Кол-во операций: %d\n", swap_count);
						}
						else if (s_choice == 2){
							rev_bubble_sort(numbers, count, &swap_count);
							printf("\nСортировка завершена за %.4f секунд.\n", (double)(clock() - start)/CLOCKS_PER_SEC);
							printf("Кол-во операций: %d\n", swap_count);
						}
						else{
							printf("Неверный выбор.\n");
						}
					}
					else
					{
						printf ("Введите название файла для сохранения: ");
						char output_filename[256];
						scanf("%255s", &output_filename);

						FILE *file = fopen(output_filename, "w");
						if (file == NULL) {
							printf("Ошибка открытия файла!\n");
						} else {
							for (int i = 0; i < count; i++) {
								fprintf(file, "%d\n", numbers[i]);
							}
							fclose(file);
							printf("Успешно сохранено %d чисел в файле '%s'.\n", count, output_filename);
						}
					}
				}
				break;
			}
			
			case 0: {
				printf("Работа программы завершена.\n");
				break;
			}
			default: {
				printf("Неверный выбор.\n");
				break;
			}
		}

	} while (choice != 0);

	if (numbers != NULL) {
		free(numbers);
	}
	return 0;

}