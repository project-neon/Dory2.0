// Lista com os pinos de entrada dos motores
const int motorsInputPins[4] = { MOTOR_RIGHT_IN1_PIN, MOTOR_RIGHT_IN2_PIN, MOTOR_LEFT_IN1_PIN, MOTOR_LEFT_IN2_PIN };

// Estabelecendo os canais PWM que serão utilizados
// Como os motores estão espelhados, para girar na mesma direção,
// precisamos fazer a ordem 1 0 2 3, ao invés de 0 1 2 3
// vinculando assim o IN1 com o canal IN2 e o IN2 com o canal IN1
const int chRightIN1 = 1;  // Canal PWM 1 que irá ser vinculado ao pino MOTOR_RIGHT_IN2_PIN da ponte H
const int chRightIN2 = 0;  // Canal PWM 0 que irá ser vinculado ao pino MOTOR_RIGHT_IN1_PIN da ponte H
const int chLeftIN1 = 2;   // Canal PWM 2 que irá ser vinculado ao pino MOTOR_LEFT_IN1_PIN da ponte H
const int chLeftIN2 = 3;   // Canal PWM 3 que irá ser vinculado ao pino MOTOR_LEFT_IN2_PIN da ponte H

// Configurando o PWM
const int freq = 50;                           // Frequência do PWM, definida em 50Hz
const int resolution = 12;                     // Resolução do PWM, 12 bits, de 0 a 4095.
const int totalBits = pow(2, resolution) - 1;  // Magnitude máxima que o motor recebe e interpreta como 100% da velocidade

// Variáveis responsáveis pela velocidade
double velMotorR = 0;        // Vel direita, vai de [-100, 100]
double velMotorL = 0;        // Vel esquerda, vai de [-100, 100]
double velMotorRMapped = 0;  // Vel direita, vai de [-totalBits, totalBits]
double velMotorLMapped = 0;  // Vel esquerda, vai de [-totalBits, totalBits]

void MotorsHBridgeDRV8833Init() {
  for (int i = 0; i < 4; i++) {
    pinMode(motorsInputPins[i], OUTPUT);   // Fazemos os pinos MOTOR_RIGHT_IN1_PIN, MOTOR_RIGHT_IN2_PIN, MOTOR_LEFT_IN1_PIN e MOTOR_LEFT_IN2_PIN serem OUTPUTs
    ledcSetup(i, freq, resolution);        // Atribui a todos os canais i (= 0, 1, 2, 3) a frequencia de 50Hz com resolucao de 12bits.
    ledcAttachPin(motorsInputPins[i], i);  // Associamos os pinos MOTOR_RIGHT_IN1_PIN, MOTOR_RIGHT_IN2_PIN, MOTOR_LEFT_IN1_PIN e MOTOR_LEFT_IN2_PIN aos canais i (= 0, 1, 2, 3) respectivamente.

    // Na placa modular usamos o pino 14, que inicia como HIGH,
    // A roda então ficaria rodando durante o setup()
    // Essa linha garante que a roda pare assim que possível
    ledcWrite(i, 0);
  }
}

void sendPWMToMotors() {
  // IN1 --> Horário, Positivo -------- IN2 --> Anti-horário, Negativo
  // A velocidade zero é um caso especial. Então em casos gerais:
  // Quando estamos indo do Horário pro Anti-Horário (IN1 --> IN2), o 0 deve ser do canal IN1
  // Quando estamos indo do Anti-Horário pro Horário (IN2 --> IN1), o 0 deve ser do canal IN2

  // Precisa passar pelo 0 sempre que muda de direção. O valor Mapped ainda tem guardado a velocidade do último loop nele.
  if (velMotorR > 0 != velMotorRMapped >= 0) ledcWrite((velMotorRMapped > 0) ? chRightIN1 : chRightIN2, 0);
  if (velMotorL > 0 != velMotorLMapped >= 0) ledcWrite((velMotorLMapped > 0) ? chLeftIN1 : chLeftIN2, 0);

  //Mapeia o valor da escala [-100, 100] para a escala do PWM
  velMotorRMapped = map(velMotorR, -100, 100, -totalBits, totalBits);
  velMotorLMapped = map(velMotorL, -100, 100, -totalBits, totalBits);

  //Envia para o motor o sinal PWM
  ledcWrite((velMotorR > 0) ? chRightIN1 : chRightIN2, abs(velMotorRMapped));
  ledcWrite((velMotorL > 0) ? chLeftIN1 : chLeftIN2, abs(velMotorLMapped));
}

void motorsPrintVel() {
  Serial.println("Esquerda: " + (String)velMotorL + "\tDireita: " + (String)velMotorR);
}

void printMotorsSpeed() {
  Serial.print("VelL: " + (String)velMotorL + "   VelR: " + (String)velMotorR);
  Serial.print("\t");
}

String printInternetMotorsSpeed() {
  return "  VelL: " + (String) velMotorL + "   VelR: " + (String) velMotorR;
}