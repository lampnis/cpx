from time import sleep
from adafruit_circuitplayground.express import cpx
# from gradient_colors import *

""" RGB color wheel (use one value instead of three) """

def colwheel(n):
    if n == 0:
        return (0, 0, 0)
    if n > 0 and n <= 85:
        return (0, int(255-n*3), int(3*n))
    if n > 85 and n <= 170:
        n -= 85
        return (int(n*3), 0, int(255-n*3))
    if n > 170 and n <= 255:
        n -= 170
        return (int(255-n*3), int(n*3), 0)

def tone():
    """Start/stop tone"""

    if cpx.button_a: # press button_a for tone
        cpx.start_tone(200) #freq for tone
    else:
        cpx.stop_tone() # tone plays while button is pressed

def passwords(passws):
    # cpx.pixels.fill((0, 1, 1))
    # print('Entered mode passwords')
    # kbd = Keyboard(usb_hid.devices)
    # layout = KeyboardLayoutUS(kbd)
    passw = ''
    counter = 0
    # after = counter + 1
    
    
    if cpx.button_a:
        print("a press")
        cpx.pixels.fill((0, 0, 0))
        cpx.pixels[counter] = (1, 0, 0)
        passw = passws[counter]
        counter += 1
        if counter > 9:
            counter = 0
        while cpx.button_a:
            pass
    
    if cpx.button_b:
        print("b press")
        layout.write('{}\n'.format(passw))
        cpx.pixels.fill((0, 1, 0))
        while cpx.button_b:
            pass   

def mode_3():
    cpx.pixels.fill((0, 1, 1))
    print('Mode 3 entered successfully')

def mode_4():
    cpx.pixels.fill((0, 1, 1))
    print('Mode 4 entered successfully')

def mode_5():
    cpx.pixels.fill((0, 1, 1))
    print('Mode 5 entered successfully')

def mode_6():
    cpx.pixels.fill((0, 1, 1))
    print('Mode 6 entered successfully')

def mode_7():
    cpx.pixels.fill((0, 1, 1))
    print('Mode 7 entered successfully')

def mode_8():
    cpx.pixels.fill((0, 1, 1))
    print('Mode 8 entered successfully')

def mode_9():
    cpx.pixels.fill((0, 1, 1))
    print('Mode 9 entered successfully')

def gradient_colors():
    for color in range(1, 255):
        if cpx.switch:
            break
        fill = colwheel(color) # colwheel needs to be coded or imported
        cpx.pixels.fill(fill)
        sleep(0.1)
    # if cpx.switch:
        # break


modes = [gradient_colors, tone, passwords, mode_3, mode_4, mode_5, mode_6, mode_7, mode_8, mode_9]
