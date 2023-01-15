#include <Arduino.h>
#include <NeoPixelBus.h>
#include "snake.hpp"

// LED strip declaration
const uint16_t rows = 14;
const uint16_t rowLength = 30;
const uint16_t pixelCount = rows * rowLength;
const uint8_t pixelPin = 16;
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(pixelCount, pixelPin);

// Enums
enum runmode
{
	LIVE,
	DEBUG,
	DEBUGSCREEN,
};
runmode mode = DEBUG;

enum gameselection
{
	SNAKE,
	TICTACTOE,
	PONG,
};
gameselection game = SNAKE;

// Class declarations
Snake snake;

// Function declarations
void draw();
float calcColor(int color);
void debugMode();
void (*gameupdate)();

// Color Declarations
float saturation = 1.0f;
float lightness = 0.002f;

HslColor red(calcColor(0), saturation, lightness);
HslColor red_l(calcColor(25), saturation, lightness);
HslColor green(calcColor(140), saturation, lightness);
HslColor green_l(calcColor(85), saturation, lightness);
HslColor blue(calcColor(250), saturation, lightness);
HslColor blue_l(calcColor(180), saturation, lightness);
HslColor yellow(calcColor(70), saturation, lightness);
HslColor violet(calcColor(300), saturation, lightness);
HslColor white(calcColor(0), 0.1f, 0.01f);
HslColor black(calcColor(0), saturation, 0.0f);

char grid[rows][rowLength] = 
{
//  0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29
  {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}, //0
  {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}, //1
  {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}, //2
  {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}, //3
  {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}, //4
  {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}, //5
  {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}, //6
  {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}, //7
  {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}, //8
  {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}, //9
  {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}, //10
  {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}, //11
  {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}, //12
  {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}, //13
};

void setup()
{
    Serial.begin(115200);
    while (!Serial);
    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();
    Serial.println("Started in " + mode);

    // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();

    Serial.println();
    Serial.println("Running...");

    if(mode == DEBUGSCREEN) debugMode();

    switch(game)
    {
      case SNAKE:
        snake.initGame();
        break;
      case TICTACTOE:
      
        break;
      case PONG:
      
        break;
    }
}

void loop()
{
  delay(125);
  gameupdate();
}

void draw(HslColor *canvas) {
  for(int i=0; i<sizeof(canvas); i++) {
    strip.SetPixelColor(i, canvas[i]);
  }
  strip.Show();
}

float calcColor(int color) {
  return (float)color/360;
}

void debugMode() {
  HslColor canvas[pixelCount];
  canvas[4] = red;
  canvas[5] = red_l;
  canvas[6] = green;
  canvas[7] = green_l;
  canvas[8] = blue;
  canvas[9] = blue_l;
  canvas[10] = yellow;
  canvas[11] = violet;
  canvas[12] = white;
  canvas[13] = black;
  draw(canvas);
}