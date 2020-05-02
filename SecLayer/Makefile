CUR_DIR = ./

SECLAYER_DIR := $(CUR_DIR)

# BaseInterface dir
BASEINTERFACE_DIR := $(CUR_DIR)../BaseInterface/
# NetLayer dir
NETLAYER_DIR := $(CUR_DIR)../NetLayer/
# SecFrame dir
SECFRAME_DIR := $(CUR_DIR)../SecFrame/

MODULE_HEADERS+=$(SECLAYER_DIR)Encode_SecMsg.h
MODULE_SOURCES+=$(SECLAYER_DIR)Encode_SecMsg.c

MODULE_HEADERS+=$(SECLAYER_DIR)Verify_SecMsg.h
MODULE_SOURCES+=$(SECLAYER_DIR)Verify_SecMsg.c

MODULE_HEADERS+=$(SECLAYER_DIR)FunCommun.h
MODULE_SOURCES+=$(SECLAYER_DIR)FunCommun.c

MODULE_HEADERS+=$(SECLAYER_DIR)SecLayer_Deal.h
MODULE_SOURCES+=$(SECLAYER_DIR)SecLayer_Deal.c

################################################################################
TARGET = libseclayer.so

EXTRA_CFLAGS += -I $(BASEINTERFACE_DIR)
EXTRA_CFLAGS += -I $(NETLAYER_DIR)
EXTRA_CFLAGS += -I $(SECFRAME_DIR)

LDFLAGS += -L$(BASEINTERFACE_DIR)BaseFunct -lbasefunct \
	   -L$(BASEINTERFACE_DIR)ConfUtil -lconfutil \
	   -L$(BASEINTERFACE_DIR)V2XUtil -lv2xutil \
	   -L$(NETLAYER_DIR) -lnetlayer 
EXTRA_CFLAGS += -I../BaseInterface/V2XUtil/LTEV_ZTE_Util/include

OBJS := ${MODULE_SOURCES:.c=.o}

################################################################################
# Build Rules
################################################################################
CC := $(CROSS_COMPILE)gcc

all: $(TARGET)

$(TARGET): ${OBJS}
	$(CC) -shared $(CFLAGS) -o $(TARGET) ${OBJS} ${LDFLAGS}

.SUFFIXES:
.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ $(EXTRA_CFLAGS)
clean:
	rm -f $(TARGET)
	#rm -f $(OBJS)
	find ./ -name "*.o" |xargs rm -rf
	find ./ -name "*.d" |xargs rm -rf
