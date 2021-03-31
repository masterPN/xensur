/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <cstdio>


// Declare
AnalogIn ldr1(A4);
AnalogIn ldr2(A5);
AnalogIn ldr3(A6);

I2CSlave slave(D4, D5);

int state = 0;
int ldr1T = 0;
int ldr2T = 0;
int ldr3T = 0;

void calibrate(){
    ThisThread::sleep_for(2000ms);

    int ldr1T = ldr1.read_u16();
    int ldr2T = ldr2.read_u16();
    int ldr3T = ldr3.read_u16();

    ldr1T += -8000; 
    ldr2T += -8000;
    ldr3T += -8000;

    state = 0;
}

int main()
{
    char buf[20];
    printf("LDR ready...\n");
    slave.address(0xA0);

    while (true) {
        int i = slave.receive();
        for(int i=0; i<sizeof(buf); i++){
            buf[i] = 0;
        }
        switch(i){
            case I2CSlave::ReadAddressed:
                break;
            case I2CSlave::WriteAddressed:
                slave.read(buf, sizeof(buf)-1);

                int num = atoi(buf);
                printf("Read: %d\n", num);
                state = num;
        }

        if(state==1){
            //countscore
            printf("start count score\n");
        }else if(state==2){
            printf("start calibrate\n");
            calibrate();
        }
    }
}