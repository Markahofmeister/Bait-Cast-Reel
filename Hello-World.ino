//Maya Roman

int buttonPin = 8;
int LEDPin = 9;

void setup() {

  pinMode(buttonPin, INPUT);
  pinMode(LEDPin, OUTPUT);

}

void loop() {

  if (digitalRead(buttonPin) == LOW) {
    digitalWrite(LEDPin, HIGH);
    delay(500);
    digitalWrite(LEDPin, LOW);
    delay(500);
  }

}
