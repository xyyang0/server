BUILD=./build/
SRC=./src/
HEADER=./headers/
$(BUILD)server:$(SRC)Server.cpp \
			   $(SRC)TcpSocket.cpp \
			   $(HEADER)TcpSocket.hpp \
			   $(HEADER)Epoll.hpp \
			   $(SRC)Epoll.cpp \
			   $(SRC)Util.cpp \
			   $(HEADER)Util.hpp \
			   $(SRC)Address.cpp\
			   $(HEADER)Address.hpp
	g++ $(SRC)Server.cpp $(SRC)TcpSocket.cpp $(SRC)Address.cpp \
	    $(SRC)Epoll.cpp $(SRC)Util.cpp -o $(BUILD)server
$(BUILD)client:$(SRC)client.cpp \
			   $(SRC)Address.cpp\
			   $(HEADER)Address.hpp \
			   $(SRC)TcpSocket.cpp \
			   $(HEADER)TcpSocket.hpp
	g++ $(SRC)client.cpp $(SRC)Address.cpp $(SRC)TcpSocket.cpp -o $(BUILD)client
build:$(BUILD)server $(BUILD)client
clean:
	rm $(BUILD)client $(BUILD)server
