import re

import matplotlib
import matplotlib.animation
import matplotlib.patches
import matplotlib.pyplot as plt
import serial

SERIAL_PORT = "COM3"

arduino = serial.Serial(port=SERIAL_PORT, baudrate=115200, timeout=0.1)

x = 0
y = 0

fig, ax = plt.subplots(figsize=(6, 6))  # Adjust figure size as needed
plt.xlabel("X Axis")  # Label for X Axis
plt.ylabel("Y Axis")  # Label for Y Axis
plt.title("Analog Stick Output")  # Title of the plot
plt.xlim(-32768, 32768)  # Set limits for X Axis
plt.ylim(-32768, 32768)  # Set limits for Y Axis
plt.axhline(0, color="black", linewidth=0.5)  # Horizontal grid line at y=0
plt.axvline(0, color="black", linewidth=0.5)  # Vertical grid line at x=0
plt.grid(True, linestyle="--", alpha=0.7)  # Add grid lines
plt.gca().set_aspect("equal", adjustable="box")  # Make the aspect ratio equal

sc = plt.scatter(x, y, color="blue")  # Plot the initial point

def update():
    sc.set_offsets((x, y))
    return sc, 

def read_data():
    global x,y
    data = arduino.readline()
    data = data.strip()
    data = data.decode("ascii")
    data = data[data.find('('):]
    print(data)

    if len(data) > 0 and re.match(r"^\((-){0,1}[0-9]+,(-){0,1}[0-9]+\)$", data):
        data = data[1:-1]
        data = data.split(",")
        x = int(data[0])
        y = int(data[1])

    return x, y


def update_plot(frame):
    read_data()
    return update()

ani = matplotlib.animation.FuncAnimation(fig, update_plot, interval=20)
plt.show()
