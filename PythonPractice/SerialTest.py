import serial

def main():

	ser = serial.Serial('/dev/ttyUSB0', 9600)

	ser.open()

	ser.close()
}
