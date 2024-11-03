import time
import re

import serial
import vgamepad as vg

SERIAL_PORT = "COM3"

arduino = serial.Serial(port=SERIAL_PORT,  baudrate=115200, timeout=.1)
gamepad = vg.VX360Gamepad()


while True:
    data = arduino.readline()
    arduino.reset_input_buffer()
    data = data.strip()
    data = data.decode('ascii')


    if len(data) > 0 and re.match(r"^\((-){0,1}[0-9]+,(-){0,1}[0-9]+\)$", data):
        data = data[1:-1]
        data = data.split(',')
        x = int(data[0])
        y = int(data[1])
        print(x, y)
    
        gamepad.left_joystick(x_value=x, y_value=y)
        gamepad.update()

    time.sleep(0.01)
