#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// --- Definição de Pinos ---
constexpr uint8_t PINO_FOSFORO = 14;
constexpr uint8_t PINO_POTASSIO = 4;
constexpr uint8_t PINO_LDR = 36;
constexpr uint8_t PINO_DHT = 5;
constexpr uint8_t PINO_RELE = 18;

// --- Sensor DHT ---
#define DHTTYPE DHT22
DHT dht(PINO_DHT, DHTTYPE);

// --- LCD I2C ---
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Endereço padrão I2C 0x27

// --- Variáveis Globais ---
bool fosforo = false;
bool potassio = false;
float umidade = 0.0;
float ph = 7.0;
bool bombaLigada = false;

void setup() {
  Serial.begin(115200);

  pinMode(PINO_FOSFORO, INPUT_PULLUP);
  pinMode(PINO_POTASSIO, INPUT_PULLUP);
  pinMode(PINO_RELE, OUTPUT);

  dht.begin();

  Wire.begin(21, 22);  // SDA=21, SCL=22
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");
  delay(1000);
}

// --- Função: Simula leitura de pH ---
float lerPhSimulado() {
  int leituraLDR = analogRead(PINO_LDR);
  return (leituraLDR / 4095.0f) * 14.0f;
}

// --- Função: Lê todos os sensores ---
void lerSensores() {
  fosforo = !digitalRead(PINO_FOSFORO);
  potassio = !digitalRead(PINO_POTASSIO);

  float leituraUmidade = dht.readHumidity();
  umidade = isnan(leituraUmidade) ? -1.0f : leituraUmidade;

  ph = lerPhSimulado();
}

// --- Função: Define se bomba será ativada ---
bool deveAtivarBomba() {
  bool nutriente = fosforo || potassio;
  bool phOk = (ph >= 5.5f && ph <= 7.5f);
  bool umidadeBaixa = (umidade > 0 && umidade < 40.0f);
  return nutriente && phOk && umidadeBaixa;
}

// --- Função: Liga/desliga bomba ---
void atualizarRele(bool ligar) {
  digitalWrite(PINO_RELE, ligar ? HIGH : LOW);
  bombaLigada = ligar;
}

// --- Função: Mostra dados no Serial + Plotter ---
void exibirSerial() {
  // Leitura humana
  Serial.print("[Sensores] Fósforo: ");
  Serial.print(fosforo);
  Serial.print(" | Potássio: ");
  Serial.print(potassio);
  Serial.print(" | pH: ");
  Serial.print(ph, 2);
  Serial.print(" | Umidade: ");
  Serial.print(umidade, 1);
  Serial.print("% | Bomba: ");
  Serial.println(bombaLigada ? "Ligada" : "Desligada");

  // Linha em branco para separar visualmente
  Serial.println();

  // Plotter
  Serial.print("Umidade: "); Serial.print(umidade, 2);
  Serial.print("\t");
  Serial.print("pH: "); Serial.print(ph, 2);
  Serial.print("\t");
  Serial.print("Bomba: "); Serial.println(bombaLigada ? 1 : 0);
}

// --- Função: Mostra dados no LCD ---
void exibirLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Umi:");
  lcd.print(umidade, 0);
  lcd.print("% pH:");
  lcd.print(ph, 1);

  lcd.setCursor(0, 1);
  lcd.print("Bomba: ");
  lcd.print(bombaLigada ? "Ligada" : "Deslig.");
}

// --- Loop principal ---
void loop() {
  lerSensores();
  bool ativar = deveAtivarBomba();
  atualizarRele(ativar);

  exibirSerial();
  exibirLCD();

  delay(2000);
}