#include <LiquidCrystal.h>

const int pauseBtnPin = 6;

const int rs = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

LiquidCrystal lcd(rs, enable, d4, d5, d6, d7);

unsigned long startTime;
unsigned long passedTime;
// const long focusTime = 1500000 // 25 minutes
const long focusTime = 5000;  // 5 seconds
const long breakTime = 4000;  // 4 seconds;

long remainingTime;

bool focusMode = true;

int currentPauseState = 0;
int prevPauseState = 0;
int timerRunning = 1;

unsigned long timerPausedAt;

void setup() {
  lcd.begin(16, 2);

  startTime = millis();

  Serial.begin(9600);

  pinMode(pauseBtnPin, INPUT);
}

void render(int minutes, int seconds, bool focusMode) {
  lcd.setCursor(0, 0);

  if (focusMode) {
    lcd.print("Focus Time");
  } else {
    lcd.print("Break Time");
  }

  lcd.setCursor(0, 1);
  lcd.print(minutes);
  lcd.print(":");

  if (seconds < 10) lcd.print("0");

  lcd.print(seconds);


  lcd.setCursor(5, 1);
  if (!timerRunning) {
    lcd.print("PAUSED");
  } else {
    lcd.print("      ");
  }
}

void toggleTimer(int currentPauseState) {
  Serial.println("===== Attempting to toggle timer =====");
  Serial.print("Timer running: ");
  Serial.print(timerRunning);
  Serial.print(" Pause state: ");
  Serial.println(currentPauseState);

  if (currentPauseState == 1 && prevPauseState == 0) {
    timerPausedAt = millis();
    timerRunning = !timerRunning;
  }
}

long getRemainingTime() {
  long remainingTime;

  if (focusMode) {
    remainingTime = focusTime - (millis() - startTime);
  } else {
    remainingTime = breakTime - (millis() - startTime);
  }

  return remainingTime;
}

void loop() {
  currentPauseState = digitalRead(pauseBtnPin);
  toggleTimer(currentPauseState);

  if (timerRunning) {
    startTime = startTime + (millis() - timerPausedAt);

    remainingTime = getRemainingTime();

    if (remainingTime <= 0) {
      remainingTime = 0;
      startTime = millis();
      focusMode = !focusMode;
    }
  }

  long totalSeconds = (remainingTime) / 1000;
  int seconds = totalSeconds % 60;
  int minutes = totalSeconds / 60;

  render(minutes, seconds, focusMode);

  prevPauseState = currentPauseState;
}


/**
  LCD pomodoro
  - Menu
    - startTime
    - configure
      - Set focus duration
      - Set break duration
        - maybe have some checks so that !(break > focus)
    stats
      - ???
    

    menu navigation:
    - potentiometer, rotate to go through menu items. push button to confirm menu time
    - push buttons. 1 push button to up a menu item and another to go down.

    timer screen
    - push button to pause
    - push button to reset timer
    - push button to exit to menu.


*/