import random
import time
 
import RPi.GPIO as GPIO
GPIO.setmode (GPIO.BOARD)
GPIO.setwarnings(False)
 
red = 18
yellow = 22
green = 24
blue = 26
 
GPIO.setup(red, GPIO.OUT)
GPIO.setup(yellow, GPIO.OUT)
GPIO.setup(green, GPIO.OUT)
GPIO.setup(blue, GPIO.OUT)
 
GPIO.setup(32, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(36, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(38, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(40, GPIO.IN, pull_up_down=GPIO.PUD_UP)
 
game = True
 
pattern = []
lights = [red, yellow, green, blue]
 
while game:
    pattern.append(random.randint(0,3))
    
    for x in pattern:
        GPIO.output(lights[x], GPIO.HIGH)
        time.sleep(1)
        GPIO.output(lights[x], GPIO.LOW)
        time.sleep(0.5)
    
    for x in pattern:
        
        waitingForInput = True
        
        while waitingForInput:
            redButtonState = GPIO.input(32)
            yellowButtonState = GPIO.input(36)
            greenButtonState = GPIO.input(38)
            blueButtonState = GPIO.input(40)
            
            if redButtonState == 0:
                GPIO.output(red, GPIO.HIGH)
                waitingForInput = False
                if x != 0:
                    game = False
                time.sleep(1)
                GPIO.output(red, GPIO.LOW)
                
            if yellowButtonState == 0:
                GPIO.output(yellow, GPIO.HIGH)
                waitingForInput = False
                if x != 1:
                    game = False
                time.sleep(1)
                GPIO.output(yellow, GPIO.LOW)
            
            if greenButtonState == 0:
                GPIO.output(green, GPIO.HIGH)
                waitingForInput = False
                if x != 2:
                    game = False
                time.sleep(1)
                GPIO.output(green, GPIO.LOW)
            
            if blueButtonState == 0:
                GPIO.output(blue, GPIO.HIGH)
                waitingForInput = False
                if x != 3:
                    game = False
                time.sleep(1)
                GPIO.output(blue, GPIO.LOW)
        
    time.sleep(1)
 