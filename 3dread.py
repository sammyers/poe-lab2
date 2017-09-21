from serial import Serial
import csv

SERIAL_PORT = '/dev/cu.usbmodem1411'
BAUD_RATE = 9600

def read_values(arduino):
    with open('3dscan.csv', 'w+') as csvfile:
        csvfile.write('x_rotation, z_rotation, distance\r\n')
        while True:
            line = arduino.readline()
            if line:
                values = line.decode('utf-8')
                print(values)
                csvfile.write(values)



if __name__ == '__main__':
    arduino = Serial(SERIAL_PORT, BAUD_RATE, timeout=.1)
    read_values(arduino)