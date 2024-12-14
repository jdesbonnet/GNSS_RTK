import serial
import struct

def calculate_ubx_checksum(packet):
    """Calculate the UBX checksum for a given packet excluding sync chars."""
    ck_a = 0
    ck_b = 0
    for byte in packet:
        ck_a = (ck_a + byte) & 0xFF
        ck_b = (ck_b + ck_a) & 0xFF
    return ck_a, ck_b

# Open serial port
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)  # Adjust port and baud rate as necessary

# UBX CFG-RATE payload: Measurement rate (200ms), navigation rate (1 cycle), time reference (GPS time)
payload = struct.pack('<HHH', 200, 1, 1)
header = struct.pack('<BBBB', 0xB5, 0x62, 0x06, 0x08)
length = struct.pack('<H', len(payload))

# Calculate checksum
checksum = calculate_ubx_checksum(header[2:] + length + payload)
footer = struct.pack('<BB', *checksum)

# Construct final UBX command
ubx_cmd = header + length + payload + footer

# Write UBX command to serial port
ser.write(ubx_cmd)

# Close serial port
ser.close()

