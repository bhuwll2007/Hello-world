CUR_DIR = ./

MODULE_SOURCES += $(CUR_DIR)../BaseFunct/semaphore.c
MODULE_SOURCES += $(CUR_DIR)../ConfDB/ConfDB.c
MODULE_SOURCES += $(CUR_DIR)V2X_INTERFACE.c
MODULE_SOURCES += $(CUR_DIR)LTE_V2X.c
MODULE_SOURCES += $(CUR_DIR)LTEVUtil/LTE_V2X_RAW.c
################################################################################
TARGET = libv2xutil.so

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
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ $(EXTRA_CFLAGS) ${LDFLAGS}

clean:
	rm -f $(TARGET)
	#rm -f $(OBJS)
	find ./ -name "*.o" |xargs rm -rf
	find ./ -name "*.d" |xargs rm -rf
