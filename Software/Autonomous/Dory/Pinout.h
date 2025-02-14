//Board esp32 1.0.4
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif


// Motores da Esquerda e da direita
#define MOTOR_RIGHT_IN1_PIN 14 // Também é o pino da ESC1
#define MOTOR_RIGHT_IN2_PIN 27 // Também é o pino da ESC2
#define MOTOR_LEFT_IN1_PIN 26
#define MOTOR_LEFT_IN2_PIN 25

// Sensores de distância
#define DIST_SENSOR_SHARP_LEFT_PIN 34
#define DIST_SENSOR_SHARP_RIGHT_PIN 35

// SCL, SDA para a comunicação I2C
// Como usamos os pinos padrões da I2C, a biblioteca Wire.h cuida
// disso automaticamente para nós. Então essas variáveis não seriam necessárias.
#define SCL_PIN 22
#define SDA_PIN 21

// Define o pino XSHUT do sensor de distância VL53L0X
// Quando LOW, esse pino desliga o sensor e permite o endereçamento dos outros
int VL53LOX_XSHUT_LIST_PINS[3] = {17, 16, 4};

// Pino que retorna o valor recebido pelo controle do Juiz
#define JUDGE_CONTROLLER_PIN 39

// Pinos do de Interrupt do MPU e Motion do Mouse
#define INT_MPU 32
#define MOT_MOUSE 13

// Pino do divisor de tensao
#define DDT 33
