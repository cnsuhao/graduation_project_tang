all : client 
client : client.o
	gcc client.o -o client -lstdc++
client.o : client.cpp
	cc -c client.cpp
.PHONY : clean
clean : 
	-rm client.o client
