BUILD = ./build/
CPPS = $(wildcard ./src/*.cpp)
HEADERS = $(wildcard ./headers/*.hpp)

$(BUILD)server: Server.cpp $(CPPS) $(HEADERS)
	g++ -std=c++20 Server.cpp $(CPPS) -o $(BUILD)server
build:$(BUILD)server

clean:
	rm $(BUILD)server
