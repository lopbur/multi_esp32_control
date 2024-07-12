const int ADDRESS_OFFSET            = 4;
const int COMMAND_OFFSET            = 2;
const int POSITION_OFFSET           = 0;

const int addressMask               = 0b11110000;
const int commandMask               = 0b00001100;
const int positionMask              = 0b00000011;

const int CMD_BIND_ADDRESS          = 0b11 << COMMAND_OFFSET;
const int CMD_LED_SINGLE            = 0b10 << COMMAND_OFFSET;
const int CMD_LED_MULTI             = 0b01 << COMMAND_OFFSET;

const int MINIMUM_LED_POSITION      = 0;
const int MAXIMUM_LED_POSITION      = 3;

const int ledPins[] = {8, 9, 10};
int boardAddress = -1;

void show_channel_open(int id) {
    for (int i = 0; i < 5; i++) {
        digitalWrite(ledPins[id], HIGH);
        delay(200);
        digitalWrite(ledPins[id], LOW);
        delay(200);
    }
}

bool isLedPositionValid(int position) {
    if (position >= MINIMUM_LED_POSITION && position < MAXIMUM_LED_POSITION) {
      return true;
    }
    return false;
}

void setup() {
  for (int i = MINIMUM_LED_POSITION; i < MAXIMUM_LED_POSITION; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read() | (Serial.read() << 8);
    char address = command & addressMask;
    char commandType = command & commandMask;
    char position = command & positionMask;

    if (commandType == CMD_BIND_ADDRESS) {
      boardAddress = address >> ADDRESS_OFFSET;
      show_channel_open(boardAddress);
    } else {
      // Do nothing
    }
    
    if ((boardAddress == -1) | (address != (boardAddress << ADDRESS_OFFSET))) {
      return;
    } else {
      // Do nothing
    }

    if (!isLedPositionValid(position)) {
      return;
    } else {
      // Do nothing
    }

    if (commandType == CMD_LED_SINGLE) {
      digitalWrite(ledPins[position], !digitalRead(ledPins[position]));
    } else {
      // Do nothing
    }

    if (commandType == CMD_LED_MULTI) {
      for (int i = 0; i <= position; i++) {
        digitalWrite(ledPins[i], !digitalRead(ledPins[i]));
      }
    } else {
      // Do nothing
    }
  }
}