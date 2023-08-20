BUILD = ./build/
CPPS = $(wildcard ./src/*.cpp)
HEADERS = $(wildcard ./headers/*.hpp)

$(BUILD)server: main.cpp $(CPPS) $(HEADERS)
	g++ -std=c++20 main.cpp $(CPPS) -o $(BUILD)server
build:$(BUILD)server

clean:
	rm $(BUILD)server
