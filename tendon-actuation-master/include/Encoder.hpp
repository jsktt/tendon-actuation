#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

#define GEAR_RATIO      100
#define COUNTS_PER_REV  12 * GEAR_RATIO
#define DEG_PER_REV     360

template <uint8_t encA, uint8_t encB>

class Encoder {
    static volatile int ticks;
    static volatile int lastEncoded;

    public:
        static void init() {
            pinMode(encA, INPUT_PULLUP);
            pinMode(encB, INPUT_PULLUP);
            attachInterrupt(digitalPinToInterrupt(encA), ticked, CHANGE);
            attachInterrupt(digitalPinToInterrupt(encB), ticked, CHANGE);
        }

        static void ticked() {
            int phaseA = digitalRead(encA);
            int phaseB = digitalRead(encB);

            int currentEncoded = (phaseA << 1) | phaseB;
            int sum = (lastEncoded << 2) | currentEncoded;

            switch(sum){
                case 0b0001:
                case 0b0111:
                case 0b1110:
                case 0b1000:
                ticks--;
                break;
                case 0b0010:
                case 0b1011:
                case 0b1101:
                case 0b0100:
                ticks++;
                break;
            }
            lastEncoded = currentEncoded;
        }

        static int getTicks() {
            return ticks;
        }

        static void resetCount() {
            ticks = 0;
        }
};

template <uint8_t encA, uint8_t encB>
volatile int Encoder<encA, encB>::ticks;

template <uint8_t encA, uint8_t encB>
volatile int Encoder<encA, encB>::lastEncoded;

#endif