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
APP_BIN = $(APP_DIR)/test_C $(APP_DIR)/test_S

# libNET
LIBNET_APP_DIR = ./../../NetUtil
           
#LDFLAGS_NET += -lpthread

DEVMODE_DSRC = n
ifeq ($(DEVMODE_DSRC),y)
	SQLITES_DIR:=/wll/bin/sqlite3
else
	SQLITES_DIR:=/wll/bin/sqlite3_ltev
endif

INC_CFLAGS :=# -I$(SQLITES_DIR)/include
INC_CFLAGS += -I./

#LIB_DFLAGS := -L${SQLITES_DIR}/lib -lsqlite3
#LIB_DFLAGS += -L${LIBNET_APP_DIR} -lnetutil
LIB_DFLAGS += -lpthread

CFLAGS := -Wall #-Werror -MD

################################################################################
# Build Rules
################################################################################
CC = $(CROSS_COMPILE)gcc

all: $(APP_BIN)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@
	
SRCS_TEST_C = ./Test_C.c 
OBJS_TEST_C = $(SRCS_TEST_C:.c=.o)

$(APP_DIR)/test_C: $(OBJS_TEST_C) 
	$(CC) $(CFLAGS) $(OBJS_TEST_C) -o $@ $(LIB_DFLAGS) $(INC_CFLAGS) 

SRCS_TEST_S = ./Test_S.c 
OBJS_TEST_S = $(SRCS_TEST_S:.c=.o)

$(APP_DIR)/test_S: $(OBJS_TEST_S) 
	$(CC) $(CFLAGS) $(OBJS_TEST_S) -o $@ $(LIB_DFLAGS) $(INC_CFLAGS) 

clean: 
	@rm -rf $(APP_BIN) *.o
