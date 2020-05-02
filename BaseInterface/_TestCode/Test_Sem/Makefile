################################################################################
# ARM cross compiler for MK2
################################################################################

# Compile for arm architecture
ARCH = arm
export ARCH

# Set the CROSS_COMPILE
#CROSS_COMPILE = ccache arm-linux-gnueabihf-
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

APP_DIR = $(CURDIR)
APP_BIN = $(APP_DIR)/test_Sem

# libGPS
           
LDFLAGS_GPS += -lpthread

CFLAGS +=$(LDFLAGS_GPS) -Wall -Werror -MD

################################################################################
# Build Rules
################################################################################
CC = $(CROSS_COMPILE)gcc

all: $(APP_BIN)

SRCS_TEST_GPS = ./Test.c 
OBJS_TEST_GPS = $(SRCS_TEST_GPS:.c=.o)

$(APP_DIR)/test_Sem: $(OBJS_TEST_GPS) 
	$(CC) $(CFLAGS) $(OBJS_TEST_GPS) -o $@ $(LDFLAGS_GPS)
	
%.o: %.c 
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(INCLUDE) -c $< -o $@
	
SO:
	ccache arm-linux-gnueabihf-gcc -fPIC -shared ./../../GPSUtil/GPS.c -o ./../../GPSUtil/libMyGPS.so
clean: 
	@rm -rf $(APP_BIN) *.o *.d $(LIBGPS_APP_DIR)/*.o $(LIBGPS_APP_DIR)/*.d