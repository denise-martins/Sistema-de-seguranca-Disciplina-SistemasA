Sistema de Segurança com Arduino (Senha, LEDs e Servo Motor)

Este projeto implementa um sistema de segurança simples usando Arduino UNO, botões, LEDs e um servo motor.
O usuário insere uma combinação de botões (senha). Se a senha estiver correta, um LED verde acende e o servo motor gira para liberar o acesso.

📦 Instalação
1. Baixar o código

Clone seu repositório:

git clone https://github.com/denise-martins/Sistema-de-seguranca-Disciplina-SistemasA.git


Ou baixe o ZIP pelo botão Code → Download ZIP no GitHub.

2. Instalar Arduino IDE

Baixe e instale a Arduino IDE:

🔗 https://www.arduino.cc/en/software

3. Instalar a biblioteca Servo

Na Arduino IDE:

Vá em Ferramentas → Gerenciar Bibliotecas

Pesquise por Servo

Clique em Instalar

4. Conectar o Arduino

Conecte o Arduino UNO via cabo USB

Na Arduino IDE selecione:

Ferramentas → Placa → Arduino UNO
Ferramentas → Porta → (selecione a porta COM disponível)

5. Enviar o código

Abra o arquivo .ino e clique em Upload (seta para a direita).

Pronto! Seu sistema estará funcionando.

🧭 Como Usar o Sistema

Pressione os botões na sequência configurada como senha.

A cada botão pressionado:

O LED vermelho acende (indicando entrada sendo digitada).

Após pressionar 4 botões:

Se a sequência estiver correta:

O LED verde acende

O servo motor gira para 180° (liberando o acesso)

Depois de 2 segundos, ele retorna para 0°

Se estiver errada:

Nada acontece, e o sistema reinicia a tentativa.

O sistema aceita infinitas novas tentativas.

🔐 Senha Configurada

A senha default é:

Botões nos pinos: 7 → 9 → 8 → 10

Convertida para índices internos:

{0, 2, 1, 3}


Se quiser alterar, basta mudar estes valores no código.

🔌 Ligações do Circuito
Botões

Pino 7

Pino 8

Pino 9

Pino 10

LEDs

LED vermelho → pino 11

LED verde → pino 12

Servo (SG90)

Sinal (laranja) → pino 3

VCC (vermelho) → 5V

GND (marrom) → GND

🧩 Código Completo
#include <Servo.h>

const int botoes[4] = {7, 8, 9, 10};
const int ledVermelho = 11;
const int ledVerde = 12;
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

  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);

  meuServo.attach(servoPin);
  meuServo.write(0);  // posição inicial
}

void loop() {

  // Verifica cada botão
  for (int i = 0; i < 4; i++) {
    if (digitalRead(botoes[i]) == HIGH) {

      digitalWrite(ledVerde, LOW); // verde apagado até confirmar
      digitalWrite(ledVermelho, HIGH);   // LED vermelho acende
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
          digitalWrite(ledVermelho, LOW);
          digitalWrite(ledVerde, HIGH); 

          meuServo.write(180);  
          delay(2000);          
          meuServo.write(0);    
        }

        // Reseta para nova entrada
        indice = 0;
      }
    }
  }

  // Se nenhum botão estiver sendo apertado, LED vermelho apaga
  bool algumApertado = false;
  for (int i = 0; i < 4; i++) {
    if (digitalRead(botoes[i]) == HIGH) {
      algumApertado = true;
    }
  }
  if (!algumApertado) {
    digitalWrite(ledVermelho, LOW);
  }
}
