#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <wiringPi.h>

#define DELAY 60 //Delay in seconds between connection tests
#define PIN 0 //WiringPi GPIO pin number

#define STARTDAY 1 //Start day of week for connection testing in numerical format (0 - Sunday, 6 - Saturday)
#define STOPDAY 6 //End day of week
#define STARTHOUR 8 //Start hour for connection testing in 24-hour format
#define STOPHOUR 16 //End hour

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

            //Ping Google DNS and toggle sign based on success
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
