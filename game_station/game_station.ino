#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define W 128
#define H 64

Adafruit_SSD1306 display(W, H, &Wire, -1);

// joystick
const int joyX = A0;
const int joyY = A1;
const int joyBtn = 2;

// menu
const char* items[] = {"Snake", "Dodge", "Shooter"};
int menuIndex = 0;
int totalItems = 3;

// state
int state = 0;

bool lastBtn = HIGH;

// ================= SNAKE =================
#define MAX_LEN 30

int snakeX[MAX_LEN];
int snakeY[MAX_LEN];
int snakeLen = 3;

int dirX = 1;
int dirY = 0;

int foodX;
int foodY;

int score = 0;

unsigned long snakeStartTime = 0;
bool snakeStarted = false;

// ================= DODGE =================
int playerX = 60;
int blockX[3];
int blockY[3];
int dodgeScore = 0;

// ================= SHOOTER =================
int shipX = 60;
int bulletsX[5];
int bulletsY[5];
bool bulletActive[5];

int enemyX[3];
int enemyY[3];

int shooterScore = 0;
unsigned long lastShot = 0;

// ================= SETUP =================
void setup() {
  pinMode(joyBtn, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  randomSeed(analogRead(A3));

  resetSnake();
  resetDodge();
  resetShooter();
}

// ================= LOOP =================
void loop() {
  if (state == 0) menuLoop();
  else if (state == 1) snakeLoop();
  else if (state == 2) dodgeLoop();
  else if (state == 3) shooterLoop();
}

// ================= MENU =================
void menuLoop() {
  int y = analogRead(joyY);
  bool btn = digitalRead(joyBtn);

  if (y < 400) {
    menuIndex--;
    if (menuIndex < 0) menuIndex = totalItems - 1;
    delay(150);
  }

  if (y > 600) {
    menuIndex++;
    if (menuIndex >= totalItems) menuIndex = 0;
    delay(150);
  }

  if (lastBtn == HIGH && btn == LOW) {

    if (menuIndex == 0) {
      state = 1;
      resetSnake();
      snakeStartTime = millis();
      snakeStarted = false;
    }

    else if (menuIndex == 1) {
      state = 2;
      resetDodge();
    }

    else if (menuIndex == 2) {
      state = 3;
      resetShooter();
    }
  }

  lastBtn = btn;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(20, 0);
  display.println("GAME STATION");

  for (int i = 0; i < totalItems; i++) {
    if (i == menuIndex) display.print("> ");
    else display.print("  ");
    display.println(items[i]);
  }

  display.display();
}

// ======================================================
// 🐍 SNAKE (UNCHANGED)
// ======================================================
void resetSnake() {
  snakeLen = 3;
  score = 0;

  for (int i = 0; i < MAX_LEN; i++) {
    snakeX[i] = -1;
    snakeY[i] = -1;
  }

  snakeX[0] = 6;
  snakeY[0] = 4;

  snakeX[1] = 5;
  snakeY[1] = 4;

  snakeX[2] = 4;
  snakeY[2] = 4;

  dirX = 1;
  dirY = 0;

  spawnFood();
}

void spawnFood() {
  foodX = random(0, 16);
  foodY = random(0, 8);
}

void snakeLoop() {

  if (!snakeStarted) {
    snakeStarted = true;
    snakeStartTime = millis();
  }

  if (millis() - snakeStartTime < 400) {
    drawSnake();
    return;
  }

  readInput();
  moveSnake();

  if (checkCollision()) {
    gameOver();
    return;
  }

  drawSnake();
  delay(180);
}

void readInput() {
  int x = analogRead(joyX);
  int y = analogRead(joyY);

  if (x < 400 && dirX != 1) {
    dirX = -1; dirY = 0;
  }
  else if (x > 600 && dirX != -1) {
    dirX = 1; dirY = 0;
  }
  else if (y < 400 && dirY != 1) {
    dirX = 0; dirY = -1;
  }
  else if (y > 600 && dirY != -1) {
    dirX = 0; dirY = 1;
  }
}

void moveSnake() {
  for (int i = snakeLen - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  snakeX[0] += dirX;
  snakeY[0] += dirY;

  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    if (snakeLen < MAX_LEN) snakeLen++;
    score++;
    spawnFood();
  }
}

bool checkCollision() {
  if (snakeX[0] < 0 || snakeX[0] >= 16 ||
      snakeY[0] < 0 || snakeY[0] >= 8) return true;

  for (int i = 1; i < snakeLen; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) return true;
  }

  return false;
}

void drawSnake() {
  display.clearDisplay();

  display.fillRect(foodX * 8, foodY * 8, 8, 8, WHITE);

  for (int i = 0; i < snakeLen; i++) {
    display.fillRect(snakeX[i] * 8, snakeY[i] * 8, 8, 8, WHITE);
  }

  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("Score:");
  display.print(score);

  display.display();
}

void gameOver() {
  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(20, 20);
  display.println("GAME OVER");

  display.setCursor(20, 35);
  display.print("Score:");
  display.print(score);

  display.display();

  delay(2000);

  state = 0;
}

// ======================================================
// 🧱 DODGE (UNCHANGED)
// ======================================================
void resetDodge() {
  playerX = 60;
  dodgeScore = 0;

  for (int i = 0; i < 3; i++) {
    blockX[i] = random(0, 16) * 8;
    blockY[i] = random(-10, 0) * 8;
  }
}

void dodgeLoop() {

  int x = analogRead(joyX);

  if (x < 400) playerX -= 3;
  if (x > 600) playerX += 3;

  if (playerX < 0) playerX = 0;
  if (playerX > 120) playerX = 120;

  for (int i = 0; i < 3; i++) {

    blockY[i] += 3;

    if (blockY[i] > 64) {
      blockY[i] = random(-10, 0) * 8;
      blockX[i] = random(0, 16) * 8;
      dodgeScore++;
    }

    if (abs(blockX[i] - playerX) < 8 && blockY[i] > 52) {
      dodgeGameOver();
      return;
    }
  }

  display.clearDisplay();

  display.fillRect(playerX, 56, 8, 8, WHITE);

  for (int i = 0; i < 3; i++) {
    display.fillRect(blockX[i], blockY[i], 8, 8, WHITE);
  }

  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("Score:");
  display.print(dodgeScore);

  display.display();

  delay(50);
}

void dodgeGameOver() {
  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(20, 20);
  display.println("GAME OVER");

  display.setCursor(20, 35);
  display.print("Score:");
  display.print(dodgeScore);

  display.display();

  delay(2000);

  state = 0;
}

// ======================================================
// 🔫 SHOOTER (NEW FULL GAME)
// ======================================================
void resetShooter() {
  shipX = 60;
  shooterScore = 0;

  for (int i = 0; i < 5; i++) {
    bulletActive[i] = false;
  }

  for (int i = 0; i < 3; i++) {
    enemyX[i] = random(0, 16) * 8;
    enemyY[i] = random(-10, 0) * 8;
  }
}

void shooterLoop() {

  int x = analogRead(joyX);
  bool btn = digitalRead(joyBtn);

  if (x < 400) shipX -= 3;
  if (x > 600) shipX += 3;

  if (shipX < 0) shipX = 0;
  if (shipX > 120) shipX = 120;

  if (btn == LOW && millis() - lastShot > 250) {
    lastShot = millis();

    for (int i = 0; i < 5; i++) {
      if (!bulletActive[i]) {
        bulletActive[i] = true;
        bulletsX[i] = shipX + 4;
        bulletsY[i] = 50;
        break;
      }
    }
  }

  for (int i = 0; i < 5; i++) {
    if (bulletActive[i]) {
      bulletsY[i] -= 5;
      if (bulletsY[i] < 0) bulletActive[i] = false;
    }
  }

  for (int i = 0; i < 3; i++) {

    enemyY[i] += 2;

    if (enemyY[i] > 64) {
      enemyY[i] = random(-10, 0) * 8;
      enemyX[i] = random(0, 16) * 8;
    }

    if (abs(enemyX[i] - shipX) < 8 && enemyY[i] > 52) {
      shooterGameOver();
      return;
    }

    for (int b = 0; b < 5; b++) {
      if (bulletActive[b]) {
        if (abs(bulletsX[b] - enemyX[i]) < 8 &&
            abs(bulletsY[b] - enemyY[i]) < 8) {

          bulletActive[b] = false;
          enemyY[i] = random(-10, 0) * 8;
          enemyX[i] = random(0, 16) * 8;

          shooterScore++;
        }
      }
    }
  }

  display.clearDisplay();

  display.fillRect(shipX, 56, 8, 8, WHITE);

  for (int i = 0; i < 5; i++) {
    if (bulletActive[i]) {
      display.fillRect(bulletsX[i], bulletsY[i], 2, 4, WHITE);
    }
  }

  for (int i = 0; i < 3; i++) {
    display.fillRect(enemyX[i], enemyY[i], 8, 8, WHITE);
  }

  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("Score:");
  display.print(shooterScore);

  display.display();

  delay(40);
}

void shooterGameOver() {
  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(20, 20);
  display.println("GAME OVER");

  display.setCursor(20, 35);
  display.print("Score:");
  display.print(shooterScore);

  display.display();

  delay(2000);

  state = 0;
}
