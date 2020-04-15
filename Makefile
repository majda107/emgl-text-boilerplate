# =========== SOME COMMANDS I USED TO BUILD THINGS ===========

# emcc main.cpp -std=c++11 -s WASM=1 -s USE_SDL=2 -s USE_WEBGL2=1 -O3 -o index.js -I"C:\MinGW\lib\gcc\mingw32\6.3.0\include\c++\glm"

# g++ -Wall source\main.cpp -L"C:\libsdl\lib" -L"C:\Users\majda\Downloads\glew-2.1.0-win32\glew-2.1.0\lib\Release\Win32" -lmingw32 -lSDL2main -lSDL2 -lglew32 -lopengl32 -lglu32 -I"C:\\MinGW\\lib\\gcc\\mingw32\\6.3.0\\include\\c++\\glm" -I"C:\Users\majda\Downloads\glew-2.1.0-win32\glew-2.1.0\include" -I"C:\libsdl\include\SDL2" -Dmain=SDL_main
# g++ -Wall source\main.cpp -L"C:\libsdl\lib" -L"C:\Users\majda\Downloads\glew-2.1.0-win32\glew-2.1.0\lib\Release\Win32" -L"C:\Users\majda\Downloads\freetype-2.9\freetype-2.9\objs\.libs" -lfreetype -lmingw32 -lSDL2main -lSDL2 -lglew32 -lopengl32 -lglu32 -I"C:\\MinGW\\lib\\gcc\\mingw32\\6.3.0\\include\\c++\\glm" -I"C:\Users\majda\Downloads\glew-2.1.0-win32\glew-2.1.0\include" -I"C:\libsdl\include\SDL2" -I"C:\Users\majda\Downloads\freetype-2.9\freetype-2.9\include" -Dmain=SDL_main





# =========== PUT glew32.dll into 'dll' folder !!! ===========

EMXX := emcc
EMXXFLAGS := -std=c++11 -s WASM=1 -s USE_SDL=2 -s USE_WEBGL2=1 -O3 -s USE_FREETYPE=1
EMXX_INCLUDE_PATHS := -I"C:\MinGW\lib\gcc\mingw32\6.3.0\include\c++\glm" 

CXX := g++
CXXFLAGS := -Wall
CXXLIBS := -lfreetype -lmingw32 -lSDL2main -lSDL2 -lglew32 -lopengl32 -lglu32
CXX_LIB_PATHS := -L"C:\libsdl\lib" -L"C:\libglew-2.1.0\lib\Release\Win32" -L"C:\libfreetype-2.9\objs\.libs" 
CXX_INCLUDE_PATHS := $(EMXX_INCLUDE_PATHS) -I"C:\libsdl\include\SDL2" -I"C:\libglew-2.1.0\include" -I"C:\libfreetype-2.9\include"

DDL := dll

PRELOAD := --preload-file fonts/arial.ttf

SOURCE := source

BIN := bin
WEB := web
WEB_BIN := web_bin

# source files
SRC := $(wildcard $(SOURCE)/*.cpp)
OBJ := $(patsubst $(SOURCE)%.cpp, $(BIN)%.o, $(SRC))
WEB_OBJ := $(patsubst $(SOURCE)%.cpp, $(WEB_BIN)%.o, $(SRC))

all:
	make main
	make web

# DESKTOP PART
main: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(CXX_LIB_PATHS) $(CXXLIBS) $(CXX_INCLUDE_PATHS) -Dmain=SDL_main -o $(BIN)/main
	@cp dll/* $(BIN)/


$(BIN)/%.o: $(SOURCE)/%.cpp
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -c $< $(CXX_LIB_PATHS) $(CXXLIBS) $(CXX_INCLUDE_PATHS) -Dmain=SDL_main -o $@

# EMSCRIPTEN PART
web: $(WEB_OBJ)
	$(EMXX) $(EMXXFLAGS) $(WEB_OBJ) -o $(WEB)/index.js $(EMXX_INCLUDE_PATHS) $(PRELOAD) -D __EMS__

$(WEB_BIN)/%.o: $(SOURCE)/%.cpp
	@mkdir -p $(WEB_BIN)
	$(EMXX) $(EMXXFLAGS) -c $< -o $@ $(EMXX_INCLUDE_PATHS) $(PRELOAD) -D __EMS__

run:
	$(BIN)/main.exe

.PHONY: clean
clean:
	@rm -fr $(BIN)
	@rm -fr $(WEB_BIN)
	@rm -fr $(WEB)/*.wasm $(WEB)/*.js $(WEB)/*.data