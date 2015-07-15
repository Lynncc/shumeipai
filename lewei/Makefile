CC = gcc

XX = g++
# CFLAGS = -Wall -O –g
LDFLAGS = -lwiringPi

TARGET = lewei

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

%.o:%.cpp
	$(XX) $(CFLAGS) $(LDFLAGS) -c $< -o $@

SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))

$(TARGET) : $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	chmod a+x $(TARGET)
	rm -rf *.o

clean:
	rm -rf *.o $(TARGET)
