#include "app.h";

void App::begin(unsigned long now) {
  screen_ = Screen::MENU;
  selectedIndex_ = 0;

  timer_.begin(now);
  pauseBtn_.begin();
  resetBtn_.begin();
  selectBtn_.begin();
  menuBtn_.begin();
}

void App::update() {
  handleSelect();
}


void App::handleMenuInput(unsigned long now) {
  
}

void App::handleTimerInput(unsigned long now) {
}

void App::handleSelect() {
  if (menuBtn_.wasPressed(millis()) && screen_ != Screen::TIMER) {
    Serial.println("IN APP Navigating menu...");
    selectedIndex_ += 1;

    if (selectedIndex_ > 1) {
      selectedIndex_ = 0;
    }
  }
}