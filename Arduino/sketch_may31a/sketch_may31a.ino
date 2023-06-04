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
    if(digitalRead(pinSensorD)){
      Serial.print("0\n");
    }else{
      Serial.print("1\n");
      if(fimDeCursoState == LOW){
        //Serial.print("5\n");
        myStepper.step(stepsPerRevolution);
      }
    }
    ultimaLeitura = millis();
  }

  if(Serial.available()){
    String mensagem = Serial.readString();
    int fec_janela = mensagem.toInt();
    if(fec_janela == 1){
      if(fimDeCursoState == LOW){
        myStepper.step(stepsPerRevolution);
      }
    }
  }

}
