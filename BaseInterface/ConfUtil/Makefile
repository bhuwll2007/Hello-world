CUR_DIR = ./

MODULE_HEADERS += $(CUR_DIR)Config.h
MODULE_SOURCES += $(CUR_DIR)Config.c
MODULE_HEADERS += $(CUR_DIR)file.h
MODULE_SOURCES += $(CUR_DIR)file.c

################################################################################
TARGET := libconfutil.so

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
