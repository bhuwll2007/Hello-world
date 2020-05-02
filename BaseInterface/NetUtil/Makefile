CUR_DIR = ./

MODULE_HEADERS += $(CUR_DIR)TcpClient.h
MODULE_SOURCES += $(CUR_DIR)TcpClient.c
MODULE_HEADERS += $(CUR_DIR)TcpServer.h
MODULE_SOURCES += $(CUR_DIR)TcpServer.c
MODULE_HEADERS += $(CUR_DIR)UdpClient.h
MODULE_SOURCES += $(CUR_DIR)UdpClient.c
MODULE_HEADERS += $(CUR_DIR)UdpService.h
MODULE_SOURCES += $(CUR_DIR)UdpService.c
MODULE_SOURCES += $(CUR_DIR)../BaseFunct/BaseFunct.c

################################################################################
TARGET := libnetutil.so

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
