#include <Servo.h>

const int botoes[4] = {7, 8, 9, 10};  // Botões da senha
const int botaoReset = 6;             // <<< BOTÃO PARA REDEFINIR SENHA

const int led = 11;          
const int ledSenhaOK = 12;  
const int alarme = 13;       
const int servoPin = 3;

Servo meuServo;

// SENHA INICIAL (pode ser alterada no modo reset)
int senha[4] = {0, 2, 1, 3};

// Entrada do usuário
int entrada[4];
int indice = 0;

bool modoReset = false;  // <<< indica se estamos cadastrando nova senha

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(botoes[i], INPUT);
  }

  pinMode(botaoReset, INPUT);

  pinMode(led, OUTPUT);
  pinMode(ledSenhaOK, OUTPUT);
  pinMode(alarme, OUTPUT);

  meuServo.attach(servoPin);
  meuServo.write(0);
}

void loop() {

  // -----------------------------
  // ENTRAR NO MODO DE REDEFINIÇÃO
  // -----------------------------
  if (digitalRead(botaoReset) == HIGH) {
    modoReset = true;
    indice = 0;

    digitalWrite(ledSenhaOK, HIGH);  // Indica início da redefinição
    delay(500);
    digitalWrite(ledSenhaOK, LOW);
    delay(500);
    digitalWrite(ledSenhaOK, HIGH);
    delay(500);
    digitalWrite(ledSenhaOK, LOW);

    // Aguarda soltar o botão reset
    while (digitalRead(botaoReset) == HIGH);

    return;  // evita que continue lendo botões no mesmo ciclo
  }

  // --------------------------------------------
  // LEITURA NORMAL DE BOTÕES (senha ou redefinição)
  // --------------------------------------------
  for (int i = 0; i < 4; i++) {

    if (digitalRead(botoes[i]) == HIGH) {

      digitalWrite(led, HIGH);

      entrada[indice] = i;
      indice++;

      delay(300);

      // Se 4 dígitos foram inseridos…
      if (indice == 4) {

        // ------------------------------------
        // MODO REDEFINIÇÃO DE SENHA
        // ------------------------------------
        if (modoReset) {

          // Salva nova senha
          for (int k = 0; k < 4; k++) {
            senha[k] = entrada[k];
          }

          // Feedback visual
          digitalWrite(ledSenhaOK, HIGH);
          delay(1000);
          digitalWrite(ledSenhaOK, LOW);

          modoReset = false;
          indice = 0;
          return;
        }

        // ------------------------------------
        // MODO NORMAL -> VERIFICAR SENHA
        // ------------------------------------
        bool correta = true;
        for (int j = 0; j < 4; j++) {
          if (entrada[j] != senha[j]) {
            correta = false;
            break;
          }
        }

        if (correta) {

          digitalWrite(ledSenhaOK, HIGH);
          meuServo.write(180);  
          delay(2000);
          meuServo.write(0);
          digitalWrite(ledSenhaOK, LOW);

        } else {

          tone(alarme, 1000);
          delay(1000);
          noTone(alarme);
        }

        indice = 0;
      }
    }
  }

  // Desliga LED quando nenhum botão está pressionado
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

