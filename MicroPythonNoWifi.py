#Sample code for MicroPython and NeoPixel lights for DATTA SA

#modules setup
from time import sleep
import machine, neopixel
#from machine import Pin  #this is an alternative and would save writing machine.Pin and machine.HIGH read more here: https://stackoverflow.com/questions/710551/use-import-module-or-from-module-import

#neopixel setup
ledCount = 16                                         # number of pixels      
dataPin = 0                                           # data control gpio 0 pin (Wemos D3)
np = neopixel.NeoPixel(machine.Pin(dataPin), ledCount) 

#input setup
button = machine.Pin(2, machine.Pin.IN)     #setup a button on GPIO2 (Wemos D4) that will be used to switch LED on
mode = 1                                    # start light mode in off

print("NeoPixel sample code for DATTA SA!")

def clearLEDS():
  for i in range(ledCount):
    np[i] = (0, 0, 0)
  np.write()
    
def showLeds(r,g,b):              # pass r,g,b values into local variables to use in this function
  for i in range(ledCount):       # for each led in range 0 to number of pixels complete loop
      np[i] = (r, g, b)           # turn each pixel (i) to set rgb color
  np.write()                      # update neopixel strip with set values

while True:
  
  button_state = button.value()   #check button state to see if it is HIGH 1 or LOW 0

  if button_state == 0:                 # if button is being pressed (LOW) (normaly pulled high) then
    mode += 1                           # increase mode variable by one
    print("Light mode = " + str(mode))  # print the light mode to the terminal for feedback and debugging
  
  if mode == 1:                 # if mode is equal to 1 then:
    showLeds(255,0,0)           # turn leds to color (r,g,b) 0 is off 255 is full brightness.
  elif mode == 2:               # else if mode is equal to 2 then:
    showLeds(150,0,200)         # turn leds to color (r,g,b) 0 is off 255 is full brightness.
  elif mode == 3:
    showLeds(0,0,255)
  elif mode > 3:
    clearLEDS()                 # turn all leds off (0,0,0)
    mode = 0
    
  sleep(0.5)
