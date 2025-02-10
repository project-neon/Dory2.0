// ---------------------------------------------//
// --------VL53L0X CONSTANTS DECLARATION--------//
// ---------------------------------------------//


#include <VL53L0X.h>  // Sensores de distância, Pololu 1.3.1
#include <Wire.h>     // Auxiliar dos sensores

VL53L0X sensorL;  // Sensor da esquerda
VL53L0X sensorC;  // Sensor do centro
VL53L0X sensorR;  // Sensor da direita

VL53L0X* sensorsList[3] = { &sensorL, &sensorC, &sensorR };  // Lista de ponteiros com os sensores

uint8_t sensorsAddresses[3] = { 0x1, 0x2, 0x3 };  // Endereços pro I2C

int distL;  // Valor lido pelo sensor da esquerda
int distC;  // Valor lido pelo sensor da frente
int distR;  // Valor lido pelo sensor da direita

// ---------------------------------------------//
// -----SHARP 0A41SK0F CONSTANTS DECLARATION----//
// ---------------------------------------------//

#define DISTANCE_ARRAY_LENGHT 20  // Tamanho da lista utilizada para armazenar as medidas do Sharp

// Estabelece a leituras das distâncias SHARP
int distanceCenterLeftList[DISTANCE_ARRAY_LENGHT] = { 0 }; // Lista das distâncias lidas da esquerda
int distanceCenterRightList[DISTANCE_ARRAY_LENGHT] = { 0 }; // Lista das distâncias lidas da direita
int distanceCenterLeft = 0; 
int distanceCenterRight = 0;
int minimumDistanceSharp = 50;


// ---------------------------------------------//
// --------- SHARP 0A41SK0F FUNCTIONS ----------//
// ---------------------------------------------//


void distanceSensors0A41SK0FInit() {
  pinMode(DIST_SENSOR_SHARP_LEFT_PIN, INPUT);
  pinMode(DIST_SENSOR_SHARP_RIGHT_PIN, INPUT);
}

// Leitura da lista de distância
int returnAverageValue(int list[DISTANCE_ARRAY_LENGHT]) {
  int sum = 0;
  for (int i = 0; i < DISTANCE_ARRAY_LENGHT; i++) sum += list[i];
  return sum / DISTANCE_ARRAY_LENGHT; // Média da lista de distância
}

// Atualiza as distâncias reconhecidas pelo sensor
void updateAvgDistanceLists(int centerLeftAnalogRead, int centerRightAnalogRead) {
  for (int i = 0; i < (DISTANCE_ARRAY_LENGHT - 1); i++) distanceCenterLeftList[i] = distanceCenterLeftList[i + 1];
  distanceCenterLeftList[DISTANCE_ARRAY_LENGHT - 1] = centerLeftAnalogRead; // Atualiza com o dado mais novo lido e retira a leitura mais antiga (sensor da esquerda)
  for (int i = 0; i < (DISTANCE_ARRAY_LENGHT - 1); i++) distanceCenterRightList[i] = distanceCenterRightList[i + 1];
  distanceCenterRightList[DISTANCE_ARRAY_LENGHT - 1] = centerRightAnalogRead; // Atualiza com o dado mais novo lido e retira a leitura mais antiga (sensor da direita)
}

void readDistanceSensors0A41SK0FValues() {
  updateAvgDistanceLists(analogRead(DIST_SENSOR_SHARP_LEFT_PIN), analogRead(DIST_SENSOR_SHARP_RIGHT_PIN));
  distanceCenterLeft = returnAverageValue(distanceCenterLeftList);
  distanceCenterRight = returnAverageValue(distanceCenterRightList);
}


// ---------------------------------------------//
// ------------- VL53L0X FUNCTIONS -------------//
// ---------------------------------------------//


void distanceSensorVL53L0XInit() {

  Wire.begin();  // Inicia o I2C
  Wire.setClock(400000); //TODO: iniciar o Wire em uma funcao separada no setup do codigo

  // Desliga todos os sensores
  for (int i = 0; i < 3; i++) {
    pinMode(VL53LOX_XSHUT_LIST_PINS[i], OUTPUT);
    digitalWrite(VL53LOX_XSHUT_LIST_PINS[i], LOW);
  }

  delay(10);

  //Endereça todos os sensores
  for (int i = 0; i < 3; i++) {
    pinMode(VL53LOX_XSHUT_LIST_PINS[i], INPUT); // XSHUT volta pra HIGH
    sensorsList[i]->init(); // Inicia o sensor usando a função da lib
    sensorsList[i]->setAddress(sensorsAddresses[i]); // Endereça o sensor
    sensorsList[i]->setTimeout(100); // Tempo que pode ficar sem responder
    sensorsList[i]->startContinuous(); // Modo contínuo, ↓ Precisão ↑ Velocidade
    delay(10);
  }
}

void readDistanceSensorsVL53L0XValues() {
  // Armazena os valores lidos nos sensores
  distL = sensorL.readRangeContinuousMillimeters();
  //distC = sensorC.readRangeContinuousMillimeters(); //Atualmente, não usamos
  distR = sensorR.readRangeContinuousMillimeters();
}


// ---------------------------------------------//
// ------------- GENERAL FUNCTIONS -------------//
// ---------------------------------------------//


void distanceSensorsInit() {
  distanceSensors0A41SK0FInit();
  distanceSensorVL53L0XInit();
}

// Atualiza com as novas leituras e retorna a variável para ocorrer o que é esperado dado a programação
void readDistanceSensorsValues() {
  readDistanceSensors0A41SK0FValues();
  readDistanceSensorsVL53L0XValues();
}

void distanceSensorsPrint() {
  Serial.print(+ "L: " + (String)distL
            + "\tC-L: " + (String)distanceCenterLeft
            + "\tC-R: " + (String)distanceCenterRight
            + "\tR: " + (String)distR);
}