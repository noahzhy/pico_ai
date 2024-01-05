// #include "pico/stdlib.h"
#include <stdio.h>
#include <math.h>


// dict -> {0: ' ', 1: 'a', 2: 'b', 3: 'c', 4: 'd', 5: 'e', ...}
void decode(int* input, char* output, int len) {
    char dict[69] = {
        '-', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 
        'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 
        't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', 
        '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 
        'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 
        'X', 'Y', 'Z', ',', '.', '?', '!', ' '};
    for (int i = 0; i < len; i++) {
        output[i] = dict[input[i]];
    }
}


void argmax(float* input, int* output, int T, int C) {
    int max_idx = 0;
    float max_val = 0;
    for (int t = 0; t < T; t++) {
        max_idx = 0;
        max_val = 0;
        for (int c = 0; c < C; c++) {
            if (input[t * C + c] > max_val) {
                max_val = input[t * C + c];
                max_idx = c;
            }
        }
        output[t] = max_idx;
    }
}

// group_by
// input: [1, 1, 2, 3, 3, 5, 3, 3, 4]
// output: [1, 2, 3, 5, 3, 4]
void group_by(int arr[], int *size) {
    if (*size <= 1) {
        // No consecutive duplicates to remove
        return;
    }
    int index = 1;
    for (int i = 1; i < *size; i++) {
        // If the current element is different from the previous one, add it to the result
        if (arr[i] != arr[i - 1]) {
            arr[index++] = arr[i];
        }
    }
    *size = index; // Update the size of the array
}


void ctc_decode(float* input, int* output, int T, int C, int* size) {
    argmax(input, output, T, C);
    group_by(output, size);
}


int main() {

    int T = 16;
    int C = 69;
    float logits[T * C];
    int output[T];

    for (int t = 0; t < T; t++) {
        for (int c = 0; c < C; c++) {
            logits[t * C + c] = (float) (rand() % 10);
        }
    }

    int size = T;
    ctc_decode(logits, output, T, C, &size);

    for (int t = 0; t < size; t++) {
        printf("%d ", output[t]);
    }

}