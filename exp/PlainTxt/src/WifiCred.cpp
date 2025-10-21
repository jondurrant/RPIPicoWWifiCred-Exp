/*
 * WifiCred.cpp
 *
 *  Created on: 21 Oct 2025
 *      Author: jondurrant
 */

#include "WifiCred.h"

#include "NVSOnboard.h"
#include <cstdio>

WifiCred::WifiCred() {
	NVSOnboard *nvs = NVSOnboard::getInstance();
	size_t sz = CRED_LEN;
	if(nvs->get_str("ssid", xSSID, &sz) != NVS_OK){
		strcpy(xSSID, "");
	} else {
		printf("Read SSID from NVS %s\n", xSSID);
	}

	sz = CRED_LEN;
	if(nvs->get_pwd("pwd", xPWD, &sz) != NVS_OK){
		strcpy(xPWD, "");
	} else {
		printf("Read PWD from NVS %s\n", xPWD);
	}
}

WifiCred * WifiCred::pSelf = NULL;
WifiCred * WifiCred::singleton(){
	if (pSelf == NULL){
		pSelf = new WifiCred;
	}
	return pSelf;
}

WifiCred::~WifiCred() {
	// TODO Auto-generated destructor stub
}

bool WifiCred::isAvailable(){
	return (strlen(xSSID) > 0);
}

void WifiCred::setSSID(const char *ssid){
	strcpy(xSSID, ssid);

	NVSOnboard *nvs = NVSOnboard::getInstance();
	if(nvs->set_str("ssid", xSSID) == NVS_OK){
		nvs->commit();
	}

	printf("Set SSID to %s\n", xSSID);
}

const char * WifiCred::getSSID(){
	return xSSID;
}

void WifiCred::setPWD(const char *pwd){
	strcpy(xPWD, pwd);

	NVSOnboard *nvs = NVSOnboard::getInstance();
	if(nvs->set_pwd("pwd", xPWD) == NVS_OK){
		nvs->commit();
	}

	printf("Set PWD to %s\n", xPWD);
}

const char *WifiCred::getPWD(){
	return xPWD;
}

void WifiCred::nuke(){
	strcpy(xSSID,"");
	strcpy(xPWD,"");
	NVSOnboard *nvs = NVSOnboard::getInstance();
	nvs->erase_all();
	nvs->commit();
	printf("NVS Wipped Clean\n");
}
