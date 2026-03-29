import serial
import time

SERIAL_PORT = 'COM9'
BAUD_RATE = 9600
AUTHORIZED_CARD = ("0013078065")

def main():
    # Establish connection with the Arduino
    arduino = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Wait for Arduino to reset

    print("RFID System Ready.")
    print("Scan a card to test the system. Press CTRL+C to stop.")

    try:
        while True:
            uid = input("\nTap card: ").strip()

            if uid == AUTHORIZED_CARD:
                print(f"[{uid}] - Card Authorized! Turning LED ON...")
                arduino.write(b'1')  # Send ON command to Arduino

                time.sleep(3)  # Keep LED on for 3 seconds

                arduino.write(b'0')  # Send OFF command
                print("LED OFF. Ready for next scan.")

            else:
                print(f"[{uid}] - Unauthorized Card! LED remains OFF.")

    except KeyboardInterrupt:
        print("\nExiting program.")
    finally:
        # Ensure the LED is safely turned off when the program closes
        arduino.write(b'0')
        arduino.close()
        print("Serial connection closed.")

if __name__ == "__main__":
    main()

