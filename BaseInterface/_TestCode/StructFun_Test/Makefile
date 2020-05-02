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
APP_BIN = $(APP_DIR)/test_StructFun 

# libStructFun
#LIBStructFun_APP_DIR = ./../../StructFunUtil
#LIBCONF_APP_DIR = ./../../ConfUtil
#LIBStructFun_APP_BIN = $(LIBStructFun_APP_DIR)/libMyStructFun.so
           
LDFLAGS_StructFun += -lpthread
           
LIBS +=$(LIBNL_APP_BIN) $(LIBCFG_APP_BIN) $(LIB1609_APP_BIN) $(Libs) 
 
CFLAGS +=$(LDFLAGS_StructFun) -Wall -Werror -MD

EXTRA_CFLAGS += -I. \
				-I$(prefix)/include
################################################################################
# Build Rules
################################################################################
CC = $(CROSS_COMPILE)gcc

all: $(APP_BIN)

SRCS_TEST_StructFun = ./Test.c 
OBJS_TEST_StructFun = $(SRCS_TEST_StructFun:.c=.o)

$(APP_DIR)/test_StructFun: $(OBJS_TEST_StructFun) 
	$(CC) $(CFLAGS) $(OBJS_TEST_StructFun) -o $@ $(LDFLAGS_StructFun)
	
%.o: %.c 
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(INCLUDE) -c $< -o $@
	
SO:
	ccache arm-linux-gnueabihf-gcc -fPIC -shared ./../../StructFunUtil/StructFun.c -o ./../../StructFunUtil/libMyStructFun.so
clean: 
	@rm -rf $(APP_BIN) *.o *.d $(LIBStructFun_APP_DIR)/*.o $(LIBStructFun_APP_DIR)/*.d