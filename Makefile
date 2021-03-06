#OBJS specifies which files to compile as part of the project
DEPS = Card.h  Resources.h Font.h SDL_TexturesManager.h SDL_FontsManager.h SDLGame.h InputHandler.h Texture.h TL_Socket.h TuteSerializable.h TuteLAN_Client.h TuteLAN_Server.h

OBJ = Card.o  Resources.o Font.o SDL_TexturesManager.o SDL_FontsManager.o  SDLGame.o InputHandler.o Texture.o TL_Socket.o TuteSerializable.o TuteLAN_Client.o TuteLAN_Server.o

#CC specifies which compiler we're using
CC = g++ -std=c++11

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf  -lpthread

#This is the target that compiles our executable
%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(COMPILER_FLAGS)

all: ts tc
	
ts: $(OBJ) TL_Server.o
	$(CC) -o $@ $^ $(COMPILER_FLAGS) $(LINKER_FLAGS)

tc: $(OBJ) TL_Client.o
	$(CC) -o  $@ $^ $(COMPILER_FLAGS) $(LINKER_FLAGS)

.PHONY: clean

clean:
	rm -f *.o ts tc
