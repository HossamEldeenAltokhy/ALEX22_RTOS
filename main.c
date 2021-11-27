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
#include "FreeRTOS/queue.h"
#include "mDrivers/mUART.h"
#include "mDrivers/mIO.h"
#include "mDrivers/mADC.h"
#include <util/delay.h>



xQueueHandle Queue_Handler;
xTaskHandle T1_Handler;
// infinite Loop function

void vTask1(void* parameters) {

    // Define Variables
//        int* pv = (int *) parameters;
//        int sendValue = * pv;
    int sendValue = *(int *) parameters;

        

    while (1) {
        // Code 

        xQueueSendToFront(Queue_Handler, &sendValue, 0);
//        vTaskDelay(50);

        taskYIELD();
    }

    // vTaskDelete(NULL);
}

void vTask2(void* parameters) {

    // Define Variables
    int receivedData ;
    portBASE_TYPE xStatus;
    while (1) {
        // Code 
        xStatus = xQueueReceive(Queue_Handler, &receivedData, 100/configTICK_RATE_HZ);

        if (xStatus == pdPASS) {
            uart_send_Num(receivedData);
            uart_send('\r');
            
        }
        vTaskDelay(50);

    }

}

int main(void) {
    /* Replace with your application code */

    Queue_Handler = xQueueCreate(1, sizeof (int));
    init_uart(9600, Rx_disable, Tx_enable);

//
    int data1 = 100;
    int data2 = 200;
    int data3 = 300;

    if (Queue_Handler)
    {

        xTaskCreate(vTask1, /**Task to be called**/
                "sender", //** Task Name **//
                100, //** Task Stack size **//
                (void *) &data1, //** Parameters Address **//
                1, //** Task Priority Level **//
                NULL // ** Task Handler **//
                );
    

        xTaskCreate(vTask1, /**Task to be called**/
                "sender", //** Task Name **//
                100, //** Task Stack size **//
                (void *) &data2, //** Parameters Address **//
                1, //** Task Priority Level **//
                NULL // ** Task Handler **//
                );

      
    

   
       


        xTaskCreate(vTask2, /**Task to be called**/
                "receive", //** Task Name **//
                100, //** Task Stack size **//
                NULL, //** Parameters Address **//
                1, //** Task Priority Level **//
                NULL // ** Task Handler **//
                );

        vTaskStartScheduler();


    }







    while (1) {


    }
}
