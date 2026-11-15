
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <math.h>

// ==========================================
// MONITOR DE INDICADORES FISIOLOGICOS
// ESP32 + NTC + RGB + BUZZER + WIFI + MQTT
// ==========================================

// PINOS
#define PINO_NTC 34
#define LED_R 25
#define LED_G 26
#define LED_B 33
#define BUZZER 27

// ==========================================
// CONFIGURACAO DO SENSOR NTC
// ==========================================

const float BETA = 3950.0;
const float R_NOMINAL = 10000.0;

// ==========================================
// LIMITES DE TEMPERATURA
// ==========================================

const float LIMITE_MINIMO = 17.0;
const float LIMITE_ATENCAO = 28.0;
const float LIMITE_ALERTA = 31.0;

// ==========================================
// CONFIGURACAO WIFI
// ==========================================

const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_SENHA = "";

// ==========================================
// CONFIGURACAO MQTT
// ==========================================

const char* MQTT_SERVIDOR = "test.mosquitto.org";
const int MQTT_PORTA = 1883;

const char* MQTT_CLIENTE = "esp32-kayque-iot-2026";

const char* MQTT_TOPICO =
  "mackenzie/iot/kayque2026/temperatura";

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

// ==========================================
// CONTROLE DE TEMPO
// ==========================================

unsigned long ultimaLeitura = 0;
unsigned long ultimaTentativaWiFi = 0;
unsigned long ultimaTentativaMQTT = 0;

const unsigned long INTERVALO = 1000;
const unsigned long RECONEXAO = 5000;

// ==========================================
// LED RGB - ANODO COMUM
// ==========================================

void desligarLED() {
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, HIGH);
}

void ledVerde() {
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, HIGH);
}

void ledAmarelo() {
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, HIGH);
}

void ledVermelho() {
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, HIGH);
}

// ==========================================
// LEITURA DO SENSOR NTC
// ==========================================

float lerTemperatura() {

  int valorADC = analogRead(PINO_NTC);

  if (valorADC <= 0 || valorADC >= 4095) {
    return NAN;
  }

  // Calculo da resistencia do NTC
  // Resistor fixo entre VCC e OUT
  // NTC entre OUT e GND

  float resistencia =
    R_NOMINAL * valorADC / (4095.0 - valorADC);

  if (resistencia <= 0 || !isfinite(resistencia)) {
    return NAN;
  }

  // Conversao para Kelvin
  float temperaturaKelvin =
    1.0 / (
      (1.0 / 298.15) +
      (log(resistencia / R_NOMINAL) / BETA)
    );

  // Conversao para Celsius
  float temperaturaCelsius =
    temperaturaKelvin - 273.15;

  if (!isfinite(temperaturaCelsius)) {
    return NAN;
  }

  // Informacoes para diagnostico
  Serial.print("Valor ADC: ");
  Serial.println(valorADC);

  Serial.print("Resistencia NTC: ");
  Serial.print(resistencia);
  Serial.println(" ohms");

  return temperaturaCelsius;
}

// ==========================================
// CONEXAO WIFI
// ==========================================

void conectarWiFi() {

  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.println("Conectando ao Wi-Fi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_SENHA, 6);
}

// ==========================================
// CONEXAO MQTT
// ==========================================

void conectarMQTT() {

  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  if (mqtt.connected()) {
    return;
  }

  Serial.println("Conectando ao MQTT...");

  if (mqtt.connect(MQTT_CLIENTE)) {

    Serial.println("MQTT conectado!");

  } else {

    Serial.print("Falha MQTT. Codigo: ");
    Serial.println(mqtt.state());
  }
}

// ==========================================
// PUBLICACAO MQTT
// ==========================================

void publicarDados(float temperatura,
                   const char* status) {

  if (!mqtt.connected()) {
    Serial.println("MQTT indisponivel.");
    return;
  }

  char mensagem[160];

  if (isnan(temperatura)) {

    snprintf(
      mensagem,
      sizeof(mensagem),
      "{\"temperatura\":null,\"status\":\"ERRO_SENSOR\"}"
    );

  } else {

    snprintf(
      mensagem,
      sizeof(mensagem),
      "{\"temperatura\":%.2f,\"status\":\"%s\"}",
      temperatura,
      status
    );
  }

  bool publicado = mqtt.publish(
    MQTT_TOPICO,
    mensagem
  );

  if (publicado) {

    Serial.print("MQTT publicado: ");
    Serial.println(mensagem);

  } else {

    Serial.println("Falha na publicacao MQTT.");
  }
}

// ==========================================
// SETUP
// ==========================================

void setup() {

  Serial.begin(115200);

  // Configuracao do sensor
  pinMode(PINO_NTC, INPUT);

  analogReadResolution(12);

  analogSetPinAttenuation(
    PINO_NTC,
    ADC_11db
  );

  // Configuracao dos atuadores
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  desligarLED();
  noTone(BUZZER);

  Serial.println();
  Serial.println("==============================");
  Serial.println(" MONITOR FISIOLOGICO - IOT");
  Serial.println("==============================");

  mqtt.setServer(MQTT_SERVIDOR, MQTT_PORTA);

  conectarWiFi();

  ultimaTentativaWiFi = millis();
}

// ==========================================
// LOOP PRINCIPAL
// ==========================================

void loop() {

  unsigned long agora = millis();

  // ========================================
  // RECONEXAO WIFI
  // ========================================

  if (WiFi.status() != WL_CONNECTED) {

    if (agora - ultimaTentativaWiFi >= RECONEXAO) {

      ultimaTentativaWiFi = agora;

      conectarWiFi();
    }
  }

  // ========================================
  // RECONEXAO MQTT
  // ========================================

  if (WiFi.status() == WL_CONNECTED &&
      !mqtt.connected()) {

    if (agora - ultimaTentativaMQTT >= RECONEXAO) {

      ultimaTentativaMQTT = agora;

      conectarMQTT();
    }
  }

  if (mqtt.connected()) {
    mqtt.loop();
  }

  // ========================================
  // LEITURA A CADA SEGUNDO
  // ========================================

  if (agora - ultimaLeitura < INTERVALO) {
    return;
  }

  ultimaLeitura = agora;

  float temperatura = lerTemperatura();

  const char* status;

  Serial.println();
  Serial.println("------------------------------");

  // ========================================
  // ERRO DO SENSOR
  // ========================================

  if (isnan(temperatura)) {

    status = "ERRO_SENSOR";

    Serial.println("ERRO: leitura invalida!");
    Serial.println("Verifique o sensor NTC.");

    desligarLED();
    noTone(BUZZER);
  }

  // ========================================
  // ALERTA DE TEMPERATURA BAIXA
  // ABAIXO DE 17 GRAUS
  // ========================================

  else if (temperatura < LIMITE_MINIMO) {

    status = "ALERTA_BAIXA";

    Serial.print("Temperatura: ");
    Serial.print(temperatura, 2);
    Serial.println(" C");

    Serial.println("STATUS: ALERTA DE TEMPERATURA BAIXA!");

    ledVermelho();
    tone(BUZZER, 2000);
  }

  // ========================================
  // ESTADO NORMAL
  // DE 17 A ABAIXO DE 28 GRAUS
  // ========================================

  else if (temperatura < LIMITE_ATENCAO) {

    status = "NORMAL";

    Serial.print("Temperatura: ");
    Serial.print(temperatura, 2);
    Serial.println(" C");

    Serial.println("STATUS: NORMAL");

    ledVerde();
    noTone(BUZZER);
  }

  // ========================================
  // ESTADO DE ATENCAO
  // DE 28 A ABAIXO DE 31 GRAUS
  // ========================================

  else if (temperatura < LIMITE_ALERTA) {

    status = "ATENCAO";

    Serial.print("Temperatura: ");
    Serial.print(temperatura, 2);
    Serial.println(" C");

    Serial.println("STATUS: ATENCAO");

    ledAmarelo();
    noTone(BUZZER);
  }

  // ========================================
  // ALERTA DE TEMPERATURA ALTA
  // 31 GRAUS OU MAIS
  // ========================================

  else {

    status = "ALERTA_ALTA";

    Serial.print("Temperatura: ");
    Serial.print(temperatura, 2);
    Serial.println(" C");

    Serial.println("STATUS: ALERTA DE TEMPERATURA ALTA!");

    ledVermelho();
    tone(BUZZER, 2000);
  }

  // ========================================
  // PUBLICACAO DOS DADOS NO MQTT
  // ========================================

  publicarDados(temperatura, status);
}

