CC=gcc
DEPS=-lrt
SOURCES= timetag.c gamepad_events.c
EXECUTABLE= timetag nmea_filter gamepad_events

#$(EXECUTABLE): $(SOURCES) $(DEPS)
#	$(CC) -o $@ $^

all: timetag nmea_filter gamepad_events

timetag: timetag.c
	$(CC) -o timetag timetag.c
	
nmea_filter: nmea_filter.c
	$(CC) -o nmea_filter nmea_filter.c

gamepad_events: gamepad_events.c
	$(CC) -o gamepad_events gamepad_events.c


.PHONY: clean

clean:
	rm -f *.o *~ $(EXECUTABLE)
