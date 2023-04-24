import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)

red = 18
GPIO.setup(red, GPIO.OUT)
GPIO.output(red, GPIO.LOW)