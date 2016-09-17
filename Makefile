health: health.o health_util.o
	gcc -o health health.o health_util.o
health.o:	health.c health.h
	gcc -c health.c
health_util.o:	health_util.c health.h
	gcc -c health_util.c
clean:
	rm -f *.o health
	
