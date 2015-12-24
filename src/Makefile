all: clean server tcpclient fcgi_server

tcpserver: simple_tcpserver.c
	gcc simple_tcpserver.c -o simple_tcpserver

tcpclient: simple_tcpclient.c
	gcc simple_tcpclient.c -o simple_tcpclient

fcgi_server: fcgi_server.cpp
	g++ fcgi_server.cpp -lfcgi++ -lfcgi -o fcgi_server

clean:
	rm -fr *.exe *.so *.cgi simple_tcpclient simple_tcpserver fcgi_server
