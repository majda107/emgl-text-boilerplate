# emcc main.cpp -std=c++11 -s WASM=1 -s USE_SDL=2 -s USE_WEBGL2=1 -O3 -o index.js -I"C:\MinGW\lib\gcc\mingw32\6.3.0\include\c++\glm"

# g++ -Wall source\main.cpp -L"C:\libsdl\lib" -L"C:\Users\majda\Downloads\glew-2.1.0-win32\glew-2.1.0\lib\Release\Win32" -lmingw32 -lSDL2main -lSDL2 -lglew32 -lopengl32 -lglu32 -I"C:\\MinGW\\lib\\gcc\\mingw32\\6.3.0\\include\\c++\\glm" -I"C:\Users\majda\Downloads\glew-2.1.0-win32\glew-2.1.0\include" -I"C:\libsdl\include\SDL2" -Dmain=SDL_main
# g++ -Wall source\main.cpp -L"C:\libsdl\lib" -L"C:\Users\majda\Downloads\glew-2.1.0-win32\glew-2.1.0\lib\Release\Win32" -L"C:\Users\majda\Downloads\freetype-2.9\freetype-2.9\objs\.libs" -lfreetype -lmingw32 -lSDL2main -lSDL2 -lglew32 -lopengl32 -lglu32 -I"C:\\MinGW\\lib\\gcc\\mingw32\\6.3.0\\include\\c++\\glm" -I"C:\Users\majda\Downloads\glew-2.1.0-win32\glew-2.1.0\include" -I"C:\libsdl\include\SDL2" -I"C:\Users\majda\Downloads\freetype-2.9\freetype-2.9\include" -Dmain=SDL_main

EMXX := emcc
EMXXFLAGS := -std=c++11 -s WASM=1 -s USE_SDL=2 -s USE_WEBGL2=1 -O3 -s USE_FREETYPE=1

PRELOAD := --preload-file fonts/arial.ttf

INCLUDE_PATHS := -I"C:\MinGW\lib\gcc\mingw32\6.3.0\include\c++\glm" -I"C:\Users\majda\Downloads\freetype-2.9\freetype-2.9\include"

SOURCE := source
BIN := bin

# source files
SRC := $(wildcard $(SOURCE)/*.cpp)

all:
	@mkdir -p $(BIN)
	make main

main: $(SRC)
	$(EMXX) $(EMXXFLAGS) $(SRC) -o $(BIN)/index.js $(INCLUDE_PATHS) $(PRELOAD) 

clean:
	@rm -fr $(BIN)