CC=gcc
CFLAGS=-g -MD
OBJECTS=bhdr.o
DFILES=$(patsubst %.o,%.d,$(OBJECTS))

all: bh
.PHONY: all clean


bh: $(OBJECTS)
	$(CC) -Wall -o $@ $<
	cp bh ../../bin/. 

clean:
	rm -f $(OBJECTS) $(DFILES) bh
	
.c.o:
	gcc  -g -MD -c $<

-include $(DFILES)




