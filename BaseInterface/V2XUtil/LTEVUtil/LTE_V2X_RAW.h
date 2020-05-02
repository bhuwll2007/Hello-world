#ifndef __LTE_V2X_RAW_H
#define __LTE_V2X_RAW_H

struct dsmp_msg_t
{
	unsigned int ueid;
	unsigned int psid;

	char power;
	unsigned char priority;
	unsigned char period;
	unsigned char proc_id;

	unsigned short plen;
	unsigned char test_mode;
	unsigned char test_sfn;
};

int Raw_init(const char *interface);

int Raw_release();

int RawSendData(int AID, char buf[], int buflen);

int RawRecvData(char buf[]);
#endif
