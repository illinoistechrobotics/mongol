TARGET		= SimpleTest 
OBJS		= SimpleTest.o
SRCS		= $(OBJS:.o=.c)
INCLUDEDIRS	+= -I/home/allen/Documents/IIT/Robotics/Mongol/Dynamixel\ SDK/DXL_SDK_LINUX_v1_01/include
LIBDIRS		+= -L/home/allen/Documents/IIT/Robotics/Mongol/Dynamixel\ SDK/DXL_SDK_LINUX_v1_01/lib
CFLAGS		= $(INCLUDEDIRS) -W -Wall -O2
LDFLAGS		= $(LIBDIRS) -lm -ldxl

CC			= gcc

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	
.c.o:
	$(CC) -c -g $< $(CFLAGS)
	
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "file deleted."

dep:
	gccmakedep $(SRCS)

