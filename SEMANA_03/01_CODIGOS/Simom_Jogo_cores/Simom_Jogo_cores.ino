#include <Arduino.h>

const int buttonPins[] = {2, 4, 5, 16}; // Portas dos botões das cores
const int resetButtonPin = 32;          // Porta do botão de reset
const int ledPins[] = {27, 26, 25, 33}; // Portas dos LEDs das cores

const int numberOfPhases = 4;
const int sequenceLength = 4;

int phases[numberOfPhases][sequenceLength] = {
  {0, -1, -1, -1}, // Fase 1
  {0, 2, -1, -1}, // Fase 2
  {0, 2, 1, -1}, // Fase 3
  {0, 2, 1, 3}  // Fase 4
};

int currentPhase = 0;

void setup() {
  Serial.begin(115200);
  
  // Configura todos os botões como INPUT_PULLUP e LEDs como OUTPUT
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }

  pinMode(resetButtonPin, INPUT_PULLUP); // Botão de reset

  startGame();
}

void loop() {
  if (digitalRead(resetButtonPin) == LOW) { // Verifica se o botão de reset foi pressionado
    Serial.println("Reset acionado.");
    startGame();
    delay(500); // Debounce delay para evitar múltiplas leituras
    return; // Sai imediatamente do loop após o reset
  }

  playPhase(phases[currentPhase]); // Executa a fase atual
  
  int result = checkPlayerInput(phases[currentPhase]);
  if (result == 1) { // Se a entrada do jogador estiver correta
    currentPhase++; // Avança para a próxima fase
    if (currentPhase >= numberOfPhases) {
      Serial.println("Parabéns! Você completou todas as fases!");
      startGame(); // Reinicia o jogo
    }
  } else if (result == 0) { // Se o jogador errar
    Serial.println("Errado! Jogo será reiniciado.");
    startGame(); // Reinicia o jogo
  }
  // Não é necessário tratar -1 pois o reset foi tratado no início do loop
}

void startGame() {
  Serial.println("Jogo reiniciado. Siga a sequência!");
  currentPhase = 0;
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW); // Garante que todos os LEDs estão apagados ao iniciar
  }
  delay(1000); // Espera um pouco antes de começar
}

void playPhase(int phase[]) {
delay(1000); // Espera um pouco antes de mostrar a sequência
  for (int i = 0; i < sequenceLength; i++) {
    if (phase[i] != -1) { // Verifica se o valor no array não é -1 (fim da sequência)
      digitalWrite(ledPins[phase[i]], HIGH);
      delay(600);
      digitalWrite(ledPins[phase[i]], LOW);
      delay(400);
    }
  }
}

void blinkLed(int ledPin) {
  digitalWrite(ledPins[ledPin], HIGH); // Acende o LED
  delay(600); // Mantém aceso por um período
  digitalWrite(ledPins[ledPin], LOW); // Apaga o LED
  delay(400); // Espera antes de continuar
}


bool checkPlayerInput(int phase[]) {
  for (int i = 0; i < sequenceLength; i++) {
    if (phase[i] == -1) break; // Verifica se alcançou o fim da sequência

    int pressedButton = waitForButtonPress();
    if (pressedButton == -1) {
      // O botão de reset foi pressionado, reinicie o jogo
      return false;
    }
    if (pressedButton != phase[i]) {
      return false; // Se a entrada do jogador estiver errada
    }
    // Verifica apenas os botões necessários para a fase atual
    if ((currentPhase == 0 && i >= 0) || (currentPhase == 1 && i >= 1)) {
      break;
    }
  }
  return true; // Se a entrada do jogador estiver correta
}



int waitForButtonPress() {
  Serial.println("Esperando por pressionamento de botão...");
  int pressedButton = -1;

  // Loop até que um botão seja pressionado
  while (pressedButton == -1) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        pressedButton = i;
        // Acende o LED correspondente
        digitalWrite(ledPins[i], HIGH);
        delay(200); // Pequeno atraso para debouncing
        
        // Aguarda até que o botão seja solto
        while (digitalRead(buttonPins[i]) == LOW) {
          
        }

        digitalWrite(ledPins[i], LOW); // Apaga o LED
        delay(50); 
        break; // Sai do loop após um botão ser pressionado
      }
    }
    // Verifica se o botão de reset foi pressionado dentro da espera de um botão
    if (digitalRead(resetButtonPin) == LOW) {
      return -1; // Retorna -1 indicando que o jogo foi resetado
    }
  }
  return pressedButton; // Retorna o índice do botão que foi pressionado
}
