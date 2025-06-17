#include <Arduino.h> // Standard Arduino library
#include <Adafruit_CircuitPlayground.h> // For CPX specific features like NeoPixels, buttons, switch, tone
#include <Keyboard.h> // For USB HID keyboard emulation (ensure this library is installed if not default)

// Define the pins for the joystick and switch as they are accessed directly in Arduino
// (These are often the same as board.A1, board.A2, board.A3 in CircuitPython)
#define JOY_VRX_PIN A1
#define JOY_VRY_PIN A2
#define JOY_SW_PIN A3 // The joystick button/switch

// Global variables
int counter = 0; // Global counter for mode selection
String currentPassw = ""; // Stores the currently selected password

// Dummy list of passwords (using const char* for memory efficiency, conversion to String for printing)
const char* passwordList[] = {
    "pass123",
    "securePW",
    "admin",
    "guest",
    "MySecretCode",
    "anotherpass",
    "testing123",
    "finalone",
    "short",
    "verylongpassword"
};
const int NUM_PASSWORDS = sizeof(passwordList) / sizeof(passwordList[0]); // Get the number of passwords

unsigned long lastUpdateTime = 0;
const int updateInterval = 10; // 1/n updates per second

// --- Function Prototypes ---
// Declare functions before setup() and loop() if they are called before being defined.
// This is good practice in C++.
void joystick();
uint32_t colwheel(int n, float brightness); // Pass RGB by reference
void tone_player(); // Renamed to avoid conflict with Arduino tone()
void passwords();
void mode_3();
void mode_4();
void mode_5();
void mode_6();
void mode_7();
void mode_8();
void mode_9();
void gradient_colors();


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/////////////// ARDUINO SETUP FUNCTION #/////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void setup() {
    // Initialize serial communication for debugging output
    // This is equivalent to CircuitPython's implicit print()
    Serial.begin(115200); // Higher baud rate for faster output

    // Initialize Circuit Playground Express library
    CircuitPlayground.begin();

    // Initialize Keyboard for USB HID emulation
    Keyboard.begin();

    // Seed the random number generator using an analog read from an unconnected pin
    // This provides a somewhat random seed for random passwords
    randomSeed(analogRead(A0)); // A0 is usually floating, providing noise
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/////////////// ARDUINO MAIN LOOP #//////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

// The main loop() function runs continuously
void loop() {
    // Check the slide switch state
    // CircuitPython: not cpx.switch (if false then true, if true then false)
    // Arduino: CircuitPlayground.slideSwitch() (true for one side, false for other)
    if (!CircuitPlayground.slideSwitch()) { // Assuming 'not cpx.switch' means switch is to the right
        Serial.println("Switch is turned to right");
        // This loop will execute the current mode function repeatedly as long as the switch is right.
        // It's a bit different from the Python logic where it might break out immediately.
        // We'll rely on the mode function itself to break if the switch changes inside.
        // The original Python has a `while not cpx.switch` that can also enter a `if cpx.switch: break`
        // which implies an exit condition from the `modes[counter]()` call.
        // In Arduino, we need to ensure the called function also respects the switch change.
        CircuitPlayground.strip.fill(CircuitPlayground.strip.Color(0, 0, 0)); // Turn all pixels off
        CircuitPlayground.strip.setPixelColor(counter, CircuitPlayground.strip.Color(255, 0, 0)); // Red for current mode
        CircuitPlayground.strip.show(); // Update NeoPixels

        // Call the function pointer based on the counter
        // This array of function pointers needs to be defined globally.
        // Array of function pointers (must be defined outside loop/setup)
        void (*modes[])() = {
            gradient_colors, tone_player, passwords, joystick, mode_4,
            mode_5, mode_6, mode_7, mode_8, mode_9
        };
        modes[counter](); // Execute the selected mode function
    } else { // Switch is turned to left (or its default position)
        CircuitPlayground.strip.fill(CircuitPlayground.strip.Color(0, 0, 0)); // Turn all pixels off
        CircuitPlayground.strip.setPixelColor(counter, CircuitPlayground.strip.Color(255, 0, 0)); // Red for current mode
        CircuitPlayground.strip.show(); // Update NeoPixels

        if (CircuitPlayground.rightButton()) {
            Serial.println("B button press");
            counter++; // Increment counter
            if (counter > 9) { // Reset counter if it exceeds the number of modes
                counter = 0;
            }
            Serial.print("Current Mode: ");
            Serial.println(counter);

            CircuitPlayground.strip.fill(CircuitPlayground.strip.Color(0, 0, 0));
            CircuitPlayground.strip.setPixelColor(counter, CircuitPlayground.strip.Color(255, 0, 0));
            CircuitPlayground.strip.show();
            while (CircuitPlayground.rightButton()) { // Debounce button
                delay(10);
            }
        }

        if (CircuitPlayground.leftButton()) {
            Serial.println("A button press");
            counter--; // Decrement counter
            if (counter < 0) { // Reset counter if it goes below zero
                counter = 9;
            }
            Serial.print("Current Mode: ");
            Serial.println(counter);

            CircuitPlayground.strip.fill(CircuitPlayground.strip.Color(0, 0, 0));
            CircuitPlayground.strip.setPixelColor(counter, CircuitPlayground.strip.Color(255, 0, 0));
            CircuitPlayground.strip.show();
            while (CircuitPlayground.leftButton()) { // Debounce button
                delay(10);
            }
        }
    }
    delay(10); // Small delay to prevent busy-waiting too much
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/////////////// FUNCTIONS IMPLEMENTATION #///////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

/**
 * @brief Reads joystick values and prints min/max to Serial.
 * Note: This version stores min/max globally or needs to pass them around.
 * For simplicity, min/max are reset on each call in this translation.
 * In a more robust C++ version, you'd manage these bounds differently (e.g., as static variables
 * or members of a joystick class if you were writing one).
 */
void joystick() {
    // These min/max variables are local to this function and will reset each time it's called.
    // If you need persistent min/max, they should be global or static.
    static int x_max = 0;
    static int x_min = 70000;
    static int y_max = 0;
    static int y_min = 70000;
    static int s_max = 0;
    static int s_min = 70000;

    // The CircuitPython code's try/except MemoryError and collect() are not needed here
    // as C++ handles memory differently and is more efficient by design.
    // The `sleep(0.1)` (delay(100)) acts as the loop rate.
    // The `while(true)` in the original joystick function would make it never return.
    // We'll make it run once per `loop()` iteration if selected, or add its own internal loop
    // that breaks on switch change. For now, it runs once per `loop()` call.

    // Read the analog values from the joystick pins
    int x = analogRead(JOY_VRX_PIN);
    int y = analogRead(JOY_VRY_PIN);
    int s = analogRead(JOY_SW_PIN); // Read the joystick button/switch

    // Update min/max values
    if (x > x_max) x_max = x;
    if (x < x_min) x_min = x;
    if (y > y_max) y_max = y;
    if (y < y_min) y_min = y;
    if (s > s_max) s_max = s;
    if (s < s_min) s_min = s;

    // Print the analog values to the serial console
    Serial.print("MIN X: "); Serial.print(x_min);
    Serial.print(", MIN Y: "); Serial.print(y_min);
    Serial.print(", MIN SW: "); Serial.println(s_min);
    Serial.print("MAX X: "); Serial.print(x_max);
    Serial.print(", MAX Y: "); Serial.print(y_max);
    Serial.print(", MAX SW: "); Serial.println(s_max);
    Serial.print("X: "); Serial.print(x);
    Serial.print(", Y: "); Serial.print(y);
    Serial.print(", SW: "); Serial.println(s);

    delay(100); // Wait for a short period of time (0.1 seconds)

    // The original code had `x = None`, `collect()`. These are not applicable in C++.
    // Variables go out of scope or memory is managed automatically.
}

/**
 * @brief RGB color wheel function. Converts a single value (0-255) to an RGB tuple.
 * Outputs RGB values by reference.
 * @param n Input value (0-255).
 * @param brightness Brightness multiplier (0.0 to 1.0).
 */
uint32_t colwheel(int n, float brightness) {
    int r; int g; int b;
    if (n == 0) {
        r = 1; g = 1; b = 1;
        return CircuitPlayground.strip.Color(r, g, b);
    }
    // Ensure n is within 0-255 range for safety
    n = constrain(n, 0, 255);

    if (n > 0 && n <= 85) {
        r = 0;
        g = static_cast<uint8_t>((255 - n * 3) * brightness);
        b = static_cast<uint8_t>((3 * n) * brightness);
         return CircuitPlayground.strip.Color(r, g, b);
    }
    
    if (n > 85 && n <= 170) {
        n -= 85;
        r = static_cast<uint8_t>((n * 3) * brightness);
        g = 0;
        b = static_cast<uint8_t>((255 - n * 3) * brightness);
         return CircuitPlayground.strip.Color(r, g, b);
    }
    
    if (n > 170 && n <= 255) {
        n -= 170;
        r = static_cast<uint8_t>((255 - n * 3) * brightness);
        g = static_cast<uint8_t>((n * 3) * brightness);
        b = 0;
        return CircuitPlayground.strip.Color(r, g, b);
    }

}

/**
 * @brief Generates a color from a wheel using sine waves for a perfectly smooth transition.
 * @param pos A value from 0-255 representing the position on the color wheel.
 * @param brightness A value from 0.0 to 1.0.
 * @return A 32-bit packed RGB color.
 */
uint32_t smoothColorWheel(byte pos, float brightness) {
    // Map the 0-255 position to a 0-2PI angle in radians
    float angle = (pos / 255.0) * 2.0 * PI;

    // Calculate the RGB components using sine waves offset by 120 degrees (2*PI/3)
    // The sine function outputs from -1 to 1. We scale it to 0-1 and then to 0-255.
    float r = (sin(angle) * 0.5 + 0.5);
    float g = (sin(angle + 2.0 * PI / 3.0) * 0.5 + 0.5);
    float b = (sin(angle + 4.0 * PI / 3.0) * 0.5 + 0.5);

    // Apply brightness and convert to 8-bit values
    uint8_t r_byte = r * 255 * brightness;
    uint8_t g_byte = g * 255 * brightness;
    uint8_t b_byte = b * 255 * brightness;

    return CircuitPlayground.strip.Color(r_byte, g_byte, b_byte);
}

/**
 * @brief Starts/stops a tone based on button A/B presses.
 */
void tone_player() { // Renamed to avoid conflict with Arduino's tone() function
    if (CircuitPlayground.leftButton()) { // press button_a for tone
        CircuitPlayground.playTone(10, 100); // freq for tone
    } else if (CircuitPlayground.rightButton()) {
        CircuitPlayground.playTone(900, 100); // freq for tone
    } else {
        Serial.println("Not playing any sound"); // tone plays while button is pressed
    }
    delay(10); // Small delay to avoid rapid tone switching
}

/**
 * @brief Generates a random 4-digit password on button A press and types it on button B press.
 * Exits if the slide switch is turned off (right).
 */
void passwords() {
    static int passwordIndex = 0; // Static to retain value across function calls
    Serial.println("Entered passwords mode.");

    // Loop until the slide switch is moved to the right (off)
    while (!CircuitPlayground.slideSwitch()) { // Loop while switch is to the left (assuming this is "on" for passwords mode)
        // Indicate current password by lighting up pixels (e.g., number of lit pixels = password index + 1)
        CircuitPlayground.strip.fill(CircuitPlayground.strip.Color(0, 0, 0)); // Clear all pixels
        for (int i = 0; i <= passwordIndex; i++) {
            CircuitPlayground.strip.fill(CircuitPlayground.strip.Color(0, 0, 0));
            CircuitPlayground.strip.setPixelColor(i, CircuitPlayground.strip.Color(0, 0, 20)); // Blue light for selection
        }
        CircuitPlayground.strip.show();

        if (CircuitPlayground.leftButton()) { // Button A: Cycle through passwords
            Serial.println("A button press (Next Password)");
            passwordIndex++;
            if (passwordIndex >= NUM_PASSWORDS) {
                passwordIndex = 0; // Wrap around to the start
            }
            Serial.print("Selected Password Index: ");
            Serial.println(passwordIndex);
            Serial.print("Selected Password: ");
            Serial.println(passwordList[passwordIndex]); // Print the selected password for debugging
            CircuitPlayground.strip.fill(CircuitPlayground.strip.Color(0, 0, 0)); // Clear pixels for visual feedback
            CircuitPlayground.strip.setPixelColor(0, CircuitPlayground.strip.Color(255, 0, 0)); // Red for selection change
            CircuitPlayground.strip.show();
            while (CircuitPlayground.leftButton()) { // Debounce
                delay(10);
            }
        }

        if (CircuitPlayground.rightButton()) { // Button B: Simulate keystrokes for selected password
            Serial.println("B button press (Type Password)");
            CircuitPlayground.strip.fill(CircuitPlayground.strip.Color(0, 255, 0)); // Green for typing action
            CircuitPlayground.strip.show();

            // Type the selected password
            Keyboard.print(passwordList[passwordIndex]);
            Keyboard.press(KEY_RETURN); // Simulate Enter key press
            Keyboard.releaseAll(); // Release all keys

            Serial.print("Typed: ");
            Serial.println(passwordList[passwordIndex]);

            while (CircuitPlayground.rightButton()) { // Debounce
                delay(10);
            }
        }

        // Check if the slide switch is turned off (to the right) to exit this mode
        if (CircuitPlayground.slideSwitch()) {
            Serial.println("Exiting passwords mode due to switch change.");
            break;
        }
        delay(10); // Small delay to prevent busy-waiting
    }
    // Optional: Clear pixels when exiting the mode
    CircuitPlayground.strip.fill(CircuitPlayground.strip.Color(0,0,0));
    CircuitPlayground.strip.show();
}

/**
 * @brief Placeholder for mode 3.
 */
void mode_3() {
    CircuitPlayground.strip.fill(colwheel(90, 0.5));
    CircuitPlayground.strip.show();
    Serial.println("Mode 3 entered successfully");
    delay(100); // Keep LED on for a bit
}

/**
 * @brief Placeholder for mode 4.
 */
void mode_4() {
    CircuitPlayground.strip.fill(colwheel(120, 0.5));
    CircuitPlayground.strip.show();
    Serial.println("Mode 4 entered successfully");
    delay(100);
}

/**
 * @brief Placeholder for mode 5.
 */
void mode_5() {
    CircuitPlayground.strip.fill(colwheel(150, 0.5));
    CircuitPlayground.strip.show();
    Serial.println("Mode 5 entered successfully");
    delay(100);
}

/**
 * @brief Placeholder for mode 6.
 */
void mode_6() {
    CircuitPlayground.strip.fill(colwheel(180, 0.5));
    CircuitPlayground.strip.show();
    Serial.println("Mode 6 entered successfully");
    delay(100);
}

/**
 * @brief Placeholder for mode 7.
 */
void mode_7() {
    CircuitPlayground.strip.fill(colwheel(210, 0.5));
    CircuitPlayground.strip.show();
    Serial.println("Mode 7 entered successfully");
    delay(100);
}

/**
 * @brief Placeholder for mode 8.
 */
void mode_8() {
    CircuitPlayground.strip.fill(colwheel(240, 0.5));
    CircuitPlayground.strip.show();
    Serial.println("Mode 8 entered successfully");
    delay(100);
}

/**
 * @brief Placeholder for mode 9.
 */
void mode_9() {
    CircuitPlayground.strip.fill(colwheel(30, 0.5));
    CircuitPlayground.strip.show();
    Serial.println("Mode 9 entered successfully");
    delay(100);
}


/**
 * @brief Displays a perfectly smooth, non-blocking color gradient.
 */
void gradient_colors() {
    if (millis() - lastUpdateTime > updateInterval) {
        static int colorPosition = 0;
        lastUpdateTime = millis();

        // Use the new, smoother color wheel function
        CircuitPlayground.strip.fill(smoothColorWheel(colorPosition, 0.4));
        CircuitPlayground.strip.show();

        colorPosition++;
        // No need to check for > 255, as 'colorPosition' is a byte and will automatically wrap from 255 to 0.
        // But for clarity, we can cast it to a byte.
        colorPosition = (byte)(colorPosition + 1);
    }
}

// IMPORTANT: Define the array of function pointers globally or make it static inside loop()
// This cannot be defined outside of a function if it directly refers to functions defined after it.
// So, we'll define it where it's used, within the `loop()` function.
// This is already done in the `loop()` function itself for clarity.