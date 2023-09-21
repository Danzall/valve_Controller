/*
 * GSM.h
 *
 *  Created on: Apr 24, 2019
 *      Author: Jeefo
 */

#ifndef GSM_H_
#define GSM_H_

void GSM_Service();
void GSM_Send(char* data);
void Debug_Send(char* data);
void GSM_Init();
void sendSMS(char* num, char* msg);
void GSM_Receive(char in);
void recData();
void procData();
void OK();
void Context();
void checkGPRS();
void procSMS();
void buildInfo();
void buildTest();
void getSignal();
void caps(char* str);
void Error();
void pec_Update(char* pec, char index);
void GPRS_SendData(char* gprs);
void Socket1();
void DataStatus();
void DataDecrypt();
int Getvalue(char in);
void getIMEI();
void getCCID();
void Register();
void VoltageAlarm();
void Call();
void Balance();
void Network();
//void alarmFunc(Alarm input);
void Deny();
void GSM_Init();
char CheckUser();
void RegisterConfirm();
void SendChar(char data);
void BuildPower();
#include "motor.h"
void setEmail(event* log, int length, float ave);
void publish(uint8_t* data);
typedef enum{
	GSM_Off = 1,
	GSM_On,
	GSM_Enable,
	Search,
	GPRS_Search,
	Pin,
	Manufacturer,
	SMSconfig,
	Operator,
	Imei,
	Ccid,
	GprsContext,
	USSD_Config,
	USSD,
	CallerID,
	SMS_Check,
	SMS_Send,
	SMS_Text,
	GPRS_AttachCheck,
	GPRS_Attach,
	GPRS_On,
	GPRS_Check,
	GPRS_IP,
	GPRS_Off,
	SocketSetup,
	SocketOpen,
	SocketClose,
	SMS_Del,
	SMS_DelAll,
	Signal,
	SocketListen,
	SocketCheck,
	GPRS_Write,
	GPRS_Send,
	GPRS_SendMode,
	GPRS_SendModeTRans,
	AutoBaud,
	DataMode,
	GPRS_Text,
	EmailCid,
	EmailTO,
	SMTP_Server,
	SMTP_Authenticate,
	SMTP_From,
	SMTP_Recepient,
	SMTP_Subject,
	SMTP_BodyCMD,
	SMTP_Body,
	SMTP_Send,
	GSM_ClockSet,
	GSM_Clock,
	SaveSettings,
	MQTT_Start,
	MQTT_Acquire,
	MQTT_Connect,
	MQTT_SubTopic,
	MQTT_PubTopic,
	MQTT_PubPayload,
	MQTT_Pub,
	MQTT_PubDemo,
	MQTT_Disconnect,
	MQTT_Release,
	MQTT_Stop
}State;


typedef struct{
	int signal;
	int config;
	int send;
	int prompt;
	int content;
	int context;
	int gprsPending;
	int gprsActive;
	int del;
	int reply;
	int restartCount;
	int socket;
}SMSflags;

typedef struct{
	char index[4];
	char recMSISDN[25];
	int prompt;
	char content1[180];
	char IP[30];
	int count;
	char build[160];
}SMSinfo;

typedef struct{
	char url[30];
	char urlport[6];
	char imei[40];
	char ccid[40];
	int socket1;
	//int socket2;
	//int socket3;
	//int socket4;
	int socket;
	int registered;
	int gprsDataPending;
	int userFlag;
	int msisdnFlag;
	int passwordFlag;
	int alarm;
	char user[15];
	char user1[15];
	//char user2[15];
	char msisdn[20];
	char password[15];
	char network[25];
	char balance[15];
	int low;
	char vth[10];
	int vth_l;
	int vthFlag;
	int relayFlag;
	int ccidFlag;
	int activeFlag;
	int updateFlag;
	int modeFlag;
	int vThreshold;
	int ccidStatus;
	int balanceFlag;
	int balancePending;
	int GPRStimer;
	int GPRSinterval;
	char GPRSint[10];
	int imeiFlag;
	int dataFlag;
	int GPRS_Rec;
	int Prompt;
	int Timeout;
	int clock;
	char date[10];
	char time[15];
	char email;
	int mqtt;
	int mqttPaytload;
	int mqttPaytloadSize;
	int mqttPub;
}GSMinfo;
#endif /* GSM_H_ */
