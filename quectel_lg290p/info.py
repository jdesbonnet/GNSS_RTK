import time
import serial
import argparse

def calculate_nmea_checksum(nmea_sentence):
    """
    Calculate the checksum for an NMEA sentence.

    Parameters:
        nmea_sentence (str): The NMEA sentence (e.g., "$GPGGA,123519,...").

    Returns:
        str: The calculated checksum as a two-character hexadecimal string.
    """
    # Remove the starting '$' if present
    if nmea_sentence.startswith('$'):
        nmea_sentence = nmea_sentence[1:]

    # Find the position of '*' to determine where the checksum ends
    checksum_end = nmea_sentence.find('*')
    if checksum_end != -1:
        nmea_sentence = nmea_sentence[:checksum_end]

    # Calculate the checksum by XORing all characters
    checksum = 0
    for char in nmea_sentence:
        checksum ^= ord(char)

    # Return the checksum as a two-character hexadecimal string
    return f"{checksum:02X}"



def send_command (ser, cmd) :

    checksum = calculate_nmea_checksum(cmd)
    full_sentence = cmd + "*" + checksum +"\r\n"
    print (f"sending {full_sentence}")

    ser.write(full_sentence.encode())


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description="Quectel GNSS")
    parser.add_argument("--device", help="Serial port device", required=True)
    parser.add_argument("--baudrate", help="Serial port speed (bps/baud rate)", default=460800)

    args = parser.parse_args()

    baudrate = int(args.baudrate)

    ser = serial.Serial(port=args.device, baudrate=baudrate, timeout=1)

    send_command(ser,"$PQTMVERNO")

    while True:
         if ser.in_waiting > 0:
              data = ser.readline().decode('utf-8').strip()
              if data.startswith("$PQTMVERNO") :
                  print(data)
                  break



