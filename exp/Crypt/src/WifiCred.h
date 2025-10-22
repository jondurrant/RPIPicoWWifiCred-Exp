/*
 * WifiCred.h
 *
 *  Created on: 21 Oct 2025
 *      Author: jondurrant
 */

#ifndef EXP_PLAINTXT_SRC_WIFICRED_H_
#define EXP_PLAINTXT_SRC_WIFICRED_H_

#include "pico/stdlib.h"

#define CRED_LEN 35
#define CRED_JSON_LEN 256
#define CRED_KEY_LEN 24

class WifiCred {
public:

	static WifiCred * singleton();
	virtual ~WifiCred();

	bool isAvailable();

	void setSSID(const char *ssid);
	const char * getSSID();

	void setPWD(const char *pwd);
	const char *getPWD();

	void nuke();

	void commit();

	void setKey(uint8_t *p24Bytes);

	void genKey(char *str);


private:
	WifiCred();
	void readCred();
	static WifiCred * pSelf;
	char xSSID[CRED_LEN] ="";
	char xPWD[CRED_LEN] ="";
	uint8_t xKey[CRED_KEY_LEN] ={0};

};


#endif /* EXP_PLAINTXT_SRC_WIFICRED_H_ */

