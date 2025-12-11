#include <Servo.h>

const int botoes[4] = {7, 8, 9, 10};
const int led = 11;          // LED que acende ao apertar um botão
const int ledSenhaOK = 12;   // LED que acende quando a senha está correta
const int alarme = 13;       // <<< ALARME PIEZO ATIVO
const int servoPin = 3;

Servo meuServo;

// Defina a senha (sequência de botões)
const int senha[4] = {0, 2, 1, 3};
int entrada[4];
int indice = 0;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(botoes[i], INPUT);
  }

  pinMode(led, OUTPUT);
  pinMode(ledSenhaOK, OUTPUT);
  pinMode(alarme, OUTPUT);     // <<< CONFIGURA ALARME COMO SAÍDA

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
          digitalWrite(ledSenhaOK, HIGH);

          meuServo.write(180);  
          delay(2000);
          meuServo.write(0);

          digitalWrite(ledSenhaOK, LOW);

        } else {
          // ---- SENHA INCORRETA -> ALARME ----
				tone(alarme, 1000);  // toca a 1000 Hz (som claro)
				delay(1000);
				noTone(alarme);      // desliga o buzzer
        }

        // Reseta para nova entrada
        indice = 0;
      }
    }
  }

  // Se nenhum botão está sendo pressionado, apaga o LED
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
