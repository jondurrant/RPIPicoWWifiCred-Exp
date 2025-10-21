/*
 * CommsAgent.h
 *
 *  Created on: 21 Oct 2025
 *      Author: jondurrant
 */

#ifndef EXP_PLAINTXT_SRC_COMMSAGENT_H_
#define EXP_PLAINTXT_SRC_COMMSAGENT_H_

#include "pico/stdlib.h"
#include "Agent.h"
#include "tiny-json.h"



#define COMMS_BUFFER_LEN (1024 )

class CommsAgent : public Agent {
public:
	CommsAgent();
	virtual ~CommsAgent();

protected:
	/***
	 * Task main run loop
	 */
	virtual void run();

	/***
	 * Get the static depth required in words
	 * @return - words
	 */
	virtual configSTACK_DEPTH_TYPE getMaxStackSize();

private:
	size_t readLine(char * buffer, size_t size);

	char xBuffer[COMMS_BUFFER_LEN];

	json_t xJsonMem[10];
};

#endif /* EXP_PLAINTXT_SRC_COMMSAGENT_H_ */
