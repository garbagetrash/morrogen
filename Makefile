CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		mwScript.o

LIBS =

TARGET =	mwScript.exe

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
