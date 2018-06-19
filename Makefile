# General
CC = g++
MAKEFLAGS += -j

#-Imavlink-interface
# Names and flags
INC_DIR = -I.
CPPSOURCES += 

COMMON_OBJECTS = $(CPPSOURCES:.cpp=.o)

SOURCES = aruco_finder.cpp
EXECUTABLES = aruco_finder 

CFLAGS = -c -Wall -g $(INC_DIR) 
OPENCV = `pkg-config opencv --cflags --libs `


LIBS = $(OPENCV)
LIBS += -Laruco/lib -laruco 
LIBS += -lboost_system -lboost_filesystem


# Targets
all: $(CPPSOURCES) $(EXECUTABLES)

aruco_finder: $(SOURCES)
	$(CC) -pthread -std=c++11 $(SOURCES) -o $@ $(LIBS) $(INC_DIR)

.c.o:
	$(CC) $(CFLAGS) -std=c++11 $(INCLUDES) $< -o $@

.cpp.o:
	$(CC) $(CFLAGS) -std=c++11 $(INCLUDES) $< -o $@

clean:
	@echo COMMON_OBJECTS = $(COMMON_OBJECTS)
	rm -rf $(COMMON_OBJECTS) $(EXECUTABLES)
