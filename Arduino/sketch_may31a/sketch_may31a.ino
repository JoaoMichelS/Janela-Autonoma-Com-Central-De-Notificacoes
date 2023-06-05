#include <Stepper.h>   //biblioteca para controle de motor de passo

// --- Mapeamento de Hardware ---
#define   in1   8      //entrada 1 do ULN2003
#define   in2   9      //entrada 2 do ULN2003
#define   in3  10      //entrada 3 do ULN2003
#define   in4  11      //entrada 4 do ULN2003

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
Stepper myStepper(stepsPerRevolution, in1,in3,in2,in4); 

// Pinos Sensor chuva 
#define pinSensorA A0
#define pinSensorD 12
#define fimDeCursoPin 2
#define fimDeCursoPin2 3
#define DELAY_LEITURA 5000

bool fimDeCursoState;
unsigned long ultimaLeitura;

void setup() {
  pinMode(fimDeCursoPin, INPUT);
  pinMode(pinSensorD, INPUT);
  myStepper.setSpeed(90);

  Serial.begin(9600);
}

void loop() {
  fimDeCursoState = digitalRead(fimDeCursoPin);

  if(millis() - ultimaLeitura > DELAY_LEITURA){
    if(digitalRead(pinSensorD) == LOW){
      while(digitalRead(fimDeCursoPin) == LOW){
        Serial.println("5");
        myStepper.step(-stepsPerRevolution);
      }
    }//else{
      //Serial.println("8");
    //}
    ultimaLeitura = millis();
  }

  if(Serial.available()){
    String mensagem = Serial.readString();
    int mensagem_int = mensagem.toInt();
    //Serial.println(mensagem);
    //Serial.println(mensagem_int);
    if(mensagem_int == 1){
      while(digitalRead(fimDeCursoPin) == LOW){
        myStepper.step(-stepsPerRevolution);
      }
    }
    if(mensagem_int == 6){
      while(digitalRead(fimDeCursoPin2) == LOW){
        myStepper.step(stepsPerRevolution);
      }
    }
    if(mensagem_int == 4){
      if(digitalRead(pinSensorD) == HIGH){
        Serial.println("0");
      }
      if(digitalRead(pinSensorD) == LOW){
        Serial.println("1");
      }
    }
  }

}
