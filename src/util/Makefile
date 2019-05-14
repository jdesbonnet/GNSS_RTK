CC=gcc
DEPS=-lrt
SOURCES= timetag.c
EXECUTABLE= timetag

$(EXECUTABLE): $(SOURCES) $(DEPS)
	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -f *.o *~ $(EXECUTABLE)
