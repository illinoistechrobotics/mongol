TARGET		= StandSit
OBJS		= StandSit.o
SRCS		= $(OBJS:.o=.c)
INCLUDEDIRS	+= -I../Dynamixel\ SDK/DXL_SDK_LINUX_v1_01/include/
LIBDIRS		+= -L../Dynamixel\ SDK/DXL_SDK_LINUX_v1_01/lib/
CFLAGS		= $(INCLUDEDIRS) -W -Wall -O2
LDFLAGS		= $(LIBDIRS) -lm -ldxl

CC			= gcc

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	
.c.o:
	$(CC) -c $< $(CFLAGS)
	
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "file deleted."

dep:
	gccmakedep $(SRCS)

