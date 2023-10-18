const int ledPin = 15;

// Durações para o código Morse
const int dotDuration = 200;        // Duração do dot (ponto)
const int dashDuration = dotDuration * 3; // Duração do dash (traço)
const int intraSymbolDuration = dotDuration; // Intervalo entre dots e dashes do mesmo caractere
const int interLetterDuration = dotDuration * 3; // Intervalo entre letras
const int interWordDuration = dotDuration * 7; // Intervalo entre palavras

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // S
  blink(dotDuration);
  delay(intraSymbolDuration);
  blink(dotDuration);
  delay(intraSymbolDuration);
  blink(dotDuration);

  delay(interLetterDuration);

  // O
  blink(dashDuration);
  delay(intraSymbolDuration);
  blink(dashDuration);
  delay(intraSymbolDuration);
  blink(dashDuration);

  delay(interLetterDuration);

  // S
  blink(dotDuration);
  delay(intraSymbolDuration);
  blink(dotDuration);
  delay(intraSymbolDuration);
  blink(dotDuration);

  delay(interWordDuration); // Pausa antes de repetir o SOS
}

void blink(int duration) {
  digitalWrite(ledPin, HIGH);
  delay(duration);
  digitalWrite(ledPin, LOW);
}