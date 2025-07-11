#!/usr/bin/env python3
 
# Script to configure UM980 into base station mode and then optionally
# relay RTCM messages to a NTRIP caster.
#
 


import time
import socket
import base64
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

def send_chunked(sock, data):
    #chunk = f"{len(data):X}\r\n".encode('ascii') + data + b"\r\n"
    #sock.sendall(chunk)
    sock.sendall(data)


def um980_init_basestation (ser, args, comport="COM1",survey_in_time=60) :
    """
    Configure UM980 to be in base station mode
    """

    send_command(ser,"CONFIG PVTALG MULTI")

    messages_10Hz = ["RTCM1006","RTCM1033"]

    for message in messages_10Hz :
        cfgline = f"{message} {comport} 10"
        send_command(ser,cfgline)

    messages_1Hz = ["RTCM1074", "RTCM1084","RTCM1094","RTCM1114","RTCM1124"]
    for message in messages_1Hz :
        cfgline = f"{message} {comport} 1"
        send_command(ser,cfgline)


    #send_command(ser,f"MODE BASE TIME {survey_in_time}")
    send_command(ser,f"MODE BASE {args.antenna_lat} {args.antenna_lng} {args.antenna_alt}")


def connect_to_ntrip_caster(host, port, mountpoint, user, passwd):
    """
    Open a TCP socket to NTRIP caster with NTRIP header indicating a RTCM 
    correction feed is to follow.
    """

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))

    print (f"socket open: {sock}")

    # Base64 encode user credentials
    credentials = base64.b64encode(f"{user}:{passwd}".encode('utf-8')).decode('ascii')

    # Send the NTRIP headers
    headers = (
        f"SOURCE /{mountpoint} HTTP/1.1\r\n"
        f"Host: {host}\r\n"
        f"Ntrip-Version: Ntrip/2.0\r\n"
        f"User-Agent: NTRIP client\r\n"
        f"Authorization: Basic {credentials}\r\n"
        f"Content-Type: gnss/data\r\n"
        f"Connection: close\r\n"
        f"Transfer-Encoding: chunked\r\n"
        f"\r\n"
    )

    print (f"sending headers: {headers}")
    sock.sendall(headers.encode('ascii'))

    print ("waiting for response from caster")

    # Read the response
    #response = sock.recv(4096)
    #if b"ICY 200 OK" not in response and b"HTTP/1.1 200 OK" not in response:
    #    raise ConnectionError("Failed to connect to NTRIP caster:\n" + response.decode())

    print("Connected to NTRIP caster.")
    return sock


def main (args) :

    ser = serial.Serial(port=args.device, baudrate=args.baudrate, timeout=1)

    um980_init_basestation (ser,args)


    if args.caster_host and args.caster_port :
        try:
            ntrip_sock = connect_to_ntrip_caster(args.caster_host, args.caster_port, args.caster_mountpoint, args.caster_username, args.caster_password)
        except Exception as e:
            print(f"Error opening socket to NTRIP caster: {e}")
            return 
        print (f"caster socket open: {ntrip_sock}")


    while True:
        if ser.in_waiting > 0:
            #print(ser.read().hex())
            msg = read_rtcm_message(ser)
            if msg:
                #print_rtcm_as_hex(msg)
                send_chunked(ntrip_sock,msg)

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description="Quectel GNSS")
    parser.add_argument("--device", help="GNSS serial port device", required=True)
    parser.add_argument("--baudrate", type=int, help="Serial port speed (bps/baud rate)", required=True)
    parser.add_argument("--caster-host",help="NTRIP caster host")
    parser.add_argument("--caster-port",type=int, help="NTRIP caster port", default=2101)
    parser.add_argument("--caster-username",help="NTRIP caster username (optional)", default="")
    parser.add_argument("--caster-password",help="NTRIP caster password (optional)", default="")
    parser.add_argument("--caster-mountpoint",help="NTRIP caster mountpoint")
    parser.add_argument("--antenna-lat",type=float,help="Antenna latitude")
    parser.add_argument("--antenna-lng",type=float,help="Antenna longitude")
    parser.add_argument("--antenna-alt",type=float,help="Antenna altitude m")


    args = parser.parse_args()
    main (args)

