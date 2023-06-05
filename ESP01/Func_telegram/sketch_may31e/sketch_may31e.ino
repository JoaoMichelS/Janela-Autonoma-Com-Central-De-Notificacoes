/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/telegram-control-esp32-esp8266-nodemcu-outputs/
  
  Project created using Brian Lough's Universal Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
  Example based on the Universal Arduino Telegram Bot Library: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/blob/master/examples/ESP8266/FlashLED/FlashLED.ino
*/
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "iPhone de Joao vitor";
const char* password = "12345678";

// Initialize Telegram BOT
#define BOTtoken "5607689150:AAEQY1NwxbPmn9BgMBzYucSb4YH8VRk6bWQ"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "908161889"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = 2;
bool ledState = LOW;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  //Serial.println("handleNewMessages");
  //Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    //Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use os comandos abaixo para controlar suas saídas.\n\n";
      welcome += "/fechar para fechar a janela \n";
      welcome += "/abrir para abrir a janela \n";
      welcome += "/status para verificar o status da janela \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/fechar") {
      bot.sendMessage(chat_id, "Janela fechando...", "");
      //ledState = HIGH;
      Serial.print("1\n");
      //digitalWrite(ledPin, ledState);
    }

    if (text == "/abrir") {
      bot.sendMessage(chat_id, "Janela abrindo...", "");
      //ledState = HIGH;
      Serial.print("6\n");
      //digitalWrite(ledPin, ledState);
    }
    
    /*if (text == "/led_off") {
      bot.sendMessage(chat_id, "LED state set to OFF", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    */

    if (text == "/status") {
      Serial.println("4");
      delay(4000);
      if(Serial.available()){
        String mensagem = Serial.readString();
        int status = mensagem.toInt();
        if(status == 1){
          bot.sendMessage(chat_id, "Está chovendo!", "");
        }
        if(status == 0){
          bot.sendMessage(chat_id, "Não está chovendo!", "");
        }
        else{
          bot.sendMessage(chat_id, "Erro na resposta via Serial!", "");
        }
      }else{
        bot.sendMessage(chat_id, "Erro na comunicação!");
      }
      /*if (Serial.available()){
        String mensagem = Serial.readString();
        int status = mensagem.toInt();
        if(status == 1){
          bot.sendMessage(chat_id, "Está chovendo!", "");
        }
        if(status == 0){
          bot.sendMessage(chat_id, "Não está chovendo!", "");
        }
        else{
        bot.sendMessage(chat_id, "LED is OFF", "");
        }
      }*/
    }
  }
}

void setup() {
  Serial.begin(9600);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  //pinMode(ledPin, OUTPUT);
  //digitalWrite(ledPin, ledState);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  //Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      //Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

  if(Serial.available()){
    String chat_id = "908161889";
    String mensagem = Serial.readString();
    int teste = mensagem.toInt();
    //Serial.print(mensagem);
    if(teste == 5){
      Serial.println("Alerta chuva!");
      bot.sendMessage(chat_id, "Está chovendo, estou fechando!");
    }
  }
} 
