README.md – Sistema de Acesso com Arduino
1. Visão Geral

Este projeto implementa um sistema de acesso eletrônico usando Arduino, botões, display LCD, LEDs, servo motor e buzzer.
O usuário deve digitar uma senha de 4 dígitos para destravar a porta (servo).
Também há avisos visuais e sonoros para senha correta e incorreta.

2. Funcionalidades

Entrada de senha por 4 botões físicos.

Senha padrão configurada no código: 1 – 3 – 2 – 4.

Display LCD 16x2 exibindo mensagens de status.

LED verde acende quando a senha está correta.

LED vermelho pisca quando a senha está incorreta.

Buzzer emite alerta de erro.

Servo motor abre ou fecha a porta simulada.

3. Acesso à Simulação

Você pode testar o projeto online pelo Tinkercad:

Simulação do Projeto:
https://www.tinkercad.com/things/8fZGBMzJDqT-neat-jofo/editel

Para usar:

Entrar no link.

Fazer login no Tinkercad.

Clicar em “Iniciar Simulação”.

4. Requisitos

Arduino Uno

4 botões

Display LCD 16x2 (modo 4 bits)

Potenciômetro para contraste

Servo motor

LEDs (vermelho e verde)

Buzzer

Jumpers e resistores

Protoboard

Se estiver usando apenas o Tinkercad, nada precisa ser instalado.

5. Instalação (caso use Arduino físico)

Instalar a Arduino IDE no computador.

Disponível em: https://www.arduino.cc/en/software

Abrir o arquivo principal do projeto:

projeto_sistema_acesso.ino


Conectar o Arduino via USB.

Selecionar:

Placa: Arduino Uno

Porta: correspondente ao seu dispositivo

Carregar o código usando Upload.

6. Como Usar

Ligue o sistema.

O display mostrará: "Digite a senha".

Pressione os botões na ordem da senha padrão:

1 → 3 → 2 → 4


Se estiver correta:

LED verde acende

Servo abre a porta

Display mostra "Acesso liberado"

Se estiver incorreta:

LED vermelho pisca

Buzzer toca

Display mostra "Senha incorreta"

Após alguns segundos, o sistema volta ao estado inicial.

7. Estrutura do Código

O código está dividido em:

Configuração de pinos

Leitura dos botões

Comparação da senha

Feedback visual e sonoro

Abertura e fechamento do servo

8. Repositório GitHub
https://github.com/denise-martins/Sistema-de-seguranca-Disciplina-SistemasA

9. Possíveis Expansões Futuras

Cadastro de novas senhas.

Modo administrador.

Registro de tentativas.

Comunicação Bluetooth ou Wi-Fi.

Sensor de presença para ativação automática.

Se quiser, posso gerar também LICENSE, código-fonte organizado, estrutura de pastas, ou transformar o README em um formato mais visual.

Dito por si:

código-fonte organizado

sim
Dito pelo ChatGPT:

Perfeito. A seguir apresento uma estrutura completa de pastas + arquivos organizados + código-fonte principal limpo e bem comentado, pronta para colocar no GitHub.

Caso queira, posso também gerar um zip, LICENSE, versão com comentários reduzidos, ou versão para Tinkercad apenas.

Estrutura Recomendada do Projeto
projeto-sistema-acesso/
│
├── README.md
├── LICENSE                  (opcional)
│
├── /docs
│   └── wiring_diagram.png   (diagrama de ligação, se quiser gerar depois)
│
├── /src
│   └── sistema_acesso.ino   (código-fonte principal do Arduino)
│
├── /extras
│   ├── melhorias_futuras.txt
│   └── notas_do_projeto.md
│
└── /tinkercad
    └── link_simulacao.txt

Arquivo: link_simulacao.txt
Simulação Tinkercad:
https://www.tinkercad.com/things/8fZGBMzJDqT-neat-jofo/editel

Arquivo: melhorias_futuras.txt (opcional)
- Implementar troca de senha via botões
- Adicionar sensor de presença (PIR)
- Adicionar módulo Bluetooth ou Wi-Fi
- Registrar tentativas em EEPROM
- Criar modo administrador
- Criar animações no LCD

Arquivo: notas_do_projeto.md (opcional)
Notas gerais coletadas durante o desenvolvimento:
- O buzzer deve tocar apenas em tentativas incorretas.
- Display lcd.print() tem limites de caracteres e quebra de linha manual.
- Servo retornará ao ângulo inicial após timeout configurado.
- LEDs estão conectados com resistores de 220 Ω.

Código-Fonte organizado (sistema_acesso.ino)

Código limpo, padronizado, comentado e pronto para upload à IDE Arduino.

#include <Servo.h>
#include <LiquidCrystal.h>

// ------------------------------
// Configurações de hardware
// ------------------------------

const int botoes[4] = {7, 8, 9, 10};   // Botões de entrada
const int ledVerde = 11;              // LED para acesso liberado
const int ledVermelho = 12;           // LED para senha incorreta
const int buzzer = 6;                 // Buzzer de alerta
const int servoPin = 3;               // Servo da porta

Servo motorPorta;

// LCD no modo de 4 bits (pinos RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(2, 4, 5, A0, A1, A2);

// Senha padrão (1-3-2-4)
int senhaCorreta[4] = {1, 3, 2, 4};
int senhaDigitada[4];
int posicao = 0;

// ------------------------------
// Configuração Inicial
// ------------------------------

void setup() {
  // Botões como entrada
  for (int i = 0; i < 4; i++) {
    pinMode(botoes[i], INPUT_PULLUP);
  }

  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);

  motorPorta.attach(servoPin);
  motorPorta.write(0);  // Porta fechada

  lcd.begin(16, 2);
  lcd.print("Digite a senha");
}

// ------------------------------
// Leitura dos botões
// ------------------------------

int lerBotaoPressionado() {
  for (int i = 0; i < 4; i++) {
    if (digitalRead(botoes[i]) == LOW) {
      delay(250);  // Debounce simples
      return i + 1; // Botões numerados de 1 a 4
    }
  }
  return 0; // Nada pressionado
}

// ------------------------------
// Comparar senha
// ------------------------------

bool senhaCorretaDigitada() {
  for (int i = 0; i < 4; i++) {
    if (senhaDigitada[i] != senhaCorreta[i]) {
      return false;
    }
  }
  return true;
}

// ------------------------------
// Feedback: senha correta
// ------------------------------

void acessoLiberado() {
  lcd.clear();
  lcd.print("Acesso liberado");
  digitalWrite(ledVerde, HIGH);

  motorPorta.write(90);   // Abre a porta
  delay(3000);

  motorPorta.write(0);    // Fecha novamente
  digitalWrite(ledVerde, LOW);

  lcd.clear();
  lcd.print("Digite a senha");
}

// ------------------------------
// Feedback: senha incorreta
// ------------------------------

void acessoNegado() {
  lcd.clear();
  lcd.print("Senha incorreta");

  // Pisca LED vermelho
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledVermelho, HIGH);
    tone(buzzer, 1000);
    delay(200);

    digitalWrite(ledVermelho, LOW);
    noTone(buzzer);
    delay(200);
  }

  lcd.clear();
  lcd.print("Digite a senha");
}

// ------------------------------
// Loop Principal
// ------------------------------

void loop() {
  int botao = lerBotaoPressionado();
  if (botao != 0) {
    senhaDigitada[posicao] = botao;
    posicao++;

    if (posicao >= 4) {
      posicao = 0;

      if (senhaCorretaDigitada()) {
        acessoLiberado();
      } else {
        acessoNegado();
      }
    }
  }
}
