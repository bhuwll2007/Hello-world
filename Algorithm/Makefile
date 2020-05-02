CUR_DIR = ./

# Algorithm dir
ALGORITHM_DIR := $(CUR_DIR)

# MsgFrame dir
MSGFRAME_DIR := $(CUR_DIR)../MsgFrame/

MODULE_HEADERS += $(ALGORITHM_DIR)DataSet/DataSet.h
MODULE_SOURCES += $(ALGORITHM_DIR)DataSet/DataSet.c

MODULE_HEADERS += $(ALGORITHM_DIR)Appconfigure.h

MODULE_SOURCES += $(ALGORITHM_DIR)BSM_Algorithm.c
MODULE_HEADERS += $(ALGORITHM_DIR)BSM_Algorithm.h

MODULE_SOURCES += $(ALGORITHM_DIR)cJSON.c
MODULE_HEADERS += $(ALGORITHM_DIR)cJSON.h

MODULE_SOURCES += $(ALGORITHM_DIR)ComputeTool.c
MODULE_HEADERS += $(ALGORITHM_DIR)ComputeTool.h

MODULE_SOURCES += $(ALGORITHM_DIR)MapMatch.c
MODULE_HEADERS += $(ALGORITHM_DIR)MapMatch.h

MODULE_SOURCES += $(ALGORITHM_DIR)GetMapPoints.c
MODULE_HEADERS += $(ALGORITHM_DIR)GetMapPoints.h

MODULE_SOURCES += $(ALGORITHM_DIR)RSI_Algorithm.c
MODULE_HEADERS += $(ALGORITHM_DIR)RSI_Algorithm.h

MODULE_SOURCES += $(ALGORITHM_DIR)SPAT_Algorithm.c
MODULE_HEADERS += $(ALGORITHM_DIR)SPAT_Algorithm.h

MODULE_SOURCES += $(ALGORITHM_DIR)MercatorProj.c
MODULE_HEADERS += $(ALGORITHM_DIR)MercatorProj.h

MODULE_HEADERS += $(ALGORITHM_DIR)Type.h

################################################################################
TARGET := libAlgorithm.so

EXTRA_CFLAGS += -I $(MSGFRAME_DIR) 
EXTRA_CFLAGS += -I $(ALGORITHM_DIR) 

LDFLAGS :=
LDFLAGS += -L$(MSGFRAME_DIR) -lmsgframe

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
