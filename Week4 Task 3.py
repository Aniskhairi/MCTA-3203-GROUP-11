import serial
import time
import math

# --- CONFIGURATION ---
SERIAL_PORT = 'COM9' # Update with your Arduino port
BAUD_RATE = 9600
AUTHORIZED_ID = "0013078065"

# ---------------------

def detect_circle(arduino):
    """
    Asks Arduino to stream a burst of MPU data, calculates the rotation angles,
    and decides whether a circular motion happened.
    """
    print("\n[!] Requesting samples from Arduino… Move in a circle NOW!")
    arduino.reset_input_buffer()
    arduino.write(b'R')  # Trigger Arduino data stream

    points = []
    start_time = time.time()
    timeout = 3.0

    # Read until timeout (Arduino stops streaming automatically after 2s)
    while time.time() - start_time < timeout:
        raw = arduino.readline().decode(errors='ignore').strip()
        if not raw:
            continue

        parts = raw.split(',')
        if len(parts) >= 6:
            try:
                ax = int(parts[0])
                ay = int(parts[1])
                az = int(parts[2])
                points.append((ax, ay, az))
            except ValueError:
                continue

    print(f"Collected {len(points)} samples.")
    if len(points) < 8:
        print("Not enough motion data – try moving more continuously.")
        return False

    # Build angles from (ax, ay) and unwrap to follow continuous rotation
    angles = [math.degrees(math.atan2(p[1], p[0])) for p in points]

    unwrapped = [angles[0]]
    for a in angles[1:]:
        prev = unwrapped[-1]
        diff = a - prev

        # Unwrap angles to avoid -180/180 discontinuity leaps
        if diff > 180:
            diff -= 360
        elif diff < -180:
            diff += 360
        unwrapped.append(prev + diff)

    # Compute net rotation and range
    net_rotation = unwrapped[-1] - unwrapped[0]
    rotation_range = max(unwrapped) - min(unwrapped)

    print(f"Net rotation: {net_rotation: .1f}°, Range: {rotation_range: .1f}°")

    # Lenient detection logic (either a decent net rotation OR wide range)
    if abs(net_rotation) > 100 or rotation_range > 140:
        print("-> Circle gesture detected!")
        return True

    # Extra check: count direction-consistent large angle deltas
    deltas = [unwrapped[i + 1] - unwrapped[i]
    for i in range(len(unwrapped) - 1)]
    large_moves = sum(1 for d in deltas if abs(d) > 10)

    if large_moves >= max(6, len(points) // 4):
        print(" Motion had multiple consistent movements - accepting as circle.")
        return True

    print(" Motion insufficient.Try a clearer circular motion.")
    return False


def main():
    arduino = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Wait for Arduino to reset

    print("System Ready.Waiting for RFID…")
    armed = False  # System starts locked

    try:
        while True:
            # USB RFID acts as a keyboard
            card = input("\nTap card: ").strip()

            if card == AUTHORIZED_ID:
                armed = not armed  # Toggle state

                if armed:
                    print("Card Accepted - Please perform circular motion verification!")

                    if detect_circle(arduino):
                        print("Gesture OK -> System UNLOCKED(LED Green)")
                        arduino.write(b'1')
                    else:
                        print("Wrong gesture -> Try again!")
                        armed = False  # Revert state if gesture fails
                else:
                    print("System Disarmed -> System LOCKED(LED Red)")
                    arduino.write(b'0')

            else:
                print("WRONG CARD -> Access Denied")
                arduino.write(b'X')

    except KeyboardInterrupt:
        print("\nExiting.")
    finally:
        arduino.write(b'0')  # Ensure locked on exit
        arduino.close()

if __name__ == "__main__":
    main()
