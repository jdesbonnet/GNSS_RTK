CC=gcc
DEPS=-lrt
SOURCES= timetag.c
EXECUTABLE= timetag nmea_filter gamepad_events

$(EXECUTABLE): $(SOURCES) $(DEPS)
	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -f *.o *~ $(EXECUTABLE)
