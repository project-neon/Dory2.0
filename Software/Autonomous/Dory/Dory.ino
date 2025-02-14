// Outros arquivos da dory
#include "Pinout.h"
#include "MotorsHBridgeDRV8833.h"
#include "DistanceSensors.h"
#include "IRJudgeControllerVS1838B.h"

// Declara as variáveis do robô
int enemyLastTimeSeenLeft = 0;  //enemyLastTimeSeenLeft 0: esquerda; enemyLastTimeSeenLeft 1: direita
int velMaxR = 100;
int velMaxL = 97.5;


// Criação da estratégia simples da Dory
void simpleStrategy() {

  if (distanceCenterLeft >= minimumDistanceSharp && distanceCenterRight >= minimumDistanceSharp) {  // Quando o inimigo é percebido à frente
    velMotorL = velMaxL;
    velMotorR = velMaxR;
    Serial.print("ATACANDO \t\t");
  } else if (distanceCenterLeft >= minimumDistanceSharp || distanceCenterRight >= minimumDistanceSharp) {  // Procurando o inimigo
    velMotorL = (distanceCenterLeft >= minimumDistanceSharp) ? velMaxR * 0.7 : velMaxL;                    // Achei na esquerda
    velMotorR = (distanceCenterLeft >= minimumDistanceSharp) ? velMaxR : velMaxL * 0.7;                    // Achei na direita
    enemyLastTimeSeenLeft = (distanceCenterLeft >= minimumDistanceSharp) ? 0 : 1;
    (distanceCenterLeft >= minimumDistanceSharp) ? Serial.print("ESQ \t\t") : Serial.print("DIR \t\t");
  } else {                                                                      // Quando perco o inimigo, duas opções dado a última leitura do sensor:
    velMotorL = (enemyLastTimeSeenLeft == 0) ? -velMaxR * 0.4 : velMaxL * 0.4;  // Gira pra esquerda ou
    velMotorR = (enemyLastTimeSeenLeft == 0) ? velMaxR * 0.4 : -velMaxL * 0.4;  // Gira pra direita
    (enemyLastTimeSeenLeft == 0) ? Serial.print("PROCURANDO ESQ \t\t") : Serial.print("PROCURANDO DIR \t\t");
  }
}

void setup() {
  // Inicia a Dory
  Serial.begin(115200);

  MotorsHBridgeDRV8833Init();
  Serial.println("Motores ON  ");

  IRJudgeControllerSensorInit();
  Serial.println("Controle Remoto ON ");

  distanceSensorsInit();
  Serial.println("Sensores Distancia ON  ");

  Serial.println("Setup Terminado!!!");
  delay(500);
}

void loop() {
  // Checa o sensor
  checkSensorIR();              // De acordo com o botão clicado, muda o mode
  readDistanceSensorsValues();  // Atualiza os valores da distância

  // Roda a programação de acordo com os casos do sensor de IR
  if (isRunning) {
    simpleStrategy();
  }

  sendPWMToMotors();  // Manda as velocidades para a ponte H

  distanceSensorsPrint();
  motorsPrintVel();
}
