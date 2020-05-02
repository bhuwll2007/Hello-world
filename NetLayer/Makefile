CUR_DIR = ./

# NetLayer dir
NETLAYER_DIR := $(CUR_DIR)

# BaseInterface dir
BASEINTERFACE_DIR := $(CUR_DIR)../BaseInterface/

MODULE_HEADERS+=$(NETLAYER_DIR)DataSet.h
MODULE_SOURCES+=$(NETLAYER_DIR)DataSet.c

MODULE_HEADERS+=$(NETLAYER_DIR)DSMP.h
MODULE_SOURCES+=$(NETLAYER_DIR)DSMP.c

MODULE_HEADERS+=$(NETLAYER_DIR)MIBUtil.h
MODULE_SOURCES+=$(NETLAYER_DIR)MIBUtil.c

MODULE_HEADERS+=$(NETLAYER_DIR)FunCommun.h
MODULE_SOURCES+=$(NETLAYER_DIR)FunCommun.c

################################################################################
TARGET = libnetlayer.so

EXTRA_CFLAGS += -I $(BASEINTERFACE_DIR)

LDFLAGS += -L$(BASEINTERFACE_DIR)BaseFunct -lbasefunct \
	   -L$(BASEINTERFACE_DIR)ConfUtil -lconfutil \
	   -L$(BASEINTERFACE_DIR)V2XUtil -lv2xutil 

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
