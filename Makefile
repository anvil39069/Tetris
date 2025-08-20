# Compiler
CXX = g++

# Compiler flags with correct include path
CXXFLAGS = -std=c++17 -O2 -IC:/raylib/include

# Windows libraries with correct library path
LIBS = -LC:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm

# Source files
SRCS = src/main.cpp src/game.cpp src/grid.cpp src/block.cpp src/position.cpp src/blocks.cpp src/colors.cpp src/settings.cpp

# Output file
TARGET = tetris.exe

# Build rule
all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

# Clean rule
clean:
	del /f /q $(TARGET) 2>nul

# Run the game
run: all
	$(TARGET)

# Static build (no external DLL needed)
static: LIBS = -LC:/raylib/lib -lraylib -static -lopengl32 -lgdi32 -lwinmm -lstdc++ -lm
static: all

.PHONY: all clean run static