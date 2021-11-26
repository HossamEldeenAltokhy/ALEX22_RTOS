/*
 * File:   main.c
 * Author: hossa
 *
 * Created on November 20, 2021, 3:49 PM
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include "mDrivers/config.h"
#include "mDrivers/mADC.h"
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "mDrivers/mUART.h"
#include "mDrivers/mIO.h"
#include "mDrivers/mADC.h"
#include <util/delay.h>

char str1[] = "Task1 is running.\r";
char str2[] = "Task2 is running.\r";

xTaskHandle T1_Handler;
// infinite Loop function
void vTask1(void* parameters){
    
    // Define Variables
    int i = 0;
    while(1){
        // Code 
        i++;
        uart_send_str(str1);
        uart_send('\r');
        vTaskDelay(2);
        if(i==5){
            vTaskSuspend(NULL);
        }
    }
    
   // vTaskDelete(NULL);
}

void vTask2(void* parameters){
    
    // Define Variables
    static int y =0;
    while(1){
        // Code 
//        uart_send_str(str2);
//        uart_send('\r');
//        vTaskDelay(2);
        y++;
        togglePortData(_PA);
        vTaskDelay(50);
        if(y == 20){
            vTaskResume(T1_Handler);
            y =0;
        }
    }
    
    vTaskDelete(NULL);
}

int main(void) {
    /* Replace with your application code */
    setPortDir(_PA, OUT);
    init_uart(9600, Rx_disable, Tx_enable);
    
 
    
    xTaskCreate(vTask2,       /**Task to be called**/
                "printer",   //** Task Name **//
                100,        //** Task Stack size **//
                NULL,         //** Parameters Address **//
                2,          //** Task Priority Level **//
                &T1_Handler // ** Task Handler **//
                );
    
    xTaskCreate(vTask1,       /**Task to be called**/
                "search",   //** Task Name **//
                100,        //** Task Stack size **//
                NULL,         //** Parameters Address **//
                2,          //** Task Priority Level **//
                NULL // ** Task Handler **//
                );
    
    vTaskStartScheduler();
    
    
    

    
   
    while (1) {
   
    }
}
