import socket
import base64
import argparse
import serial
import sys
import select
import time

def build_ntrip_request(host, port, mountpoint, user, password):
    headers = [
        f"GET /{mountpoint} HTTP/1.1",
        f"Host: {host}:{port}",
        "User-Agent: NTRIP PythonClient/1.0",
        f"Ntrip-Version: Ntrip/2.0",
        "Connection: close"
    ]
    if user and password:
        credentials = f"{user}:{password}"
        encoded_credentials = base64.b64encode(credentials.encode()).decode()
        headers.append(f"Authorization: Basic {encoded_credentials}")
    headers.append("")
    headers.append("")
    return "\r\n".join(headers)

def connect_ntrip(host, port, request):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, port))
    s.send(request.encode())
    response = b""
    while b"\r\n\r\n" not in response:
        chunk = s.recv(1024)
        if not chunk:
            raise ConnectionError("No response from NTRIP server")
        response += chunk

    header_end = response.find(b"\r\n\r\n") + 4
    header = response[:header_end].decode(errors='ignore')
    if "200 OK" not in header:
        raise ConnectionError(f"NTRIP server error: {header.strip()}")

    return s

def relay_data(ntrip_sock, serial_port, gga_interval):
    last_gga = None
    last_gga_send = 0

    try:
        while True:
            now = time.time()
            rlist, _, _ = select.select([ntrip_sock, serial_port.fileno()], [], [], 0.5)

            # Handle incoming RTCM data from NTRIP to GNSS device
            if ntrip_sock in rlist:
                data = ntrip_sock.recv(1024)
                if not data:
                    print("NTRIP stream ended.")
                    break
                serial_port.write(data)

            # Handle incoming NMEA data from GNSS device
            if serial_port.fileno() in rlist:
                try:
                    line = serial_port.readline()
                    if not line:
                        continue
                    if line.startswith(b'$'):
                        decoded = line.decode(errors='ignore').strip()
                        print(decoded)
                        if decoded.startswith("$GPGGA") or decoded.startswith("$GNGGA"):
                            last_gga = decoded + "\r\n"
                except Exception as e:
                    print(f"Serial read error: {e}")

            # Send latest GGA to NTRIP server at configured interval
            if gga_interval > 0 and last_gga:
                if now - last_gga_send >= gga_interval:
                    try:
                        ntrip_sock.send(last_gga.encode())
                        print(f"Sent GGA to NTRIP: {last_gga.strip()}")
                        last_gga_send = now
                    except Exception as e:
                        print(f"Failed to send GGA to NTRIP server: {e}")
                        break

    except KeyboardInterrupt:
        print("Interrupted by user.")
    finally:
        serial_port.close()
        ntrip_sock.close()

def main():
    parser = argparse.ArgumentParser(description="NTRIP to Serial RTCM relay + NMEA reader + GGA forwarder")
    parser.add_argument("--device", required=True, help="Serial device (e.g. /dev/ttyUSB0)")
    parser.add_argument("--baudrate", type=int, required=True, help="Serial baudrate (e.g. 9600)")
    parser.add_argument("--host", required=True, help="NTRIP caster hostname")
    parser.add_argument("--port", type=int, default=2101, help="NTRIP caster port (default: 2101)")
    parser.add_argument("--mountpoint", required=True, help="NTRIP mountpoint")
    parser.add_argument("--user", help="NTRIP username (optional)")
    parser.add_argument("--password", help="NTRIP password (optional)")
    parser.add_argument("--gga-interval", type=int, default=0,
                        help="Interval (in seconds) to send GGA sentences to NTRIP server (0 = disabled)")
    args = parser.parse_args()

    try:
        ser = serial.Serial(args.device, args.baudrate, timeout=0.5)
        print(f"Opened serial port {args.device} at {args.baudrate} baud.")

        request = build_ntrip_request(args.host, args.port, args.mountpoint, args.user, args.password)
        print(f"Connecting to NTRIP caster {args.host}:{args.port}...")
        ntrip_sock = connect_ntrip(args.host, args.port, request)
        print(f"Connected to mountpoint '{args.mountpoint}', relaying RTCM and reading NMEA...")

        relay_data(ntrip_sock, ser, args.gga_interval)

    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
