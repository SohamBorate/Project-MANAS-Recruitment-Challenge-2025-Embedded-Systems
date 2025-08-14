# Smart Farming Setup

### Components
- Arduino Uno R3: Need a microcontroller for the smartness

- Breadboard: There are way too many components to connect, a breadboard helped manage all those connections

- Moisture Sensor: Detects the amount of moisture present in the soil

- Servo: Controls the valve which determines the flow of water

- TMP36 Temperature Sensor: Measures the current temperature

- RGB LED: Used to display 3 different colors (red, green, blue) using just one LED

- Light Dependant Resistor: Used to detect the level of light, and dtermine the time of day

- 300 ohm resistor after RGB LED: Too much current without a resistor, would've lead to damage. 1000 ohms was safe but then 300 ohms increased brightness

- 1000 ohm resistor after LDR: It just wouldn't work without it

### How it works

At the top of the code there are multiple constants, which determine which action takes place when.

During each loop with a 1000 ms delay:
- Moisture level, Light level, Temperature in Celcius are stored into variables and are printed to the serial monitor
  - `moisture`
  - `light`
  - `temperatureC`

- Checks whether the moisture is under a certain value, if it is then the servo is set to the specific angle at which the valve is opened, otherwise the valve is closed.

- Checks whether the light is under a certain value, if it is then the `ideal_temp_lower` and `ideal_temp_upper` variables are changed according to the constants defined at the top of the code, and vice versa.

- Checks what range the temperature value lies in, and then accordingly lights up the LED, with different colors.