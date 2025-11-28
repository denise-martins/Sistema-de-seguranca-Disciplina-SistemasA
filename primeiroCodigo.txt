const int botoes[4] = {7, 8, 9, 10};
const int led = 11;

void setup() {
  // Botões como entrada normal
  for (int i = 0; i < 4; i++) {
    pinMode(botoes[i], INPUT);
  }

  pinMode(led, OUTPUT);
}

void loop() {

  bool algumApertado = false;

  for (int i = 0; i < 4; i++) {
    if (digitalRead(botoes[i]) == HIGH) {  
      // HIGH = botão apertado (com pull-down)
      algumApertado = true;
    }
  }

  if (algumApertado) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
}
