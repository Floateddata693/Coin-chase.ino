#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

AsyncDelay delay_1m;

bool slide = 0;
int i = 0;
int enemypos = -1;
int coinpos = -1;
int coins = 0;
int enemynum = 0;
bool alive = 0;
int score = 0;
int loops = 0;
bool death = 0;
int level = 0;
int loopthreshM = 1;
int loopthreshC = 1;



void setup() {
  CircuitPlayground.begin(10);
  Serial.begin(9600);
  CircuitPlayground.clearPixels();
  while (!Serial) {
    delay(10);
  }
 

  delay_1m.start(60000, AsyncDelay::MILLIS);


  attachInterrupt(digitalPinToInterrupt(7), SlideS, CHANGE);


  Serial.println("booting up");
  int boot = 0;
  for (int cycle = 0; cycle < 4; cycle++) {
    CircuitPlayground.setPixelColor(9 - boot, 255, 255, 255);
    CircuitPlayground.setPixelColor(0 + boot, 255, 255, 255);
    boot += 1;
    delay(500);
    if (boot == 4) {
      CircuitPlayground.clearPixels();
      boot = 0;
    }
  }
  level = 0;
  alive = 1;
  score = 0;
  slide = 1;
  i = 1;
  delay_1m.expire();
}




void loop() {
  //while slide is on for the game
  if (slide == 1) {
    //whilst the character is alive
    while (alive == 1) {
      //when the delay expires (that means when you have survived or started a level)
      //this is the code that sets up score and the next level's enemy and coin generation
      if (delay_1m.isExpired()) {
        score += (10 * level);
        level += 1;
        enemynum = random(10 * level, 30 * level);
        coins = random(5 * level, 10 * level);
        CircuitPlayground.clearPixels();
        enemypos = (-1);
        coinpos = (-1);
        loopthreshM = 1;
        loopthreshC = 1;
        delay_1m.repeat();
        loops = 0;
        CircuitPlayground.setPixelColor(0, 0, 128, 0);
        CircuitPlayground.setPixelColor(1, 0, 128, 0);
        CircuitPlayground.setPixelColor(2, 0, 128, 0);
        CircuitPlayground.setPixelColor(3, 0, 128, 0);
        CircuitPlayground.setPixelColor(4, 0, 128, 0);
        CircuitPlayground.setPixelColor(5, 0, 128, 0);
        CircuitPlayground.setPixelColor(6, 0, 128, 0);
        CircuitPlayground.setPixelColor(7, 0, 128, 0);
        CircuitPlayground.setPixelColor(8, 0, 128, 0);
        CircuitPlayground.setPixelColor(9, 0, 128, 0);
        delay(1000);
        CircuitPlayground.clearPixels();
      }
      
      //update player location
      
      CircuitPlayground.setPixelColor(i, 0, 0, 0);
      i += 1;

      //controls and movement
      if (CircuitPlayground.motionX() < .10) {
        i -= 2;
      }
      if (i > 9) {
        i = 0;
      }
      if (i < 0) {
        i = 9;
      }
      //player pixel will have a different hue based on time progressed(loops is the number of runs in the loop)
      CircuitPlayground.setPixelColor(i, 0, 128, 255 - loops);
      //enemy "hitbox"
      if (i == enemypos) {
        alive = 0;
        death = 1;
      }
      //coin "hitbox"  (i is the player position)
      if (i == coinpos) {
        score += 3;
        coinpos = (-1);
        CircuitPlayground.playTone(500, 50);
      CircuitPlayground.playTone(700, 50);
      }
      
    //below here is the spawning code for enemies and coins
      if (loops == (round(600 / enemynum + random(-9, 10)) * loopthreshM)){
        CircuitPlayground.setPixelColor(enemypos, 0, 0, 0);
      enemypos = random(0, 9);

      while (enemypos == coinpos || enemypos == i) {
        enemypos = random(0, 9);
      }
      loopthreshM += 1;

      }


      //more spawning
      if (loops == (round(600 / coins + random(-9, 10)) * loopthreshC))
      {  CircuitPlayground.setPixelColor(coinpos, 0, 0, 0);

      coinpos = random(0, 9);
      while (enemypos == coinpos || enemypos == i) {
        coinpos = random(0, 9);
      }
      loopthreshC += 1;
      }
      

     //once all loops of spawning and testing have occured, it will update the pixels to match the hitboxes it settled on
      CircuitPlayground.setPixelColor(coinpos, 128, 156, 0);
      CircuitPlayground.setPixelColor(enemypos, 255, 0, 0);

      //the check for on off switch and general delay between movements
      delay(250 - (level * 5));
      loops += 1;
      if (slide == 0) {
        break;
      }
    }

    //if you die, do ____
    if (death == 1) {
      CircuitPlayground.playTone(300, 1000);
      CircuitPlayground.playTone(250, 250);
      CircuitPlayground.playTone(350, 500);
       CircuitPlayground.playTone(400, 500);
    

      //resets for death
      Serial.print("score=");
      Serial.println(score);
      slide = 0;
      level = 0;
      alive = 1;
      score = 0;
      death = 0;
      loops = 0;
      delay_1m.expire();
      Serial.println("please flip switch to play");
    }
  }
}







 //changes bool from slide input
  void SlideS() {
    delay(20);
    slide = !slide;
  }