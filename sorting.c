//sorting.c: Multithreaded sorting program
//Name: Adrian Zacapala
//Group: 7
//Assignment: Prgramming assignment #2, Threads

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define SIZE 11

int list[] = {7, 12, 19, 3, 18, 4, 2, -5, 6, 15, 8};
int result[SIZE];
typedef struct {
    int *sub_array;
    unsigned int size;
}

sorting_thread_parameters;

typedef struct {
    sorting_thread_parameters left;
    sorting_thread_parameters right;
}

merging_thread_parameters;

void *sort_array(void *param) {
    sorting_thread_parameters *args = (sorting_thread_parameters *)param;
    for (unsigned int i = 0; i < args->size - 1; i++) {
        for (unsigned int j = 0; j < args->size - i - 1; j++) {
            if (args->sub_array[j] > args->sub_array[j + 1]) {
                int temp = args->sub_array[j];
                args->sub_array[j] = args->sub_array[j + 1];
                args->sub_array[j + 1] = temp;
            }
        }
    }
    pthread_exit(NULL);
}

void *merge_arrays(void *param) {
    merging_thread_parameters *args = (merging_thread_parameters *)param;
    int left_index = 0, right_index = 0, result_index = 0;
    while (left_index < args->left.size && right_index < args->right.size) {
        if (args->left.sub_array[left_index] < args->right.sub_array[right_index]) {
            result[result_index++] = args->left.sub_array[left_index++];
        } else {
            result[result_index++] = args->right.sub_array[right_index++];
        }
    }

    while (left_index < args->left.size) {
        result[result_index++] = args->left.sub_array[left_index++];
    }

    while (right_index < args->right.size) {
        result[result_index++] = args->right.sub_array[right_index++];
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t thread_left, thread_right, thread_merge;
    sorting_thread_parameters *params_left = malloc(sizeof(sorting_thread_parameters));
    sorting_thread_parameters *params_right = malloc(sizeof(sorting_thread_parameters));
    merging_thread_parameters *params_merge = malloc(sizeof(merging_thread_parameters));
    
    params_left->sub_array = list;
    params_left->size = SIZE / 2;
    params_right->sub_array = list + params_left->size;
    params_right->size = SIZE - params_left->size;
    
    pthread_create(&thread_left, NULL, sort_array, params_left);
    pthread_create(&thread_right, NULL, sort_array, params_right);
    pthread_join(thread_left, NULL);
    pthread_join(thread_right, NULL);
    
    params_merge->left = *params_left;
    params_merge->right = *params_right;
    
    pthread_create(&thread_merge, NULL, merge_arrays, params_merge);
    pthread_join(thread_merge, NULL);
    
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", result[i]);
    }

    printf("\n");
    
    free(params_left);
    free(params_right);
    free(params_merge);
    
    return 0;
}