################################################################################
# ARM cross compiler for MK2
################################################################################

# Compile for arm architecture
ARCH = arm
export ARCH

# Set the CROSS_COMPILE
CROSS_COMPILE = ccache arm-linux-gnueabi-
export CROSS_COMPILE 

################################################################################
# Build config
################################################################################

# Comment/uncomment the following line to disable/enable debugging
DEBUG = n
ifeq ($(DEBUG),y)
	EXTRA_CFLAGS += -O -g -DDEBUG
else
	EXTRA_CFLAGS += -O2
endif

APP_DIR = ./
APP_BIN = $(APP_DIR)/test_TcpClient $(APP_DIR)/test_TcpServer $(APP_DIR)/test_UdpClient $(APP_DIR)/test_UdpServer $(APP_DIR)/test_1 $(APP_DIR)/test_2 $(APP_DIR)/test_3

# libNET
LIBNET_APP_DIR = ./../../NetUtil
           
#LDFLAGS_NET += -lpthread

LIB_DFLAGS := -lpthread

CFLAGS := -Wall -Werror -MD

################################################################################
# Build Rules
################################################################################
CC = $(CROSS_COMPILE)gcc

all: $(APP_BIN)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@
	
SRCS_TEST_TcpClient = ./Test_TcpClient.c ./TcpClient.c
OBJS_TEST_TcpClient = $(SRCS_TEST_TcpClient:.c=.o)

$(APP_DIR)/test_TcpClient: $(OBJS_TEST_TcpClient) 
	$(CC) $(CFLAGS) $(OBJS_TEST_TcpClient) -o $@ $(LIB_DFLAGS) $(INC_CFLAGS) 

SRCS_TEST_TcpServer = ./Test_TcpServer.c ./TcpServer.c
OBJS_TEST_TcpServer = $(SRCS_TEST_TcpServer:.c=.o)

$(APP_DIR)/test_TcpServer: $(OBJS_TEST_TcpServer) 
	$(CC) $(CFLAGS) $(OBJS_TEST_TcpServer) -o $@ $(LIB_DFLAGS) $(INC_CFLAGS) 
	
SRCS_TEST_UdpClient = ./Test_UdpClient.c ./UdpClient.c
OBJS_TEST_UdpClient = $(SRCS_TEST_UdpClient:.c=.o)

$(APP_DIR)/test_UdpClient: $(OBJS_TEST_UdpClient) 
	$(CC) $(CFLAGS) $(OBJS_TEST_UdpClient) -o $@ $(LIB_DFLAGS) $(INC_CFLAGS) 

SRCS_TEST_UdpServer = ./Test_UdpServer.c ./UdpService.c
OBJS_TEST_UdpServer = $(SRCS_TEST_UdpServer:.c=.o)

$(APP_DIR)/test_UdpServer: $(OBJS_TEST_UdpServer) 
	$(CC) $(CFLAGS) $(OBJS_TEST_UdpServer) -o $@ $(LIB_DFLAGS) $(INC_CFLAGS) 
	
SRCS_TEST_1 = ./Test_1.c 
OBJS_TEST_1 = $(SRCS_TEST_1:.c=.o)

$(APP_DIR)/test_1: $(OBJS_TEST_1) 
	$(CC) $(CFLAGS) $(OBJS_TEST_1) -o $@ $(LIB_DFLAGS) $(INC_CFLAGS) 
	
SRCS_TEST_2 = ./Test_2.c 
OBJS_TEST_2 = $(SRCS_TEST_2:.c=.o)

$(APP_DIR)/test_2: $(OBJS_TEST_2) 
	$(CC) $(CFLAGS) $(OBJS_TEST_2) -o $@ $(LIB_DFLAGS) $(INC_CFLAGS) 
	
SRCS_TEST_3 = ./Test_3.c 
OBJS_TEST_3 = $(SRCS_TEST_3:.c=.o)

$(APP_DIR)/test_3: $(OBJS_TEST_3) 
	$(CC) $(CFLAGS) $(OBJS_TEST_3) -o $@ $(LIB_DFLAGS) $(INC_CFLAGS) 
	
#SO:
#	ccache arm-linux-gnueabihf-gcc -fPIC -shared ./../../NETUtil/NET.c -o ./../../NETUtil/libMyNET.so
clean: 
	@rm -rf $(APP_BIN) *.o *.d $(LIBNET_APP_DIR)/*.o $(LIBNET_APP_DIR)/*.d
