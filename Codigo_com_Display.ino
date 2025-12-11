#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int botoes[4] = {7, 8, 9, 10};
const int led = 11;
const int ledSenhaOK = 12;
const int alarme = 13;
const int servoPin = 3;
const int botaoReset = 6;

Servo meuServo;

int senha[4] = {0, 2, 1, 3};
int entrada[4];
int novaSenha[4];

int indice = 0;
bool modoNovaSenha = false;
int indiceNova = 0;

void setup() {
  for (int i = 0; i < 4; i++) pinMode(botoes[i], INPUT);

  pinMode(led, OUTPUT);
  pinMode(ledSenhaOK, OUTPUT);
  pinMode(alarme, OUTPUT);
  pinMode(botaoReset, INPUT);

  meuServo.attach(servoPin);
  meuServo.write(0);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema iniciado");
  delay(1500);
}

void mostrarEntrada(int n, bool novaSenhaFlag) {
  lcd.clear();
  if (novaSenhaFlag)
    lcd.print("Nova senha:");
  else
    lcd.print("Senha:");

  lcd.setCursor(0, 1);
  for (int i = 0; i < 4; i++) {
    if (i < n) lcd.print("* ");
    else lcd.print("_ ");
  }
}

void loop() {

  // verifica se o usuário quer definir nova senha
  if (digitalRead(botaoReset) == HIGH && !modoNovaSenha) {
    modoNovaSenha = true;
    indiceNova = 0;
    lcd.clear();
    lcd.print("Nova senha:");
    lcd.setCursor(0, 1);
    lcd.print("_ _ _ _");
    delay(300);
  }

  // captura botões
  for (int i = 0; i < 4; i++) {
    if (digitalRead(botoes[i]) == HIGH) {

      digitalWrite(led, HIGH);
      delay(200);

      // MODO DEFINIR NOVA SENHA
      if (modoNovaSenha) {

        novaSenha[indiceNova] = i;
        indiceNova++;

        mostrarEntrada(indiceNova, true);

        if (indiceNova == 4) {
          for (int k = 0; k < 4; k++) senha[k] = novaSenha[k];

          lcd.clear();
          lcd.print("Senha alterada");
          delay(1500);

          modoNovaSenha = false;
        }

      } else {
        // MODO NORMAL (verificar senha)
        entrada[indice] = i;
        indice++;

        mostrarEntrada(indice, false);

        if (indice == 4) {
          bool correta = true;
          for (int j = 0; j < 4; j++)
            if (entrada[j] != senha[j]) correta = false;

          if (correta) {
            lcd.clear();
            lcd.print("Senha OK");
            lcd.setCursor(0, 1);
            lcd.print("Porta aberta");

            digitalWrite(ledSenhaOK, HIGH);
            meuServo.write(180);
            delay(2000);
            meuServo.write(0);
            digitalWrite(ledSenhaOK, LOW);

          } else {
            lcd.clear();
            lcd.print("Senha incorreta");
            tone(alarme, 1000);
            delay(1000);
            noTone(alarme);
          }

          indice = 0;
          delay(800);
          lcd.clear();
        }
      }

      while (digitalRead(botoes[i]) == HIGH) {} // aguarda soltar
    }
  }

  // apaga LED indicador
  bool algum = false;
  for (int i = 0; i < 4; i++)
    if (digitalRead(botoes[i]) == HIGH) algum = true;

  if (!algum) digitalWrite(led, LOW);
}
