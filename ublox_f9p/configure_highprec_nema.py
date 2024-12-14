import serial
import time

def send_ubx_command(serial_port, command):
    """Sends a UBX command to the GNSS module."""
    with serial.Serial(serial_port, baudrate=9600, timeout=1) as ser:
        ser.write(command)
        time.sleep(0.1)  # Wait for the command to be processed

def enable_high_precision_nmea(serial_port):
    """Enables high precision mode for NMEA output."""
    # UBX command to enable high precision NMEA output
    high_precision_command = bytes([0xB5, 0x62, 0x06, 0x17, 0x00, 0x00, 0x1D, 0x5A])
    
    # Send the command to the GNSS module
    send_ubx_command(serial_port, high_precision_command)
    
    print("High precision NMEA output enabled.")

def save_configuration(serial_port):
    """Saves the current configuration to non-volatile memory."""
    # UBX command to save the current configuration
    save_command = bytes([0xB5, 0x62, 0x06, 0x09, 0x00, 0x00, 0x0F, 0x30])
    
    # Send the command to save the configuration
    send_ubx_command(serial_port, save_command)
    
    print("Configuration saved to non-volatile memory.")

if __name__ == "__main__":
    # Replace with your serial port (e.g., '/dev/ttyACM0')
    serial_port = '/dev/ttyACM0'
    
    # Enable high precision NMEA output
    enable_high_precision_nmea(serial_port)
    
    # Optional: Save the configuration to non-volatile memory
    #save_configuration(serial_port)


