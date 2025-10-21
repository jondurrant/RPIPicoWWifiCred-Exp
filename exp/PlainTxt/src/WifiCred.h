/*
 * WifiCred.h
 *
 *  Created on: 21 Oct 2025
 *      Author: jondurrant
 */

#ifndef EXP_PLAINTXT_SRC_WIFICRED_H_
#define EXP_PLAINTXT_SRC_WIFICRED_H_

#define CRED_LEN 35

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


private:
	WifiCred();
	static WifiCred * pSelf;
	char xSSID[CRED_LEN] ="";
	char xPWD[CRED_LEN] ="";

};


#endif /* EXP_PLAINTXT_SRC_WIFICRED_H_ */

