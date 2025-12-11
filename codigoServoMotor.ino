#include <Servo.h>

const int botoes[4] = {7, 8, 9, 10};
const int led = 11;
const int servoPin = 3;

Servo meuServo;

// Defina a senha (sequência de botões)
// Exemplo: 0-2-1-3 (botões 7, 9, 8, 10)
const int senha[4] = {0, 2, 1, 3};
int entrada[4];
int indice = 0;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(botoes[i], INPUT);
  }

  pinMode(led, OUTPUT);
  meuServo.attach(servoPin);
  meuServo.write(0);  // posição inicial
}

void loop() {

  // Verifica cada botão
  for (int i = 0; i < 4; i++) {
    if (digitalRead(botoes[i]) == HIGH) {

      digitalWrite(led, HIGH);   // LED acende quando botão é apertado
      entrada[indice] = i;
      indice++;

      delay(300); // evita múltiplas leituras do mesmo clique

      // Se completou 4 digitações...
      if (indice == 4) {
        bool correta = true;
        
        for (int j = 0; j < 4; j++) {
          if (entrada[j] != senha[j]) {
            correta = false;
            break;
          }
        }

        if (correta) {
          // ---- SENHA CORRETA ----
          meuServo.write(180);  
          delay(2000);           // trava aberta por 2 segundos
          meuServo.write(0);     // volta ao normal
        }

        // Reseta para nova entrada
        indice = 0;
      }
    }
  }

  // Se nada apertado, apaga o LED
  bool algumApertado = false;
  for (int i = 0; i < 4; i++) {
    if (digitalRead(botoes[i]) == HIGH) {
      algumApertado = true;
    }
  }
  if (!algumApertado) {
    digitalWrite(led, LOW);
  }
}

