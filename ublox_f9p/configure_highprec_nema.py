import serial
import time

#
# Configure ZED-F9P to output extra precision in GGA (and similar sentences).
# Two extra digits are supplied on lat,lng, and alt.
#
def send_ubx_command(serial_port, command):
    with serial.Serial(serial_port, baudrate=115200, timeout=1) as ser:
        ser.write(command)
        time.sleep(0.1)

def enable_high_precision_nmea(serial_port):
    # UBX command to enable high precision NMEA output
    # Use UBX-CFG-VALSET with the key CFG-NMEA-HIGHPREC (0x10930006)
    #https://portal.u-blox.com/s/question/0D52p00009uoiC9CAI/how-to-set-high-precision-in-zedf9p

    #high_precision_command = bytes([0xB5, 0x62, 0x06, 0x17, 0x00, 0x00, 0x1D, 0x5A])
    high_precision_command = bytes([0xb5, 0x62, 0x06, 0x8a, 0x09, 0x00, 0x00, 0x01, 0x00, 0x00, 0x06, 0x00, 0x93, 0x10, 0x01, 0x44, 0x29 ])
    
    send_ubx_command(serial_port, high_precision_command)
    
    print("High precision NMEA output enabled.")

if __name__ == "__main__":

    serial_port = '/dev/ttyACM0'
    
    enable_high_precision_nmea(serial_port)
    

