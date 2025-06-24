import time
import serial
import argparse

def send_command (ser, cmd) :
    print (f"CMD: {cmd}")
    ser.write(f"{cmd}\r\n".encode());

def read_rtcm_message(ser):
    while True:
        # Read until we find the RTCM preamble byte
        byte = ser.read(1)
        if not byte:
            return None
        if byte[0] != 0xD3:
            continue

        # Read the next two bytes for the 10-bit length
        length_bytes = ser.read(2)
        if len(length_bytes) < 2:
            continue

        length = ((length_bytes[0] & 0x03) << 8) | length_bytes[1]

        # Read the payload and CRC (length + 3 bytes)
        payload_crc = ser.read(length + 3)
        if len(payload_crc) < length + 3:
            continue

        # Return full message: preamble + length + payload + CRC
        return b'\xD3' + length_bytes + payload_crc

def print_rtcm_as_hex(msg):
    hex_str = ' '.join(f'{byte:02X}' for byte in msg)
    print(hex_str)





if __name__ == '__main__':

    parser = argparse.ArgumentParser(description="Quectel GNSS")
    parser.add_argument("--device", help="Serial port device", required=True)
    parser.add_argument("--baudrate", help="Serial port speed (bps/baud rate)", required=True)

    args = parser.parse_args()

    baudrate = int(args.baudrate)

    ser = serial.Serial(port=args.device, baudrate=baudrate, timeout=1)


    send_command(ser,"version")
    #send_command(ser,"freset")
    send_command(ser,"CONFIG PVTALG MULTI")

    comport = "COM1"

    messages_10Hz = ["RTCM1006","RTCM1033"]

    for message in messages_10Hz :
        cfgline = f"{message} {comport} 10"
        print (f"sending {cfgline}")
        send_command(ser,cfgline)

    messages_1Hz = ["RTCM1074", "RTCM1084","RTCM1094","RTCM1114","RTCM1124"]
    for message in messages_1Hz :
        cfgline = f"{message} {comport} 1"
        print (f"sending {cfgline}")
        send_command(ser,cfgline)



    send_command(ser,"MODE BASE TIME 60")

    while True:
         if ser.in_waiting > 0:
              #data = ser.readline().decode('utf-8').strip()
              #data = ser.readline().decode('hex_codec')
              print(ser.read().hex())

              # Optional: Slow down the read loop if necessary
              #time.sleep(0.1)

              msg = read_rtcm_message(ser)
              if msg:
                   print_rtcm_as_hex(msg)
