#include "arduino_secrets.h"

#include <DHT.h>
#include <LiquidCrystal_I2C.h> // Inclui a biblioteca do LCD I2C
#include <Wire.h> // Necessário para comunicação I2C

// Define o pino do sensor DHT e o tipo do DHT
#define DHTPIN 2
#define DHTTYPE DHT22

// Define o pino de controle do relÃ©
#define RELEPIN 13

// Define o endereÃ§o I2C do LCD e as dimensões (colunas, linhas)
LiquidCrystal_I2C lcd(0x27, 16, 2); // *** SUBSTITUA 0x27 PELO ENDEREÇO DO SEU DISPLAY SE NECESSÁRIO ***

// Inicializa o sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Define a temperatura limite para ligar o umidificador
float temperaturaLimite = 23.0;

void setup() {
  // Inicializa a comunicaÃ§Ã£o serial
  Serial.begin(9600);
  Serial.println("Controle do Umidificador por Temperatura");
  Serial.println("---------------------------------------");

  // Inicializa o display LCD I2C
  lcd.init();
  lcd.backlight();
  lcd.print("Umidificador");
  lcd.setCursor(0, 1);
  lcd.print("Inicializando...");

  // Inicializa o sensor DHT
  dht.begin();

  // Define o pino do relÃ© como saÃ­da
  pinMode(RELEPIN, OUTPUT);
  // Garante que o relé comece desligado (lógica invertida)
  digitalWrite(RELEPIN, HIGH);

  Serial.println("Umidificador DESLIGADO");
}

void loop() {
  // Aguarda um pouco antes de ler novamente
  delay(2000);

  // LÃª a temperatura (em Celsius)
  float temp = dht.readTemperature();

  // Limpa a primeira linha do LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp:        Â°C"); // Preenche com espaços para limpar a linha

  // Verifica se a leitura falhou
  if (isnan(temp)) {
    Serial.println("Falha ao ler a temperatura do DHT22!");
    lcd.setCursor(6, 0);
    lcd.print("Erro!");
    return;
  }

  Serial.print("Temperatura atual: ");
  Serial.print(temp);
  Serial.println(" Â°C");

  // Exibe a temperatura no LCD
  lcd.setCursor(6, 0);
  lcd.print(temp);

  // Verifica se a temperatura atingiu ou ultrapassou o limite
  lcd.setCursor(0, 1); // Move o cursor para a segunda linha
  if (temp >= temperaturaLimite) {
    // Liga o umidificador (ativa o relé - lógica invertida)
    digitalWrite(RELEPIN, LOW);
    Serial.println("Temperatura atingiu " + String(temperaturaLimite) + " Â°C ou mais.");
    Serial.println("Umidificador LIGADO");
    lcd.print("Umidificador: ON "); // Preenche com espaços para limpar a linha
  } else {
    // Desliga o umidificador (desativa o relé - lógica invertida)
    digitalWrite(RELEPIN, HIGH);
    Serial.println("Temperatura abaixo de " + String(temperaturaLimite) + " Â°C.");
    Serial.println("Umidificador DESLIGADO");
    lcd.print("Umidificador: OFF"); // Preenche com espaços para limpar a linha
  }

  Serial.println("---------------------------------------");
}
