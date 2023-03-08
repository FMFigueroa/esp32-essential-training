#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    char c = 0;                  // capturing from the keyboard
    char str[100];               // in this array push those characters that you got from the keyboard onto.
    memset(str, 0, sizeof(str)); // it keep to capturing input until the carriage return (enter key) is hit.
    while (c != '\n')
    {
        c = getchar(); //Getchar returns an integer, which is going to be the key.
        if (c != 0xff) // is the character a space?
        {
            str[strlen(str)] = c; //capture a character and concatenate it to our string.
            printf("%c", c); // print it out as a character on one line.
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
        /**
         * @brief put a little delay in here that will make sure that the watchdog timer doesn't get upset with us because we're sort of sitting in an infinite loop, and also gives the keys time to respond and get an "up press".
         *
         */
    }

    printf("you typed: %s\n", str); // string print stored into array
}