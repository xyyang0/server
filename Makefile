server:Server.cpp ServerAddr.cpp ServerAddr.hpp Epoll.hpp Epoll.cpp Util.cpp Util.hpp
	g++ Server.cpp ServerAddr.cpp Epoll.cpp Util.cpp -o server
client:client.cpp Util.cpp Util.hpp
	g++ client.cpp Util.cpp -o client
build:server client
clean:
	rm client server
