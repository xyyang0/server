BUILD = ./build/
SRC = ./src/
HEADER = ./headers/
CPPS = $(wildcard ./src/*.cpp)
HEADERS = $(wildcard ./headers/*.hpp)

$(BUILD)server: Server.cpp $(CPPS) $(HEADERS)
	g++ Server.cpp $(CPPS) -o $(BUILD)server

$(BUILD)client:Client.cpp\
			   $(SRC)Address.cpp\
			   $(HEADER)Address.hpp \
			   $(SRC)TcpSocket.cpp \
			   $(HEADER)TcpSocket.hpp
	g++ Client.cpp $(SRC)Address.cpp $(SRC)TcpSocket.cpp -o $(BUILD)client
build:$(BUILD)server $(BUILD)client
clean:
	rm $(BUILD)client $(BUILD)server
