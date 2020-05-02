#ifndef __BASEINTERFACE_DATASET_H_
#define __BASEINTERFACE_DATASET_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <fcntl.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <assert.h>
#include <inttypes.h>
#include <termios.h>

#include <sys/time.h> // for rate control
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <pthread.h>

#include <getopt.h>
#include <linux/if_ether.h>

#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netinet/if_ether.h>
#include <linux/sockios.h>

#include <stdint.h>
#include <stdbool.h>
#include <sys/select.h>
#include <linux/filter.h>
//#include <sys/msg.h>

//类型声明
typedef unsigned long long int uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef long long int nint64_t;
typedef int nint32_t;
typedef short nint16_t;
typedef char nint8_t;

#define FALSE -1
#define TRUE 0

#define MODE_LTEV_IP 0
#define MODE_LTEV_RAW 0
#endif /*__BASEINTERFACE_DATASET_H_*/
