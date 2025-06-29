import sys
import time
import serial
import argparse

def send_command (ser, cmd) :
    ser.write(f"{cmd}\r\n".encode());


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description="Quectel GNSS")
    parser.add_argument("--device", help="Serial port device", required=True)
    parser.add_argument("--baudrate", help="Serial port speed (bps/baud rate)", required=True)

    args = parser.parse_args()

    baudrate = int(args.baudrate)

    ser = serial.Serial(port=args.device, baudrate=baudrate, timeout=1)


    send_command(ser,"version")
    send_command(ser,"freset")

    time.sleep(10)
    send_command(ser,"GNGGA COM1 0.2")
    send_command(ser,"GNRMC COM1 1")
    send_command(ser,"GNGSV COM1 1")

    while True:
         if ser.in_waiting > 0:
              data = ser.readline().decode('utf-8').strip()
              print(data)
              sys.stdout.flush()

              # Optional: Slow down the read loop if necessary
              #time.sleep(0.1)


