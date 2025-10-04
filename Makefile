.DEFAULT: run

CC_FLAGS = -Wall -Werror -Iraylib/src -I./include -I./raylib-cpp/include/

RAYLIB := libraylib.a
SRC := $(wildcard src/*.cpp)
HEADERS := $(wildcard include/*.hpp)

my_app: $(SRC) $(HEADERS)
	clang++ $(CC_FLAGS) -std=c++17 -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL $(RAYLIB) $(SRC) -o my_app

run: my_app
	./my_app

clean:
	rm my_app
