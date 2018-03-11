FILE=skeleton
S_DIR=Source
B_DIR=Build

########
# Output
EXEC=$(B_DIR)/$(FILE)

#default build settings
CC_OPTS=-c -pipe -Wall -Wno-switch -ggdb -g3 -O3
LN_OPTS=
CC=g++

########
#SDL options
SDL_CFLAGS := $(shell sdl2-config --cflags)
GLM_CFLAGS := -I../glm/
SDL_LDFLAGS := $(shell sdl2-config --libs)

########
#This is the default action
all:Build


#########
#Object list
OBJ = $(B_DIR)/$(FILE).o $(B_DIR)/TestModelH.o $(B_DIR)/Intersection.o $(B_DIR)/Light.o $(B_DIR)/Object.o 

########
#Objects
$(B_DIR)/$(FILE).o : $(S_DIR)/$(FILE).cpp $(S_DIR)/SDLauxiliary.h 
	$(CC) $(CC_OPTS) -o $(B_DIR)/$(FILE).o $(S_DIR)/$(FILE).cpp $(SDL_CFLAGS) $(GLM_CFLAGS)

$(B_DIR)/Intersection.o : $(S_DIR)/Intersection.cpp $(S_DIR)/Intersection.h 
	$(CC) $(CC_OPTS) -o $(B_DIR)/Intersection.o $(S_DIR)/Intersection.cpp $(SDL_CFLAGS) $(GLM_CFLAGS)

$(B_DIR)/TestModelH.o : $(S_DIR)/TestModelH.cpp $(S_DIR)/TestModelH.h 
	$(CC) $(CC_OPTS) -o $(B_DIR)/TestModelH.o $(S_DIR)/TestModelH.cpp $(SDL_CFLAGS) $(GLM_CFLAGS)

$(B_DIR)/Light.o : $(S_DIR)/Light.cpp $(S_DIR)/Light.h 
	$(CC) $(CC_OPTS) -o $(B_DIR)/Light.o $(S_DIR)/Light.cpp $(SDL_CFLAGS) $(GLM_CFLAGS)

$(B_DIR)/Object.o : $(S_DIR)/Object.cpp $(S_DIR)/Object.h 
	$(CC) $(CC_OPTS) -o $(B_DIR)/Object.o $(S_DIR)/Object.cpp $(SDL_CFLAGS) $(GLM_CFLAGS)


########
#Main build rule     
Build : $(OBJ) Makefile
	$(CC) $(LN_OPTS) -o $(EXEC) $(OBJ) $(SDL_LDFLAGS)
#
#
clean:
	rm -f $(B_DIR)/*

