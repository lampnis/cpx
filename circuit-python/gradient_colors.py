from adafruit_circuitplayground.express import cpx
from time import sleep
import colwheel
""" Insert this while loop for cpx,
to make colors change gradually,
when switch is turned on """

while True:

    if cpx.switch:

        for color in range(1, 255):
            fill = colwheel(color) # colwheel needs to be coded or imported
            cpx.pixels.fill(fill)
            sleep(0.1)

    else:
        
        cpx.pixels.fill((0, 0, 0))