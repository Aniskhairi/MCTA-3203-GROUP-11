import serial
import time
from collections import deque
import math

SERIAL_PORT = 'COM9'
BAUD_RATE = 9600

# Buffers to store the last 40 data points (approx. 2 seconds of motion)
history_x = deque(maxlen=40)
history_y = deque(maxlen=40)


def is_circular_motion(x_data, y_data):
    """
    Analyzes an array of X and Y accelerometer data.
    Returns True if the data represents a circular motion, False otherwise.
    """
    if len(x_data) < 10:
        return False

    # --- STUDENTS: ADD YOUR CIRCLE DETECTION MATH HERE ---

    # Step 1: Find the center (mean) of the X and Y movement.
    mean_x = sum(x_data) / len(x_data)
    mean_y = sum(y_data) / len(x_data)

    x_centered = [x - mean_x for x in x_data]
    y_centered = [y - mean_y for y in y_data]

    # Step 2: Check Amplitude. Calculate the variance to ensure there is
    #         enough overall movement (ignore small jitters).
    var_x = sum((x ** 2 for x in x_centered)) / len(x_data)
    var_y = sum((y ** 2 for y in y_centered)) / len(x_data)

    amplitude = math.sqrt(var_x + var_y)
    if amplitude < 150:  # Adjust this
        return False

    # Step 3: Check Symmetry. Check the ratio of X standard deviation to Y
    #         standard deviation to ensure it is round, not a straight line.
    std_x = math.sqrt(var_x)
    std_y = math.sqrt(var_y)

    if std_y == 0:
        return False

    ratio = std_x / std_y
    if ratio < 0.5 or ratio > 2.0:
        return False

    # Step 4: Check Continuous Rotation. Use the 2D Cross Product (x1*y2 - y1*x2)
    #         between consecutive centered points to see which way the path is turning.
    clockwise = 0
    counter_clockwise = 0

    for i in range(len(x_data) - 1):
        x1 = x_centered[i]
        y1 = y_centered[i]
        x2 = x_centered[i + 1]
        y2 = y_centered[i + 1]

        cross = x1 * y2 - y1 * x2

        if cross < 0:
            clockwise += 1
        elif cross > 0:
            counter_clockwise += 1

    total = clockwise + counter_clockwise
    if total == 0:
        return False

    # Step 5: Tally the clockwise vs. counter-clockwise turns. If the movement
    #         is consistently turning in one direction (e.g., > 70%), return True!
    dominant = max(clockwise, counter_clockwise) / total

    if dominant > 0.7:  # Adjust this
        return True

    return False # Change this to return True when your math detects a circle


def main():
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.1)
    time.sleep(2)  # Wait for Arduino to reset
    print("System ready. Move the MPU6050 in a circle to toggle the LED...")

    # Keep track of the LED state
    led_is_on = False

    try:
        while True:
            raw = ser.readline().decode(errors='ignore').strip()
            if raw:
                parts = raw.split(',')

                if len(parts) >= 3:
                    try:
                        ax = int(parts[0])
                        ay = int(parts[1])

                        # Add new data to our buffers
                        history_x.append(ax)
                        history_y.append(ay)

                        # Only analyze if our buffer is full (40 points)
                        if len(history_x) == 40:
                            if is_circular_motion(history_x, history_y):
                                # Toggle the state
                                led_is_on = not led_is_on

                                if led_is_on:
                                    print("Circle Detected! Toggling LED: ON")
                                    ser.write(b'O')
                                else:
                                    print("Circle Detected! Toggling LED: OFF")
                                    ser.write(b'F')

                                # DEBOUNCE: Clear the buffers!
                                # Forces the system to wait for 2 full seconds of
                                # NEW data before it can trigger another toggle.
                                history_x.clear()
                                history_y.clear()

                    except ValueError:
                        pass  # Ignore corrupted serial lines

    except KeyboardInterrupt:
        print("\nProgram terminated by user.")
    finally:
        ser.write(b'F')  # Ensure LED turns off when script stops
        ser.close()
        print("Serial connection closed.")


if __name__ == "__main__":
    main()

