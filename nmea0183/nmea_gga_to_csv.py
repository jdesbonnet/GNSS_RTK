import sys
import csv
import pynmea2

def parse_gpgga(sentence):
    try:
        msg = pynmea2.parse(sentence)
        if isinstance(msg, pynmea2.GGA):
            t = msg.timestamp
            time = t.strftime("%H:%M:%S")
            time_sec = t.hour*3600 + t.minute*60 + t.second + t.microsecond / 1_000_000
            latitude = msg.latitude
            longitude = msg.longitude
            fix_type = msg.gps_qual
            altitude = msg.altitude
            accuracy = msg.horizontal_dil
            return time, time_sec, latitude, longitude, altitude, accuracy, fix_type
    except pynmea2.ParseError:
        return None

def main(input_file, output_file):
    with open(input_file, 'r') as infile, open(output_file, 'w', newline='') as outfile:
        writer = csv.writer(outfile)
        writer.writerow(["Time", "Latitude", "Longitude", "Altitude", "Accuracy", "Fix Type"])
        
        for line in infile:
            data = parse_gpgga(line.strip())
            if data:
                writer.writerow(data)

if __name__ == "__main__":


    input_file = sys.argv[1]
    output_file = sys.argv[2]
    
    main(input_file, output_file)

