CFLAGS+=`pkg-config --cflags opencv4`
LDFLAGS+=`pkg-config --libs opencv4`


client: client.cpp
	g++ $(CFLAGS) client.cpp -o client $(LDFLAGS)

server: server.cpp
	g++ $(CFLAGS) server.cpp -o server $(LDFLAGS)