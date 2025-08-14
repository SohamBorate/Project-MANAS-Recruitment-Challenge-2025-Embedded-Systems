#include <Adafruit_NeoPixel.h>

// pixel array configurations
const int pixel_pin = 13;
const int pixel_rows = 10;
const int pixel_columns = 5;
const int pixel_num = pixel_rows * pixel_columns;

// pins
const int left_btn = 12;
const int right_btn = 11;
const int reset_btn = 10;
const int buzzer_pin = 9;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(pixel_num, pixel_pin, NEO_GRB + NEO_KHZ800);

// -------------
// game settings
// -------------

// the time interval between traffic moving forwards and new traffic spawning
const int move_traffic_interval = 1000; // miliseconds
// the time interval between two player movements
const int move_player_interval = 100; // miliseconds
// max number of cars that can be spawned at a time
const int max_cars = pixel_columns - 1;
// controls the spawn probability of traffic
const int ease = 3;

// --------------
// game variables
// --------------

// 2D array, which represents if a car is present at a specific pixel
int traffic[pixel_columns][pixel_rows];
// the column number of where the player is
int player = 0;
// the last timestamp when the traffic was moved
int last_move_traffic = 0;
// the last timestamp when the player moved
int last_move_player = 0;
// score ._.
int score = 0;
// a variable to tell various parts of the code that the game is over
int game_over = 0;

// --------------
// game functions
// --------------

// reset the LEDs, player position, and other variables
void reset() {
  for (int i = 0; i < pixel_columns; i++) {
    for (int j = 0; j < pixel_rows; j++) {
      traffic[i][j] = 0;
    }
  }
  player = 0;
  score = 0;
  game_over = 0;
}

// move traffic one pixel forward, and spawn in new traffic
void move_traffic() {
  // the amount of new traffic that has been spawned into the top row so far
  int new_traffic = 0;
  for (int i = 0; i < pixel_columns; i++) {
    // moves the row down
    for (int j = 0; j < pixel_rows - 1; j++) {
      traffic[i][j] = traffic[i][j + 1];
      traffic[i][j + 1] = 0;
    }

    // spawns a new car in that column
    // by a 1 in X (ease) chance
    // and only if the row has not crossed the limit
    if (random(ease) == 0 && new_traffic < max_cars) {
      traffic[i][pixel_rows - 1] = 1;
      new_traffic++;
    }
  }

  // for (int i = 0; i < pixel_columns; i++) {
  //   for (int j = 0; j < pixel_rows; j++) {
  //     Serial.print(traffic[i][j]);
  //     Serial.print(" ");
  //   }
  //   Serial.println("");
  // }
}

// lights up the LEDs, according to where the cars are and are not, and also the player
void light_leds() {
  // red if car
  // nothing if no car
  for (int i = 0; i < pixel_columns; i++) {
    for (int j = 0; j < pixel_rows; j++) {
      // converts the 2D array, to a 1D array, and lights up the specific LED in the Neopixel array
      if (traffic[i][j]) {
        pixels.setPixelColor(j + pixel_rows * i, pixels.Color(255, 0, 0));
      } else {
        pixels.setPixelColor(j + pixel_rows * i, pixels.Color(0, 0, 0));
      }
    }
  }
  // green for player
  pixels.setPixelColor(pixel_rows * player, pixels.Color(0, 255, 0));
  pixels.show();
}

// starter Arduino function
void setup() {
  Serial.begin(9600);
  randomSeed(random(100000));

  pinMode(left_btn, INPUT);
  pinMode(right_btn, INPUT);
  pinMode(reset_btn, INPUT);
  pinMode(buzzer_pin, OUTPUT);
  // start the neopixel array
  pixels.begin();
  // start the game... even though it says `reset`
  reset();
}

void loop() {
  // only light leds if changes happen
  int update_led = 0;
  // current timestamp
  const int mili = millis();
  // move traffic only if it has been a certain time interval, and game isnt over
  if ((mili - last_move_traffic) > move_traffic_interval && !game_over) {
    last_move_traffic = mili;
    // Serial.println("===========");
    move_traffic();

    // if there is a car in the bottom row, increment score
    for (int i = 0; i < pixel_columns; i++) {
      if (traffic[i][0] == 1) {
        score++;
        break;
      }
    }

    Serial.print("Score: ");
    Serial.println(score);
    // activate the buzzer
    tone(buzzer_pin, 800, 10);
    update_led = 1;
  }

  // take player input only if it has been a certain time interval, and there is infact space to move, and game isnt over
  if (digitalRead(left_btn) == HIGH && player > 0 && (mili - last_move_player) > move_player_interval && !game_over) {
    last_move_player = mili;
    player--;
    update_led = 1;
  } else if (digitalRead(right_btn) == HIGH && player < (pixel_columns - 1) && (mili - last_move_player) > move_player_interval && !game_over) {
    last_move_player = mili;
    player++;
    update_led = 1;
  }

  if (digitalRead(reset_btn) == HIGH) {
    reset();
    // tone(buzzer_pin, 800, 3000);
  }

  // ---------------
  // collision logic
  // ---------------
  // nothing fancy, just an if statement
  if (traffic[player][0] == 1 && !game_over) {
    game_over = 1;
    // turn the whole pixel array red
    for (int i = 0; i < pixel_columns; i++) {
      for (int j = 0; j < pixel_rows; j++) {
        pixels.setPixelColor(j + pixel_rows * i, pixels.Color(255, 0, 0));
      }
    }
    pixels.show();
    // activate the buzzer
    tone(buzzer_pin, 800, 2000);
    delay(3000);
    // reset();
    return;
  }

  // only light leds if changes happen
  if (update_led && !game_over) {
    light_leds();
  }

  delay(10);
}
