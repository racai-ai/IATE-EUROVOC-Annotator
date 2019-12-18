all: annotate

Writer.o: Writer.cpp Writer.h
	g++ -c -o Writer.o Writer.cpp

AhoCorasick.o: AhoCorasick.cpp AhoCorasick.h Writer.h
	g++ -c -o AhoCorasick.o AhoCorasick.cpp

FileProcesser.o: FileProcesser.cpp FileProcesser.h AhoCorasick.h Writer.h
	g++ -c -o FileProcesser.o FileProcesser.cpp

main.o: main.cpp FileProcesser.h AhoCorasick.h Writer.h
	g++ -c -o main.o main.cpp

annotate: Writer.o AhoCorasick.o FileProcesser.o main.o
	g++ -o annotate Writer.o AhoCorasick.o FileProcesser.o main.o

clean:
	rm -f *.o
	rm -f annotate
