# import necessary libs
import usb_hid  # type: ignore
from adafruit_hid.keyboard import Keyboard  # type: ignore
from adafruit_hid.keycode import Keycode  # type: ignore # noqa: F401
from adafruit_hid.keyboard_layout_us import KeyboardLayoutUS  # type: ignore
from adafruit_circuitplayground.express import cpx  # type: ignore
from time import sleep
import random
import analogio  # type: ignore
import board  # type: ignore
from gc import collect


def joystick():
    vrx = analogio.AnalogIn(board.A1)
    vry = analogio.AnalogIn(board.A2)
    sw = analogio.AnalogIn(board.A3)
    x_max = 0
    x_min = 70000
    y_max = 0
    y_min = 70000
    s_max = 0
    s_min = 70000

    while True:
        try:
            x = vrx.value
            y = vry.value
            s = sw.value

            if x > x_max:
                x_max = x
            if x < x_min:
                x_min = x

            if y > y_max:
                y_max = y
            if y < y_min:
                y_min = y

            if s > s_max:
                s_max = s
            if s < s_min:
                s_min = s

            print(f"MIN X: {x_min}, MIN Y: {y_min}, MIN SW: {s_min}")
            print(f"MAX X: {x_max}, MAX Y: {y_max}, MAX SW: {s_max}")
            print("X: ", x, "Y: ", y, "SW: ", s)

            sleep(0.1)
            x = None
            y = None
            s = None
            x_max = None
            x_min = None
            y_max = None
            y_min = None
            s_min = None
            s_max = None
            collect()

        except MemoryError:
            print(f"Memory error: only {gc.mem_free()} bytes free\n")


def colwheel(n, brightness):
    if n == 0:
        return (0, 0, 0)
    if n > 0 and n <= 85:
        return tuple([i * brightness for i in (0, int(255 - n * 3), int(3 * n))])
    if n > 85 and n <= 170:
        n -= 85
        return tuple([i * brightness for i in (int(n * 3), 0, int(255 - n * 3))])
    if n > 170 and n <= 255:
        n -= 170
        return tuple([i * brightness for i in (int(255 - n * 3), int(n * 3), 0)])


def tone():
    if cpx.button_a:  # press button_a for tone
        cpx.start_tone(10)  # freq for tone
    elif cpx.button_b:
        cpx.start_tone(900)  # freq for tone
    else:
        cpx.stop_tone()  # tone plays while button is pressed


def passwords():
    kbd = Keyboard(usb_hid.devices)
    layout = KeyboardLayoutUS(kbd)

    while True:  # Keep listening for button presses
        if cpx.button_a:
            cpx.pixels.fill((0, 0, 0))
            # Generate password (example: a simple number sequence)
            passw = str(random.randint(1000, 9999))  # Or any other method
            cpx.pixels[0] = (1, 0, 0)  # visualize something
            while cpx.button_a:  # Debouncing
                pass

        if cpx.button_b:
            layout.write("{}\n".format(passw))
            cpx.pixels.fill((0, 1, 0))
            print(passw)
            while cpx.button_b:  # Debouncing
                pass

        if not cpx.switch:
            break  # exit the loop if the switch is turned


# def passwords():
#     # cpx.pixels.fill((0, 1, 1))
#     # print('Entered mode passwords')
#     # kbd = Keyboard(usb_hid.devices)
#     # layout = KeyboardLayoutUS(kbd)
#     kbd = Keyboard(usb_hid.devices)
#     layout = KeyboardLayoutUS(kbd)

#     passws = ['1234',
#               '1234',
#               '1234', '1234', '1', '1ll', '1', 'q', 'u$er h4cked y0u']


#     passw = ''
#     p_counter = 0
#     # after = counter + 1
#     if cpx.button_a:
#         cpx.pixels.fill((0, 0, 0))
#         p_counter += 1
#         print(p_counter)
#         cpx.pixels[p_counter] = (1, 0, 0)
#         passw = passws[p_counter]

#         if p_counter > 9:
#             p_counter = 0
#         while cpx.button_a:
#             pass

#     if cpx.button_b:
#         layout.write('{}\n'.format(passw))
#         cpx.pixels.fill((0, 1, 0))
#         print(passw)
#         while cpx.button_b:
#             pass


def mode_3():
    cpx.pixels.fill((0, 1, 1))
    print("Mode 3 entered successfully")


def mode_4():
    cpx.pixels.fill((0, 1, 1))
    print("Mode 4 entered successfully")


def mode_5():
    cpx.pixels.fill((0, 1, 1))
    print("Mode 5 entered successfully")


def mode_6():
    cpx.pixels.fill((0, 1, 1))
    print("Mode 6 entered successfully")


def mode_7():
    cpx.pixels.fill((0, 1, 1))
    print("Mode 7 entered successfully")


def mode_8():
    cpx.pixels.fill((0, 1, 1))
    print("Mode 8 entered successfully")


def mode_9():
    cpx.pixels.fill((0, 1, 1))
    print("Mode 9 entered successfully")


def gradient_colors():
    for color in range(1, 255):
        if cpx.switch:
            break
        fill = colwheel(color, brightness=0.1)  # colwheel needs to be defined
        cpx.pixels.fill(fill)
        sleep(0.1)
    # if cpx.switch:
    # break


modes = [
    gradient_colors,
    tone,
    passwords,
    joystick,
    mode_4,
    mode_5,
    mode_6,
    mode_7,
    mode_8,
    mode_9,
]

counter = 0

while True:
    # cpx.pixels.fill((1, 1, 1))
    # print(cpx.switch)

    if not cpx.switch:
        print("switch is turned to right")
        while not cpx.switch:
            if cpx.switch:
                cpx.pixels.fill((0, 0, 0))
                cpx.pixels[counter] = (1, 0, 0)

                break

            modes[counter]()

    else:
        cpx.pixels.fill((0, 0, 0))
        cpx.pixels[counter] = (1, 0, 0)
        if cpx.button_b:
            print("b press")

            if counter > 9:
                counter = 0
            counter += 1
            if counter > 9:
                counter = 0
            print(counter)

            cpx.pixels.fill((0, 0, 0))
            cpx.pixels[counter] = (1, 0, 0)
            while cpx.button_b:
                pass

        if cpx.button_a:
            print("a press")

            if counter < 0:
                counter = 9
            counter -= 1
            if counter < 0:
                counter = 9
            print(counter)

            cpx.pixels.fill((0, 0, 0))
            cpx.pixels[counter] = (1, 0, 0)

            while cpx.button_a:
                pass

# Replace the code above with either of these for a single functionality!
#     #PASSWORD MANAGER#
#     if cpx.switch:
#         if cpx.button_a:
#             print("a press")
#             cpx.pixels.fill((0, 0, 0))
#             cpx.pixels[counter] = (1, 0, 0)
#             passw = passws[counter]
#             counter += 1
#             if counter > 9:
#                 counter = 0
#             while cpx.button_a:
#                 pass

#         if cpx.button_b:
#             print("b press")
#             layout.write('{}\n'.format(passw))
#             cpx.pixels.fill((0, 1, 0))
#             while cpx.button_b:
#                 pass

#     #TEMP SENSOR#
#     else:
#         for i in range(round(cpx.temperature/10)):
#             if i > -1:
#                 cpx.pixels[i]=((1,0,0))
#             if i <= -1:
#                 cpx.pixels[-i+1]=((0,0,1))
#         sleep(1)
#         cpx.pixels.fill((0,0,0))
#         for i in range(round(cpx.temperature)-round(cpx.temperature/10)*10):
#             cpx.pixels[i]=((0,1,0))
#         sleep(1)
#         cpx.pixels.fill((0,0,0))
