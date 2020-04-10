# emcc main.cpp -std=c++11 -s WASM=1 -s USE_SDL=2 -s USE_WEBGL2=1 -O3 -o index.js -I"C:\MinGW\lib\gcc\mingw32\6.3.0\include\c++\glm"

EMXX := emcc
EMXXFLAGS := -std=c++11 -s WASM=1 -s USE_SDL=2 -s USE_WEBGL2=1 -O3

INCLUDE_PATHS := -I"C:\MinGW\lib\gcc\mingw32\6.3.0\include\c++\glm"

SOURCE := source
BIN := bin

# source files
SRC := $(wildcard $(SOURCE)/*.cpp)

all:
	@mkdir -p $(BIN)
	make main

main: $(SRC)
	$(EMXX) $(EMXXFLAGS) $(SRC) -o $(BIN)/index.js $(INCLUDE_PATHS)

clean:
	@rm -fr $(BIN)