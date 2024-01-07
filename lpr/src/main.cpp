#include <math.h>
#include <stdio.h>

#include "ml_model.h"
#include "model_settings.h"
#include "pico/stdlib.h"
#include "tflite_model.h"

MLModel ml_model(tflite_model, 128 * 1024);

int8_t *scaled_spectrum = nullptr;
int32_t spectogram_divider;
float spectrogram_zero_point;

// dict -> {0: ' ', 1: 'a', 2: 'b', 3: 'c', 4: 'd', 5: 'e', ...}
void decode(int *input, char *output, int len) {
    char dict[69] = {'-', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
                     'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
                     'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8',
                     '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
                     'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
                     'X', 'Y', 'Z', ',', '.', '?', '!', ' '};
    for (int i = 0; i < len; i++) {
        output[i] = dict[input[i]];
    }
}

void argmax(int *input, int *output, int T, int C) {
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
        // If the current element is different from the previous one, add it to
        // the result
        if (arr[i] != arr[i - 1]) {
            arr[index++] = arr[i];
        }
    }
    *size = index; // Update the size of the array
}

void ctc_decode(int *input, int *output, int T, int C, int *size) {
    argmax(input, output, T, C);
    group_by(output, size);
}

int main() {
    // initialize stdio
    stdio_init_all();

    if (!ml_model.init()) {
        printf("Failed to initialize ML model!\n");
        while (1) {
            tight_loop_contents();
        }
    }

    scaled_spectrum = (int8_t *)ml_model.input_data();
    spectogram_divider = 64 * ml_model.input_scale();
    spectrogram_zero_point = ml_model.input_zero_point();

    int8_t *prediction = ml_model.predict();
    // printf("Prediction: %s\n", &prediction);
    // // 2d array of shape (T, C)
    // int8_t prediction_2d[time_steps][num_classes];
    // for (int t = 0; t < time_steps; t++) {
    //     for (int c = 0; c < num_classes; c++) {
    //         prediction_2d[t][c] = prediction[t * num_classes + c];
    //     }
    // }

    int size = time_steps;
    int output[time_steps];
    ctc_decode((int*)prediction, output, time_steps, num_classes, &size);

    for (int t = 0; t < size; t++) {
        printf("%d ", output[t]);
    }
}