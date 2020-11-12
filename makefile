CC = g++
CFLAGS = -fPIC -c -I$(VSTSDK) -D__cdecl=""
LFLAGS = -shared -Wl,-soname,$(TARGET) -o $(TARGET)
VSTSDK = VST3_SDK
SRCS = src/C1Bitcrusher.cpp src/main.cpp src/mt19937ar.cpp $(VSTSDK)/public.sdk/source/vst2.x/audioeffect.cpp $(VSTSDK)/public.sdk/source/vst2.x/audioeffectx.cpp $(VSTSDK)/public.sdk/source/vst2.x/vstplugmain.cpp
OBJS = audioeffect.o audioeffectx.o C1Bitcrusher.o main.o mt19937ar.o vstplugmain.o
TARGET = C1Bitcrusher.so
INSTDIR = /usr/lib/vst

all: compile link

compile: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS)

link: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS)

install: $(TARGET)
	mkdir $(INSTDIR)
	cp $(TARGET) $(INSTDIR)

uninstall: $(INSTDIR)/$(TARGET)
	rm $(INSTDIR)/$(TARGET)

compress: $(TARGET)
	gzip $(TARGET)

decompress: $(TARGET).gz
	gzip -d $(TARGET).gz

.PHONY: clean help

clean:
	rm -f $(OBJS) $(TARGET)

help:
	@echo supported commands:
	@echo make: compile and link the library
	@echo make compile: compile source files into object code
	@echo make link: link object code into shared library
	@echo make install: install the library onto the system
	@echo make uninstall: remove the library from the system
	@echo make compress: compress the library into a gzip archive
	@echo make decompress: decompress the library from a gzip archive
	@echo make clean: remove object code and compiled library from working directory
	@echo make help: list supported commands
