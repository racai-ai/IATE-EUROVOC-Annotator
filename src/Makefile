SERVER_OBJS=server/strmap.o server/http_server.o server/http_mapping.o server/urldecode.o server/encoding/ascii.o  server/encoding/encoding.o server/encoding/utf8.o server/encoding/utf16.o
SERVER_MODULES=server/modules/annotate/annotate.o
UTILS_OBJS=utils/LinkedList.o
all: annotate

Writer.o: Writer.cpp Writer.h
	g++ -c -o Writer.o Writer.cpp

AhoCorasick.o: AhoCorasick.cpp AhoCorasick.h Writer.h
	g++ -c -o AhoCorasick.o AhoCorasick.cpp

FileProcesser.o: FileProcesser.cpp FileProcesser.h AhoCorasick.h Writer.h
	g++ -c -o FileProcesser.o FileProcesser.cpp

main.o: main.cpp FileProcesser.h AhoCorasick.h Writer.h
	g++ -c -o main.o main.cpp

server_objects: $(SERVER_OBJS)

server_modules: $(SERVER_MODULES)

server/modules/annotate/annotate.o: server/modules/annotate/annotate.cpp
	g++ -c -o server/modules/annotate/annotate.o server/modules/annotate/annotate.cpp

server/http_server.o: server/http_server.cpp
	g++ -c -o server/http_server.o server/http_server.cpp

server/http_mapping.o: server/http_mapping.cpp
	g++ -c -o server/http_mapping.o server/http_mapping.cpp

server/urldecode.o: server/urldecode.cpp
	g++ -c -o server/urldecode.o server/urldecode.cpp

server/strmap.o: server/strmap.c
	g++ -c -o server/strmap.o server/strmap.c

server/encoding/ascii.o: server/encoding/ascii.cpp
	g++ -c -o server/encoding/ascii.o server/encoding/ascii.cpp

server/encoding/encoding.o: server/encoding/encoding.cpp
	g++ -c -o server/encoding/encoding.o server/encoding/encoding.cpp

server/encoding/utf8.o: server/encoding/utf8.cpp
	g++ -c -o server/encoding/utf8.o server/encoding/utf8.cpp

server/encoding/utf16.o: server/encoding/utf16.cpp
	g++ -c -o server/encoding/utf16.o server/encoding/utf16.cpp

utils:$(UTILS_OBJS)

utils/LinkedList.o: utils/LinkedList.c utils/LinkedList.h
	g++ -c -o utils/LinkedList.o utils/LinkedList.c

annotate: Writer.o AhoCorasick.o FileProcesser.o main.o server_objects server_modules utils
	g++ -o annotate Writer.o AhoCorasick.o FileProcesser.o main.o $(SERVER_OBJS) $(SERVER_MODULES) $(UTILS_OBJS) -lpthread

clean:
	rm -f *.o
	rm -f server/*.o
	rm -f server/encoding/*.o
	rm -f server/modules/annotate/*.o
	rm -f annotate
