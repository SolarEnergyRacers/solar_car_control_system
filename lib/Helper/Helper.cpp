//
// Helper Functions
//

#include "../../include/definitions.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "Helper.h"

#include <stdio.h>

char* fgets_stdio_blocking(char* str, int n){

    char c;
    int i = 0;
    do {
        c = getchar();
        if(c == 255){ // no char available to consume
            // taskYIELD(); // does not seem to do the trick
            vTaskDelay(100 / portTICK_PERIOD_MS);
        } else { // store char
            str[i++] = c;
        }
        printf("[%d:%d]", i, c);
    } while(i < (n-1) && c != '\n' && c != '\r');
    str[i] = 0; // add zero-termination

    // on success
    return str;
}
