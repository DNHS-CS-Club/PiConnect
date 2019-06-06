// MIT License
//
// Copyright (c) 2019 DNHS CS Club
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <wiringPi.h>

#define DELAY 60 //Delay in seconds between connection tests
#define PIN 0 //WiringPi GPIO pin number

#define STARTDAY 1 //Start day of week for connection testing in numerical format (0 - Sunday, 6 - Saturday)
#define STOPDAY 5 //End day of week
#define STARTHOUR 8 //Start hour for connection testing in 24-hour format
#define STOPHOUR 17 //End hour

void blink(void); //Flash sign to signal a change in state
int get_day(void); //Get current day of week in numerical format
int get_hour(void); //Get current hour in 24-hour format

int main(void) {
    int day = get_day(), hour = get_hour();
    int enabled = 1;

    //Set up GPIO
    wiringPiSetup();
    pinMode(PIN, OUTPUT);
    blink();

    while (1) {
        //Only run in alloted time frame
        if (day >= STARTDAY && day <= STOPDAY && hour >= STARTHOUR && hour < STOPHOUR) {
            //Signal enabling of sign if disabled
            if (!enabled) {
                enabled = 1;
                blink();
            }

            //Ping test website and toggle sign based on success
            digitalWrite(PIN, !system("ping -c3 example.com > /dev/null"));
        }
        //Signal disabling of sign in off-hours
        else if (enabled) {
            enabled = 0;
            blink();
        }

        sleep(DELAY);
        day = get_day();
        hour = get_hour();
    }

    return 0;
}

//Flash sign to signal a change in state
void blink(void) {
    int i;

    for (i = 0; i < 3; i++) {
        digitalWrite(PIN, LOW);
        sleep(1);
        digitalWrite(PIN, HIGH);
        sleep(1);
    }

    digitalWrite(PIN, LOW);
}

//Get current day of week in numerical format
int get_day(void) {
    time_t current_time = time(NULL);
    return localtime(&current_time)->tm_wday;
}

//Get current hour in 24-hour format
int get_hour(void) {
    time_t current_time = time(NULL);
    return localtime(&current_time)->tm_hour;
}
