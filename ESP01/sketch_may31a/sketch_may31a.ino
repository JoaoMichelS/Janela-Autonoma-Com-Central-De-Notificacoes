#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>



//-----------------------------------------------------------------------------------------------------------------------------------------
// IOT com ESP32 e WhatsApp
// QSP tecnologia
// www.qsptecnologia.com.br
// Professor: Daniel Quispe Marquez
// Microcontrolador: ESP32
// Versão: 1.0.0
// Observações:
//-----------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------
// Área de Inclusão de arquivos.

#include <Arduino.h>                                                    // Para usar a framework do Arduino
//#include <WiFi.h>                                                       // Para acesso ao wifi
//#include <HTTPClient.h>                                                 // Para criar um cliente HTTP
#include <UrlEncode.h>                                                  // Para enviar a mensagem via post deve ser no formato URL
#include <ESP8266HTTPClient.h>
//-----------------------------------------------------------------------------------------------------------------------------------------
// Definições de Hardware

//#define Sensor  12                                                      // Sensor digital ligado do GPIO 12
//#define Led1    15                                                      // Led1 ligado ao GPIO 15

//-----------------------------------------------------------------------------------------------------------------------------------------
// Protótipos de função

void WhatsAppMessages(String message);                                  // Função que envia uma mensagem para o whatsapp

//-----------------------------------------------------------------------------------------------------------------------------------------
// Dados da sua WiFi.

const char* SSID = "VIVOFIBRA-8128";                           // Nome da rede wifi. Identificador da rede
const char* PASSWORD = "mexBq9XpGs";                                // Senha da rede wifi. Sua senha do Wifi

//-----------------------------------------------------------------------------------------------------------------------------------------
// Dados do celular e chave do whatsapp

//bool flag = 0;                                                          // Flag para uso na logica do sensor
String phoneNumber = "+5515981521584";                                  // Meu número de celular com formato internacional
String apiKey = "9610035";                                               // A chave que o bot do whatsapp enviou

//-----------------------------------------------------------------------------------------------------------------------------------------

void setup() 
{
    //pinMode(Sensor, INPUT_PULLUP);                                      // Configura pino do sensor como entrada e em pull-up
    //pinMode(Led1, OUTPUT);                                              // Configura pino do led1 como saida
    Serial.begin(9600);                                               // Inicia a serial com 115200 bps
    WiFi.begin(SSID, PASSWORD);                                         // Inicia o wifi com o nome da rede e a senha
    Serial.println("Conectando");                                       // Envia pela serial que esta conectando

    while(WiFi.status() != WL_CONNECTED)                                // Aguarda a conexão
    {
        delay(500);                                                     // Espera 500ms
        Serial.print(".");                                              // Vai escrevendo ...
    }
    Serial.println("");                                                 // Pula uma linha
    Serial.print("Conectado no IP: ");                                  // Mostra que esta conectado no IP
    Serial.println(WiFi.localIP());                                     // Mostra o IP no qual foi conectado

    WhatsAppMessages("Testando 123 WhatsApp Bot !");                    // Envia mensagem para o bot do whatsapp
}

//-----------------------------------------------------------------------------------------------------------------------------------------

void loop() 
{
    //int Porta = digitalRead(Sensor);                                    // Faz a leitura do sensor

    //if (Porta == 1)                                                     // Porta acionada?
    //{
    //    if (flag)                                                       // Primeira vez acionado?
    //    {
    //        Serial.println("ALARME ACIONADO !");                        // Sim, então escreve na serial que foi acionado
    //        WhatsAppMessages("ALARME ACIONADO !");                      // Envia a mensagem para o Whatsapp que foi acionado
    //        flag = 0;                                                   // Zera o flag
    //    }
    //} 
    //else if(flag == 0)
    //{
    //    Serial.println("");                                                 // Pula uma linha
    //    Serial.println("TUDO NORMAL !");                                // Sim, então escreve na serial que esta tudo normal
    //    flag = 1;                                                       // Não esta acionada, então reinicia o flag
    //}
    //delay(250);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
// Definição das funções

void WhatsAppMessages(String message)
{
    // Dados a serem enviados pelo método Post suportado pelo protocolo HTTP
    String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
    
    HTTPClient http;                                                            // Cria um objeto HTTP
    //http.begin(url);                                                            // Inicia o objeto HTTP passando a string acima
    http.begin(url.c_str());
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");        // Adiciona um cabeçalho
  
    uint16_t httpResponseCode = http.POST(url);                                 // Envia o método POST HTTP de requisição e lê o resultado
  
    if (httpResponseCode == 200)                                                // Deu tudo certo?
    {
        //digitalWrite(Led1, HIGH);                                               // Liga o led1
        Serial.print("Mensagem enviada com sucesso!");                          // Envia mensagem na serial que deu certo
        delay(1000);                                                            // Espera 1s
        //digitalWrite(Led1, LOW);                                                // Desliga o led1
    }
    else                                                                        // Caso contrário, se não foi possivel enviar a mensagem
    {
        Serial.println("Erro ao tentar enviar a mensagem!");                    // Envia para a serial a mensagem de erro
        Serial.print("Código HTTP: ");                                          // Vai enviar para a serial o código de erro que retornou
        Serial.println(httpResponseCode);                                       // Envia o código de erro
    }
    http.end();                                                                 // Finaliza o objeto http
}

