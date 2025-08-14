# Highway Surfers

### Components

- Arduino Uno R3: Need a microcontroller for the game logic

- Breadboard: There are way too many components to connect, a breadboard helped manage all those connections

- Neopixel Strip 10 (x5): Pixel array, which represents the cars, player, highway

- Push buttons (x3): For player movement and resetting game

- 10,000 ohms resistor (x3) near push button: To stop the buttons from floating and pull it down so that in the unpressed state the buttons outputs LOW

- Piezo Buzzer: Buzzer just as an accesory, Piezo specifically because it was the only one available in Tinkercad

### How it works

At the top of the code there are multiple constants, which determine the difficulty and timing of the game.

During each loop with a 10 ms delay:
- The current timestamp is stored into a variable

- Checks if enough time has passed since the last traffic update and if the game isnt over, if so then moves the traffic one row down, and spawns in new random traffic at the top row. Incements the score if there is a car in the row same as the player. Prints the sscore to the serial monitor and plays a short buzzer tone.

- Checks if the left or right button is being pressed and the game isnt over, there is space to move, and enough time has passed, if so then moves the player.

- Checks if the reset button is being pressed, if so then resets the game.

- Checks if there is a car in the last row, that is in the same column as that of the player, if so then ends the game, and turns the whole pixel array red while the buzzer produces a constant sound for 3 seconds.

- Checks if the updating of LEDs is called for and that the game isnt over, if so then it updates the LEDs.
