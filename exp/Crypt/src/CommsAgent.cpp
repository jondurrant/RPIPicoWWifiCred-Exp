/*
 * CommsAgent.cpp
 *
 *  Created on: 21 Oct 2025
 *      Author: jondurrant
 */

#include "CommsAgent.h"

#include <cstdio>

#include "WifiCred.h"


#define COMMS_TIMEOUT 250000000

CommsAgent::CommsAgent() {
	// TODO Auto-generated constructor stub

}

CommsAgent::~CommsAgent() {
	// TODO Auto-generated destructor stub
}

void CommsAgent::run(){
	 for (;;){

		  size_t l = readLine(xBuffer, COMMS_BUFFER_LEN);
		  if (l > 0){
				//comms.writeLine(buffer, l);
			  	printf(">%s\n", xBuffer);
			    json_t const* json = json_create( xBuffer, xJsonMem, sizeof xJsonMem / sizeof *xJsonMem );
				if ( !json ) {
						printf("Error json create: %s.\n", xBuffer);
				} else {
					json_t const *item;
					size_t len;
					bool commit = false;
					item = json_getProperty(json, "ssid");
					if (item){
						WifiCred::singleton()->setSSID(json_getValue(item));
						commit = true;
					}

					item = json_getProperty(json, "pwd");
					if (item){
						WifiCred::singleton()->setPWD(json_getValue(item));
						commit = true;
					}

					item = json_getProperty(json, "nuke");
					if (item){
						WifiCred::singleton()->nuke();
					}

					if (commit){
						WifiCred::singleton()->commit();
					}

				}
		  }

		  vTaskDelay(100);
	 }

}

size_t CommsAgent::readLine(char * buffer, size_t size){
	int c;
	int i = 0;
	c = stdio_getchar_timeout_us (COMMS_TIMEOUT);
	while (c > 0){
		if ((c == '\n') || (c=='\r')){
			break;
		}
		buffer[i] = c;
		i++;
		if (i == size){
			break;
		}
		c = stdio_getchar_timeout_us (COMMS_TIMEOUT);
	}
	buffer[i] = 0;
	return i;
}

/***
 * Get the static depth required in words
 * @return - words
 */
configSTACK_DEPTH_TYPE CommsAgent::getMaxStackSize(){
	return 1024;
}

