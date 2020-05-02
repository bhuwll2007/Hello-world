
# Compile for arm architecture
ARCH ?= arm
export ARCH

# Set the CROSS_COMPILE
CROSS_COMPILE ?= arm-linux-gnueabihf-
export CROSS_COMPILE 


export ARCH

# Set the CROSS_COMPILE
export CROSS_COMPILE 


LIB_DIR = $(CURDIR)/

MODULE_HEADERS+=$(LIB_DIR)LTE_V_Oprts.h
MODULE_SOURCES+=$(LIB_DIR)LTE_V_Oprts.c

################################################################################
TARGET = libltevutil.so

CFLAGS += -Wall


EXTRA_CFLAGS += -I$(LIB_DIR)

OBJS=${MODULE_SOURCES:.c=.o}

################################################################################
# Build Rules
################################################################################
CC = $(CROSS_COMPILE)gcc

all: $(TARGET)

$(TARGET): ${OBJS}
	$(CC) -shared $(CFLAGS) -o $(TARGET) ${OBJS}

.SUFFIXES:
.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ $(EXTRA_CFLAGS)

clean:
	rm -f $(TARGET)
	#rm -f $(OBJS)
	find ./ -name "*.o" |xargs rm -rf
	find ./ -name "*.d" |xargs rm -rf

