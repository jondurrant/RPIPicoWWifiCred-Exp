/**
 * Calculate the value of PI using 4 Worker threads across 2 cores
 * Jon Durrant - 2024
 */


#include "pico/stdlib.h"

#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include "pico/cyw43_arch.h"

#include "wolfssl/ssl.h"
#include <FreeRTOS.h>
#include <task.h>

#include "WifiHelper.h"
#include "NVSOnboard.h"
#include "WifiCred.h"
#include "CommsAgent.h"
#include "pico/unique_id.h"


#define TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define SSID_LEN 30



void runTimeStats(   ){
	TaskStatus_t *pxTaskStatusArray;
	volatile UBaseType_t uxArraySize, x;
	unsigned long ulTotalRunTime;

   // Get number of takss
   uxArraySize = uxTaskGetNumberOfTasks();
   printf("Number of tasks %d\n", uxArraySize);

   //Allocate a TaskStatus_t structure for each task.
   pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

   if( pxTaskStatusArray != NULL ){
		  // Generate raw status information about each task.
		  uxArraySize = uxTaskGetSystemState( pxTaskStatusArray,
									 uxArraySize,
									 &ulTotalRunTime );

		 // Print stats
		 for( x = 0; x < uxArraySize; x++ )
		 {
			 printf("Task: %d \t cPri:%d \t bPri:%d \t hw:%d \t%s\n",
					pxTaskStatusArray[ x ].xTaskNumber ,
					pxTaskStatusArray[ x ].uxCurrentPriority ,
					pxTaskStatusArray[ x ].uxBasePriority ,
					pxTaskStatusArray[ x ].usStackHighWaterMark ,
					pxTaskStatusArray[ x ].pcTaskName
					);
		 }


		  // Free array
		  vPortFree( pxTaskStatusArray );
   } else {
	   printf("Failed to allocate space for stats\n");
   }

   //Get heap allocation information
   HeapStats_t heapStats;
   vPortGetHeapStats(&heapStats);
   printf("HEAP avl: %d, blocks %d, alloc: %d, free: %d\n",
		   heapStats.xAvailableHeapSpaceInBytes,
		   heapStats.xNumberOfFreeBlocks,
		   heapStats.xNumberOfSuccessfulAllocations,
		   heapStats.xNumberOfSuccessfulFrees
		   );
}


char ssid[SSID_LEN];
char pwd[SSID_LEN];
uint8_t key[24] = {
		1,2,3,4,5,6,7,8,
		1,2,3,4,5,6,7,8,
		1,2,3,4,5,6,7,8
};

void debugCB(const int logLevel, const char *const logMessage){
	printf("WOLFSSL DEBUG(%d): %s\n", logLevel, logMessage);
}

void wifi_task(void* params){
	char buf[80];
	wolfSSL_Init();
	wolfSSL_SetLoggingCb( debugCB);
	//wolfSSL_Debugging_ON();

	WifiCred::singleton()->setKey(key);

	WifiHelper::init();

	//NVSAgent::getInstance()->start();
	NVSOnboard *nvs = NVSOnboard::getInstance();
	nvs->printNVS();

	for (;;){
		//runTimeStats();
		if (! WifiHelper::isJoined()){
			pico_get_unique_board_id_string (buf, 80);
			printf("Pico ID: %s\n", buf);
			WifiHelper::getMACAddressStr(buf);
			printf("MAC %s\n",  buf);

			if (WifiCred::singleton()->isAvailable()){
				WifiHelper::join(
						WifiCred::singleton()->getSSID(),
						WifiCred::singleton()->getPWD()
				);
				WifiHelper::getIPAddressStr(buf);
				printf("IP Address %s\n", buf);
			}
		}



		vTaskDelay(1000);
	}

}

void main_task(void* params){

	printf("Main task started\n");

	CommsAgent comms;

	comms.start("Comms");

  for (;;){

	  vTaskDelay(3000);
  }
}




int main() {


	//Initialise IO as we are using printf for debug
	stdio_init_all();

	sleep_ms(2000);
	printf("Start\n");

	TaskHandle_t mainTask,  wifiTask;

   xTaskCreate(main_task, "MainThread", 1024*4, NULL, TASK_PRIORITY, &mainTask);
   xTaskCreate(wifi_task, "WifiThread", 1024*2, NULL, TASK_PRIORITY, &wifiTask);

  /* Start the tasks and timer running. */
	vTaskStartScheduler();

	for (;;){

	}
	return 0;
}
