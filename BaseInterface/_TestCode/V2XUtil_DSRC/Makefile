################################################################################
# ARM cross compiler for MK2
################################################################################

# Compile for arm architecture
ARCH = arm
export ARCH

# Set the CROSS_COMPILE
CROSS_COMPILE = ccache arm-linux-gnueabihf-
export CROSS_COMPILE 

################################################################################
# Build config
################################################################################

# Comment/uncomment the following line to disable/enable debugging
DEBUG = n
ifeq ($(DEBUG),y)
  # "-O" is needed to expand inlines
	EXTRA_CFLAGS += -O -g -DDEBUG
else
	EXTRA_CFLAGS += -O2
endif

APP_DIR = ./
APP_BIN = $(APP_DIR)/Test_V2X_DSRC

LIBV2X_LTEV_APP_DIR := ./../../V2XUtil
LIBCONF_APP_DIR := ./../../ConfUtil

DEVMODE_DSRC = n
ifeq ($(DEVMODE_DSRC),y)
	SQLITES_DIR:=/wll/bin/sqlite3
else
	SQLITES_DIR:=/wll/bin/sqlite3_ltev
endif

INC_CFLAGS := -I$(SQLITES_DIR)/include
INC_CFLAGS += -I./

LIB_DFLAGS := -L${SQLITES_DIR}/lib -lsqlite3
LIB_DFLAGS += -L${LIBV2X_LTEV_APP_DIR} -lv2xutil
LIB_DFLAGS += -L${LIBCONF_APP_DIR} -lconfutil
LIB_DFLAGS += -lpthread

CFLAGS := -Wall -Werror -MD

################################################################################
# Build Rules
################################################################################
CC = $(CROSS_COMPILE)gcc

all: $(APP_BIN)

SRCS = ./Test_V2X_DSRC.c 
OBJS = $(SRCS:.c=.o)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@ $(INC_CFLAGS)

$(APP_DIR)/Test_V2X_DSRC: $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIB_DFLAGS) $(INC_CFLAGS)
	
clean: 
	@rm -rf $(APP_BIN) *.o *.d $(LIBCAN_APP_DIR)/*.o $(LIBCAN_APP_DIR)/*.d
	