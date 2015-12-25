all: clean tcpserver tcpclient fcgi_server dummy_inserts

tcpserver: simple_tcpserver.cpp db_connection.cpp
	g++ simple_tcpserver.cpp db_connection.cpp -o simple_tcpserver $(shell mysql_config --cflags --libs) -std=c++11

tcpclient: simple_tcpclient.cpp
	g++ simple_tcpclient.cpp -o simple_tcpclient

fcgi_server: fcgi_server.cpp backend_connection.h
	g++ fcgi_server.cpp backend_connection.cpp -lfcgi++ -lfcgi -o fcgi_server -std=c++11

dummy_inserts: dummy_inserts.cpp db_connection.h
	g++ dummy_inserts.cpp db_connection.cpp -o dummy_inserts $(shell mysql_config --cflags --libs)

clean:
	rm -fr *.exe *.so *.cgi simple_tcpclient simple_tcpserver fcgi_server dummy_inserts
