EXEC:=ConsoleCrawler

MAIN = main.o soul.o player.o misc.o console.o event.o

# general compiler settings
CPPFLAGS=
CXXFLAGS=-std=c++11 -Wall -Wextra -Werror -ggdb
LDFLAGS=

#default target is debug Linux
all: linux

win: CXX=x86_64-w64-mingw32-c++
win: LDFLAGS+=-static-libgcc -static-libstdc++ -static
win: EXEC:=$(EXEC).exe
win: comp

linux: EXEC:=$(EXEC).out
linux: comp

comp: $(MAIN)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(MAIN) -o $(EXEC) $(LDFLAGS)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(warning Cleaning...)
	@$(RM) $(MAIN)

.PHONY: all clean

