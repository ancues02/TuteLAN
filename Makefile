#OBJS specifies which files to compile as part of the project
DEPS = Resources.h SDL_TexturesManager.h SDLGame.h InputHandler.h Vector2.h Texture.h TL_Socket.h TuteSerializable.h TuteLAN_Client.h TuteLAN_Server.h

OBJ = Resources.o SDL_TexturesManager.o SDLGame.o InputHandler.o Vector2.o Texture.o TL_Socket.o TuteSerializable.o TuteLAN_Client.o TuteLAN_Server.o

#CC specifies which compiler we're using
CC = g++ -std=c++11

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lpthread

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
