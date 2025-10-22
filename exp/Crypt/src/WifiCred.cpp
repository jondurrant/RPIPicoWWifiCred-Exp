/*
 * WifiCred.cpp
 *
 *  Created on: 21 Oct 2025
 *      Author: jondurrant
 */

#include "WifiCred.h"

typedef unsigned char byte_workaround;
#define byte byte_workaround
#include <wolfssl/wolfcrypt/types.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/ecc.h>
#include <wolfssl/wolfcrypt/random.h>
#include <wolfssl/wolfcrypt/sha256.h>
#undef byte

#include "NVSOnboard.h"
#include <cstdio>

#include "json-maker/json-maker.h"
#include "tiny-json.h"
#include "pico/rand.h"
#include "pico/unique_id.h"

WifiCred::WifiCred() {
}

void WifiCred::readCred(){
	NVSOnboard *nvs = NVSOnboard::getInstance();

	char xJSON[CRED_JSON_LEN]={0};
	uint8_t xCipher[CRED_JSON_LEN]={0};
	size_t length = CRED_JSON_LEN;

	if (nvs->get_blob("wifi", (void*) xCipher, &length) != NVS_OK){
		printf("No Wifi Credentials in NVS\n");
		return;
	}
	printf("Cipher(%d):", length);
	for (int i=0; i < length; i++){
		if (i%8 == 0){
			printf("\n");
		}
		printf("%02X,",xCipher[i]);
	}
	printf("\n");

	const uint8_t iv[16] = {0};
	size_t ivLen = 16;
	nvs->get_blob("wifiIV",  ( void* )iv,  &ivLen);

	Aes dec;
	wc_AesInit(&dec, NULL, INVALID_DEVID);
	wc_AesSetKey(&dec, xKey, sizeof(xKey), iv, AES_DECRYPTION);
	wc_AesCbcDecrypt(&dec,  (uint8_t *)xJSON, xCipher,  length);

	printf("Decrypt: %s\n", xJSON);

	json_t xJsonMem[10];
	json_t const* json = json_create( xJSON, xJsonMem, sizeof xJsonMem / sizeof *xJsonMem );
	if ( !json ) {
			printf("Error json create: %s.\n", xJSON);
	} else {
		json_t const *item;
		item = json_getProperty(json, "ssid");
		if (item){
			setSSID(json_getValue(item));
		}

		item = json_getProperty(json, "pwd");
		if (item){
			setPWD(json_getValue(item));
		}
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

	printf("Set SSID to %s\n", xSSID);
}

const char * WifiCred::getSSID(){
	return xSSID;
}

void WifiCred::setPWD(const char *pwd){
	strcpy(xPWD, pwd);

	printf("Set PWD to %s\n", xPWD);
}

const char *WifiCred::getPWD(){
	return xPWD;
}

void WifiCred::commit(){
	NVSOnboard *nvs = NVSOnboard::getInstance();

	char xJSON[CRED_JSON_LEN];
	uint8_t xCipher[CRED_JSON_LEN]={0};


	size_t remLen = CRED_JSON_LEN;
	memset(xJSON, 0, CRED_JSON_LEN);
	char * dest = xJSON;
	dest = json_objOpen( dest, NULL, &remLen );
	dest = json_str( dest, "ssid",  xSSID, &remLen);
	dest = json_str( dest, "pwd", xPWD, &remLen );
	dest = json_objClose( dest,  &remLen );
	dest = json_end( dest,  &remLen);

	int l = strlen(xJSON);
	int l16 = (l/16 + 1) * 16;
	printf("JSON(%d, %d}: %s\n",
			l ,
			(l/16 + 1) * 16,
			xJSON);

	Aes enc;

	const uint8_t iv[16] = {0};
	get_rand_128 ((rng_128_t *) iv);
	nvs->set_blob("wifiIV",  (const void* )iv,  16);

	wc_AesInit(&enc, NULL, INVALID_DEVID);

	/*encrypt*/
	wc_AesSetKey(&enc, xKey, sizeof(xKey), iv, AES_ENCRYPTION);
	wc_AesCbcEncrypt(&enc, xCipher, (uint8_t *)xJSON,  l16);

	printf("Cipher:");
	for (int i=0; i < l16; i++){
		if (i%8 == 0){
			printf("\n");
		}
		printf("%02X,",xCipher[i]);
	}
	printf("\n");

	nvs->set_blob("wifi",  (const void* )xCipher,  l16);
	nvs->set_blob("wifiIV",  (const void* )iv,  16);
	nvs->commit();
	printf("Stored wifi credentials in blob\n");
	nvs->printNVS();
}


void WifiCred::setKey(uint8_t *p24Bytes){
	memcpy(xKey, p24Bytes, CRED_KEY_LEN);
	readCred();
}


void WifiCred::nuke(){
	strcpy(xSSID,"");
	strcpy(xPWD,"");
	NVSOnboard *nvs = NVSOnboard::getInstance();
	nvs->erase_all();
	nvs->commit();
	printf("NVS Wipped Clean\n");
}


void WifiCred::genKey(char *str){
	uint8_t hash[SHA256_DIGEST_SIZE];
	char buf[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];

	Sha256 sha;
	wc_InitSha256(&sha);

	// Then update with data
	wc_Sha256Update(&sha, (const unsigned char *)str, strlen(str));

	pico_get_unique_board_id_string (buf,  (2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1) );
	wc_Sha256Update(&sha, (const unsigned char *)buf, strlen(buf));

	// Finalize the hash
	wc_Sha256Final(&sha, hash);

	memcpy(xKey, hash, CRED_KEY_LEN);
	readCred();
}
