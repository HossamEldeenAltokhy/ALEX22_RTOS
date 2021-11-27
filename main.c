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


typedef struct{
    int data;
    int dataType;
    
}xData;

xQueueHandle Queue_Handler;
xTaskHandle T1_Handler;
// infinite Loop function

void vTask1(void* parameters) {

    // Define Variables
//        int* pv = (int *) parameters;
//        int sendValue = * pv;
    xData sendValue = *(xData *) parameters;

        

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
    xData receivedData ;
    portBASE_TYPE xStatus;
    char str1[]= " meters";
    char str2[]= " grams";
    while (1) {
        // Code 
        xStatus = xQueueReceive(Queue_Handler, &receivedData, 100/configTICK_RATE_HZ);

        if (xStatus == pdPASS) {
            uart_send_Num(receivedData.data);
//            uart_send_Num(receivedData.dataType);
            
            if(receivedData.dataType == 1){
                // some Code Task 1
                uart_send_str(str1);
            }else if (receivedData.dataType == 2){
                // Some Code Task 2
                uart_send_str(str2);
            }
            else{
                
            }
            uart_send('\r');
            
        }
        vTaskDelay(50);

    }

}

int main(void) {
    /* Replace with your application code */

    Queue_Handler = xQueueCreate(1, sizeof (xData));
    init_uart(9600, Rx_disable, Tx_enable);

//  DATA1
    xData data1;
    data1.data = 100;
    data1.dataType = 1;
//  DATA2
    xData data2;
    data2.data = 200;
    data2.dataType = 2;
    

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
