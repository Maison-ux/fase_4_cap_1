#include "DHT.h"

// Pinos
#define PINO_FOSFORO 14    // Botão de fósforo (D14)
#define PINO_POTASSIO 4    // Botão de potássio (D4)
#define PINO_LDR 36        // A0 (entrada analógica)
#define PINO_DHT 5         // DHT22 no pino 5
#define PINO_RELE 18       // LED ou relé no pino 18

// Configuração do DHT
#define DHTTYPE DHT22
DHT dht(PINO_DHT, DHTTYPE);

void setup() {
  Serial.begin(115200);

  // Configura os pinos de entrada
  pinMode(PINO_FOSFORO, INPUT_PULLUP);
  pinMode(PINO_POTASSIO, INPUT_PULLUP);

  // Pino do relé (LED)
  pinMode(PINO_RELE, OUTPUT);

  // Inicializa o sensor DHT
  dht.begin();
}

float ler_ph_simulado() {
  int valor_analogico = analogRead(PINO_LDR);
  return (valor_analogico / 4095.0) * 14.0;
}

void loop() {
  // Leitura dos botões
  bool fosforo = !digitalRead(PINO_FOSFORO);  // Invertido por causa do PULLUP
  bool potassio = !digitalRead(PINO_POTASSIO);

  // Leitura da umidade
  float umidade = dht.readHumidity();
  if (isnan(umidade)) {
    umidade = -1;
  }

  // Leitura do "pH"
  float ph = ler_ph_simulado();

  // --- LÓGICA DE IRRIGAÇÃO ---
  bool nutriente_presente = fosforo || potassio;
  bool ph_adequado = (ph >= 5.5 && ph <= 7.5);
  bool umidade_baixa = (umidade > 0 && umidade < 40);

  bool ativar_bomba = nutriente_presente && ph_adequado && umidade_baixa;

  // Aciona ou desliga o relé (LED)
  digitalWrite(PINO_RELE, ativar_bomba ? HIGH : LOW);

  // Imprime os dados simulados
  Serial.print("Fósforo: "); Serial.print(fosforo);
  Serial.print(" | Potássio: "); Serial.print(potassio);
  Serial.print(" | pH: "); Serial.print(ph, 2);
  Serial.print(" | Umidade: "); Serial.print(umidade, 1);
  Serial.print("% | Bomba: "); Serial.println(ativar_bomba ? "Ligada" : "Desligada");

  delay(2000);
}
