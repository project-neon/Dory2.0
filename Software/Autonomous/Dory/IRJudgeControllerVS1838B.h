#include <IRremote.hpp>  // Version 4.2.0

// Inicializa com a programação off até testes de casos serem realizados
bool isRunning = false;

void IRJudgeControllerSensorInit() {
  IrReceiver.begin(JUDGE_CONTROLLER_PIN, ENABLE_LED_FEEDBACK); // Inicializa teste do led e sensor do IR
}

void checkSensorIR() {
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.command, HEX);

    switch (IrReceiver.decodedIRData.command) {
      case 0:
        Serial.println("Apertou o botão 1");
        break;
      case 0x1:
      case 0x11:
        Serial.println("Apertou o botão 2");
        isRunning = true;
        break;
      // Inicializa a estratégia simples
      case 0x2:
      case 0x12:
        Serial.println("Apertou o botão 3");
        Serial.println(" Robô Morreu ✖╭╮✖");
        while (1) {
          isRunning = false;
          velMotorL = 0;
          velMotorR = 0;
          sendPWMToMotors();
        }
        break;
      // Finaliza o loop
      case 0x3:
      case 0x13:
        Serial.println("Apertou o botão 4");
        isRunning = false;
        velMotorL = 0;
        velMotorR = 0;
        sendPWMToMotors();
        break;
    }

    IrReceiver.resume();
  }
}