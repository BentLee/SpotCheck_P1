
#include <Adafruit_NeoPixel.h>
//P1 move
#define BUTTON_PIN1_M   2   // Digital IO pin connected to the button.  This will be
// driven with a pull-up resistor so the switch should
// pull the pin to ground momentarily.  On a high -> low
//P1 place                // transition the button press logic will execute.
#define BUTTON_PIN1_P   3 // p = placement of light

#define SOFT_POT_PIN1   A0 //for sliders

#define PIXEL_PIN    6    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 25



Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int prevTime = 0;
int waveFrameTime = 0;
int WAVE_FRAME_MAX = 375;
int colLight = 0; //time global variables


class Player {
  public:
    int x;
    int y;

    int r;
    int g;
    int b;

    int direct; // set 1 for player 1, -1 for player 2
    Player(int xA, int yA, int directA)
    {
      x = xA;
      y = yA;
      direct = directA;
    } //end constructor
}; //end Player

Player player1(2, 0, 1);

//lights
int light1_0x, light1_1x, light1_2x, light1_3x, light1_4x, light1_5x, light1_6x;
int light1_0y, light1_1y, light1_2y, light1_3y, light1_4y, light1_5y, light1_6y;

//Controler varialbes
bool oldState1_M = HIGH;
bool oldState1_P = HIGH;

int blockCount1 = 0; //keeps track of what block in the cycle is placed

void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON_PIN1_P, INPUT_PULLUP);
  pinMode(BUTTON_PIN1_M, INPUT_PULLUP);

  pinMode(SOFT_POT_PIN1, INPUT);
  pinMode(SOFT_POT_PIN1, INPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(9600);


}

///////////////////////// Array for music input and light state/////////
int ledStMu[25][2] = {
  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
  {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
};
//end ledMuSt
//use ledNum to call coordinates

/////////////////////////Column light////////////
void lightColumn(int col) {
  for (int row = 0; row < 5; row++) {
    strip.setPixelColor(ledNum(col, row), 0, 10, 0);

  }
}

///////////////////////// timey wimey stuff///////

//array of coordinates////////////////
int ledPositions[5][5] = {
  {0, 1, 2, 3, 4},
  {5, 6, 7, 8, 9},
  {10, 11, 12, 13, 14},
  {15, 16, 17, 18, 19},
  {20, 21, 22, 23, 24}
};

int ledNum(int x, int y) {
  return ledPositions[y][x];
} //end ledNum
//////////////////////////////////


void loop() {
  int currTime = millis();
  int dt = currTime - prevTime;
  prevTime = currTime;
  waveFrameTime += dt; // local time variables
  bool stepChk = false;
  if (waveFrameTime > WAVE_FRAME_MAX) {
    colLight = (colLight + 1) % 4;
    waveFrameTime = 0;
    stepChk = true;
  }
  int a = ledNum(player1.x, player1.y);

  int ledArray [25] = {0};

  /* ///////////////////////////////////////////////////////////////////////
     PLAYER 1 STUFF START
  */ ///////////////////////////////////////////////////////////////////////

  //PLACE BLOCK /////////////////////////////////////////////////
  bool newState1_P = digitalRead(BUTTON_PIN1_P); //BUTTON_PIN1_P: block place

  // Check if state changed from high to low (button press).
  if (newState1_P == LOW && oldState1_P == HIGH) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState1_P = digitalRead(BUTTON_PIN1_P);
    ledStMu[a][0] = 1;
    ledStMu[a][1] = 1;
  }
  oldState1_P = newState1_P;
  //REMOVE BLOCK /////////////////////////////////////////////////

  bool RemState1_P = digitalRead(BUTTON_PIN1_P); //BUTTON_PIN1_P: block place
/*
  // Check if state changed from high to low (button press).
  if (RemState1_P == LOW && oldState1_P == HIGH && ledStMu[a][0] == 1) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    RemState1_P = digitalRead(BUTTON_PIN1_P);
    ledStMu[a][0] = 0;
    ledStMu[a][1] = 0;
  }
  oldState1_P = RemState1_P;
  */
  //MOVE Y ////////////////////////////////////////////////////////////
  bool newState1_M = digitalRead(BUTTON_PIN1_M);

  // Check if state changed from high to low (button press).
  if (newState1_M == LOW && oldState1_M == HIGH) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState1_M = digitalRead(BUTTON_PIN1_M);
    if (newState1_M == LOW) {
      //player1.y++;
      //Serial.print("BEFORE: ");
      //Serial.println(player1.y);
      jump(player1);
      //Serial.print("AFTER: ");
      //Serial.println(player1.y);
    }
  }
  oldState1_M = newState1_M;

  //MOVE X///////////////////////////////////////////////////////////////

  //read pot's ADC value
  int softPotADC1 = analogRead(SOFT_POT_PIN1);
  //map the 0-1023 to 0-4
  int softPotPosition1 = map(softPotADC1, 0, 668, 0, 3);
  player1.x = softPotPosition1;
  //Serial.println('x'+ player1.x);
  //Serial.println(softPotPosition1);





  ledArray[ledNum(player1.x, player1.y)] = 1; //set player


  // Clear all leds to off
  for (int i = 0; i < 24; i = i + 1) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));

    if (ledStMu[i][0] == 1) {
      strip.setPixelColor(i, strip.Color(0, 0, 10));
    }
  } //end for, resets color of pixels back to zero

  // Set marked leds to blue


  lightColumn(colLight); //column lighting function call

  /////////////MUSIC TRIGGERS//////
  if (stepChk){
    
  
  if (colLight == 0) {
    if (ledStMu[0][1] == 1) {
      Serial.print("1,");
      //Serial.print("b0b");
    }
    if (ledStMu[5][1] == 1) {
      Serial.print("2,");
      //Serial.print("beb");

    }
    if (ledStMu[10][1] == 1) {
      Serial.print("3,");

    }
    if (ledStMu[15][1] == 1) {
      Serial.print("4,");

    }
    if (ledStMu[20][1] == 1) {
      Serial.print("5,"); //may not need a comma

    }
  }

  if (colLight == 1) {
    if (ledStMu[1][1] == 1) {
      Serial.print("1,");
    }
    if (ledStMu[6][1] == 1) {
      Serial.print("2,");

    }
    if (ledStMu[11][1] == 1) {
      Serial.print("3,");

    }
    if (ledStMu[16][1] == 1) {
      Serial.print("4,");

    }
    if (ledStMu[21][1] == 1) {
      Serial.print("5,"); //may not need a comma

    }
  }
  if (colLight == 2) {
    if (ledStMu[2][1] == 1) {
      Serial.print("1,");
    }
    if (ledStMu[7][1] == 1) {
      Serial.print("2,");
    }
    if (ledStMu[12][1] == 1) {
      Serial.print("3,");
    }
    if (ledStMu[17][1] == 1) {
      Serial.print("4,");
    }
    if (ledStMu[22][1] == 1) {
      Serial.print("5,"); //may not need a comma
    }

  }
  if (colLight == 3) {
    if (ledStMu[3][1] == 1) {
      Serial.print("1,");
    }
    if (ledStMu[8][1] == 1) {
      Serial.print("2,");
    }
    if (ledStMu[13][1] == 1) {
      Serial.print("3,");
    }
    if (ledStMu[18][1] == 1) {
      Serial.print("4,");
    }
    if (ledStMu[23][1] == 1) {
      Serial.print("5,"); //may not need a comma
    }

  }
  }

  strip.setPixelColor(a, strip.Color(10, 10, 10));



  strip.show(); // This sends the updated pixel color to the hardware.
  /*
    Serial.print("Pot Readint =");
    Serial.print(softPotADC1);
    Serial.print(", x=");
    Serial.print(player1.x);
    Serial.print(", y=");
    Serial.print(player1.y);
    Serial.print(", number=");
    Serial.println(ledNum(player1.x, player1.y)); //coordinate of cursor
  */
} //end void loop
