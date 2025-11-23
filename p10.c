#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int id;
    char name[50];
    char gender;
    int korean;
    int english;
    int math;
    int score_product;
} Student;

int sequential_search(Student students[], int size, int key) {
    int comparisons = 0;
    for (int i = 0; i < size; i++) {
        comparisons++;
        if (students[i].score_product == key) {
            break;
        }
    }
    return comparisons;
}

int selection_sort(Student students[], int size) {
    int comparisons = 0;
    for (int i = 0; i < size - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < size; j++) {
            comparisons++;
            if (students[j].score_product < students[min_idx].score_product) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            Student temp = students[i];
            students[i] = students[min_idx];
            students[min_idx] = temp;
        }
    }
    return comparisons;
}

int binary_search(Student students[], int size, int key) {
    int comparisons = 0;
    int low = 0;
    int high = size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        comparisons++;
        if (students[mid].score_product == key) {
            return comparisons;
        }

        comparisons++;
        if (students[mid].score_product < key) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    return comparisons;
}

int main() {
    FILE* file;
    if (fopen_s(&file, "dataset_id_ascending.csv", "r") != 0) {
        printf("오류: 파일을 열 수 없습니다.\n");
        getchar();
        return 1;
    }

    char line[256];

    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        getchar();
        return 1;
    }

    Student* students = NULL;
    int capacity = 100;
    int count = 0;

    students = (Student*)malloc(capacity * sizeof(Student));
    if (students == NULL) {
        fclose(file);
        getchar();
        return 1;
    }

    while (fgets(line, sizeof(line), file)) {
        if (count == capacity) {
            capacity *= 2;
            Student* new_students = (Student*)realloc(students, capacity * sizeof(Student));
            if (new_students == NULL) {
                break;
            }
            students = new_students;
        }

        if (sscanf_s(line, "%d,%49[^,],%c,%d,%d,%d",
            &students[count].id,
            students[count].name, 50,
            &students[count].gender, 1,
            &students[count].korean,
            &students[count].english,
            &students[count].math) == 6) {

            students[count].score_product =
                students[count].korean * students[count].english * students[count].math;

            count++;
        }
    }

    fclose(file);

    if (count == 0) {
        free(students);
        getchar();
        return 1;
    }

    srand(time(NULL));
    int search_key = rand() % 1000001;

    Student* seq_data = (Student*)malloc(count * sizeof(Student));
    if (seq_data == NULL) { free(students); getchar(); return 1; }
    memcpy(seq_data, students, count * sizeof(Student));

    Student* sorted_data = (Student*)malloc(count * sizeof(Student));
    if (sorted_data == NULL) { free(students); free(seq_data); getchar(); return 1; }
    memcpy(sorted_data, students, count * sizeof(Student));


    printf("============================================================\n");
    printf(" 데이터셋 정보\n");
    printf("============================================================\n");
    printf(" 읽어온 데이터 크기: %d명\n", count);
    printf(" 임의의 검색 키 (0~1,000,000): %d\n", search_key);
    printf("\n");

    int seq_comparisons = sequential_search(seq_data, count, search_key);

    printf("------------------------------------------------------------\n");
    printf(" 순차 탐색 (Sequential Search) 결과\n");
    printf("------------------------------------------------------------\n");
    printf(" 비교 횟수: %d\n", seq_comparisons);
    printf("\n");

    int sort_comparisons = selection_sort(sorted_data, count);
    int binary_comparisons = binary_search(sorted_data, count, search_key);
    int total_comparisons = sort_comparisons + binary_comparisons;

    printf("------------------------------------------------------------\n");
    printf(" 정렬 및 이진 탐색 (Sorting & Binary Search) 결과\n");
    printf("------------------------------------------------------------\n");
    printf(" 정렬 비교 횟수: %d\n", sort_comparisons);
    printf(" 이진 탐색 비교 횟수: %d\n", binary_comparisons);
    printf(" 합산 비교 횟수 (정렬 + 탐색): %d\n", total_comparisons);
    printf("============================================================\n");

    free(students);
    free(seq_data);
    free(sorted_data);

    getchar();

    return 0;
}