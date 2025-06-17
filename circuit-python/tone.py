"""Start/stop tone"""

if cpx.button_a: # press button_a for tone
    cpx.start_tone(200) #freq for tone
else:
    cpx.stop_tone() # tone plays while button is pressed