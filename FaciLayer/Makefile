CUR_DIR = ./

# FaciLayer dir
FACILAYER_DIR := $(CUR_DIR)

# BaseInterface dir
BASEINTERFACE_DIR := $(CUR_DIR)../BaseInterface/

# MsgFrame dir
MSGFRAME_DIR := $(CUR_DIR)../MsgFrame/

# NetLayer dir
NETLAYER_DIR = $(CUR_DIR)../NetLayer/

MODULE_HEADERS+=$(FACILAYER_DIR)DataSet.h
MODULE_SOURCES+=$(FACILAYER_DIR)DataSet.c

MODULE_HEADERS+=$(FACILAYER_DIR)BsmTx.h
MODULE_SOURCES+=$(FACILAYER_DIR)BsmTx.c

MODULE_HEADERS+=$(FACILAYER_DIR)SpatTx.h
MODULE_SOURCES+=$(FACILAYER_DIR)SpatTx.c

MODULE_HEADERS+=$(FACILAYER_DIR)RsiTx.h
MODULE_SOURCES+=$(FACILAYER_DIR)RsiTx.c

MODULE_HEADERS+=$(FACILAYER_DIR)RsmTx.h
MODULE_SOURCES+=$(FACILAYER_DIR)RsmTx.c

MODULE_HEADERS+=$(FACILAYER_DIR)MapTx.h
MODULE_SOURCES+=$(FACILAYER_DIR)MapTx.c

MODULE_HEADERS+=$(FACILAYER_DIR)ExtFunction.h
MODULE_SOURCES+=$(FACILAYER_DIR)ExtFunction.c

MODULE_HEADERS+=$(FACILAYER_DIR)FacCommunicate.h
MODULE_SOURCES+=$(FACILAYER_DIR)FacCommunicate.c

MODULE_HEADERS+=$(FACILAYER_DIR)FacHostProcess.h
MODULE_SOURCES+=$(FACILAYER_DIR)FacHostProcess.c

MODULE_HEADERS+=$(FACILAYER_DIR)FacMsgDup.h
MODULE_SOURCES+=$(FACILAYER_DIR)FacMsgDup.c

#MODULE_HEADERS+=$(FACILAYER_DIR)DSMP.h
#MODULE_SOURCES+=$(FACILAYER_DIR)DSMP.c
#
#MODULE_HEADERS+=$(FACILAYER_DIR)DsaTx.h
#MODULE_SOURCES+=$(FACILAYER_DIR)DsaTx.c

################################################################################
TARGET = libfacilayer.so

EXTRA_CFLAGS += -I $(BASEINTERFACE_DIR)
EXTRA_CFLAGS += -I $(MSGFRAME_DIR) 
EXTRA_CFLAGS += -I $(NETLAYER_DIR) 

LDFLAGS += -L$(BASEINTERFACE_DIR)BaseFunct -lbasefunct \
	   -L$(BASEINTERFACE_DIR)CANUtil -lcanmsg \
	   -L$(BASEINTERFACE_DIR)MsgQueue -lMsgQueue \
	   -L$(BASEINTERFACE_DIR)ConfDB -lconfdb \
	   -L$(BASEINTERFACE_DIR)ConfUtil -lconfutil \
	   -L$(BASEINTERFACE_DIR)GPSUtil -lgpsutil \
	   -L$(BASEINTERFACE_DIR)NetUtil -lnetutil \
	   -L$(BASEINTERFACE_DIR)SerialUtil -lserialutil \
	   -L$(BASEINTERFACE_DIR)V2XUtil -lv2xutil \
	   -L$(NETLAYER_DIR) -lnetlayer \
	   -L$(MSGFRAME_DIR) -lmsgframe

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
