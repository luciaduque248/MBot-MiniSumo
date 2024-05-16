// generado por mBlock5 para mBot
// los códigos te hacen feliz

#include <MeMCore.h> // Incluir la biblioteca para el robot mBot de Makeblock
#include <Arduino.h> // Incluir la biblioteca principal de Arduino
#include <Wire.h> // Incluir la biblioteca Wire para comunicación I2C
#include <SoftwareSerial.h> // Incluir la biblioteca SoftwareSerial para comunicación serial por software

MeLineFollower linefollower_2(3); // Inicializar un sensor seguidor de línea en el puerto 3
MeUltrasonicSensor ultrasonic_3(1); // Inicializar un sensor ultrasónico en el puerto 1
MeBuzzer buzzer; // Inicializar un zumbador
MeRGBLed rgbled_7(7, 2); // Inicializar un LED RGB en los puertos 7 y 2
MeDCMotor motor_9(9); // Inicializar un motor DC en el puerto 9
MeDCMotor motor_10(10); // Inicializar un motor DC en el puerto 10

void move(int direction, int speed) {
  int leftSpeed = 0;
  int rightSpeed = 0;
  if(direction == 1) { // Si la dirección es hacia adelante
    leftSpeed = speed;
    rightSpeed = speed;
  } else if(direction == 2) { // Si la dirección es hacia atrás
    leftSpeed = -speed;
    rightSpeed = -speed;
  } else if(direction == 3) { // Si la dirección es hacia la izquierda
    leftSpeed = -speed;
    rightSpeed = speed;
  } else if(direction == 4) { // Si la dirección es hacia la derecha
    leftSpeed = speed;
    rightSpeed = -speed;
  }
  motor_9.run((9) == M1 ? -(leftSpeed) : (leftSpeed));        // Hacer funcionar el motor izquierdo con la velocidad calculada
  motor_10.run((10) == M1 ? -(rightSpeed) : (rightSpeed));    // Hacer funcionar el motor derecho con la velocidad calculada
}

void _delay(float seconds) {
  long endTime = millis() + seconds * 1000;
  while(millis() < endTime) _loop();                          // Función de retraso que permite que se ejecuten otras tareas durante el retraso
}

void avanzarHastaDetectarLinea() {
  while (true) {
    int sensorValue = linefollower_2.readSensors();
    if (sensorValue == 0) { // Si los sensores detectan la línea (posiblemente, dependiendo de cómo estén configurados)
      break; // Salir del bucle si se detecta la línea
    }
    move(1, 80 / 100.0 * 255); // Mueve hacia adelante a velocidad máxima
    _loop();
  }
  // Detener el movimiento una vez que se detecta la línea
  motor_9.run(0);
  motor_10.run(0);
}


void setup() {
  Serial.begin(9600);               // Inicializar la comunicación serial a 9600 baudios

  pinMode(A7, INPUT);               // Configurar el pin A7 como entrada
  rgbled_7.fillPixelsBak(0, 2, 1);  // Inicializar el LED RGB con un patrón de color
  while(!(analogRead(A7) > 10)) {   // Esperar hasta que la entrada analógica A7 supere 10
    _loop();                        // Permitir que se ejecuten otras tareas durante el período de espera
  }
  
  while(analogRead(A7) > 10) {      // Esperar hasta que la entrada analógica A7 caiga por debajo de 10
    _loop();                        // Permitir que se ejecuten otras tareas durante el período de espera
  }

  // Añadir el nuevo while que espera 5 segundos y realiza las acciones específicas
  long startTime = millis();
  while(millis() - startTime < 5000) {       // Esperar 5 segundos
    // Tocar el pitido
    buzzer.tone(175, 0.25 * 1000);      // Reproducir una nota musical (G) durante 0,25 segundos
    _delay(0.75);                       // Esperar 0,75 segundos para que el pitido se repita cada segundo
    
    // Encender LED con color específico
    rgbled_7.setColor(0, 255, 0, 157);  // Establecer el color del LED RGB en el color especificado
    rgbled_7.show();                    // Mostrar el color
    _loop();                            // Permitir que se ejecuten otras tareas durante el período de espera
  }

  // Encender LED en azul para indicar que está buscando la distancia de la línea
  rgbled_7.setColor(0, 0, 21, 255); // Establecer el color del LED RGB en azul
  rgbled_7.show(); // Mostrar el color

  avanzarHastaDetectarLinea();
  
  while(1) {
    

    // LÍMITE DEL RING

    int distancia_ring_linea = linefollower_2.readSensors(); // Leer los sensores del seguidor de línea
    Serial.print("Distancia de la Línea: ");
    Serial.println(distancia_ring_linea);

    while(distancia_ring_linea != 0) { // Mientras se detecte la línea
      _loop(); // Permitir que se ejecuten otras tareas durante el bucle

      double distancia = ultrasonic_3.distanceCm(); // Medir la distancia utilizando el sensor ultrasónico
      Serial.print("Distancia del oponente: ");
      Serial.println(distancia);

      // OPONENTE
      if(distancia < 10) { // Si el oponente está a menos de 10 cm
        // TOCAR NOTA MUSICAL: MI
        buzzer.tone(165, 0.25 * 1000); // Reproducir una nota musical (E) durante 0,25 segundos
        _delay(0.02); // Retraso durante 0,02 segundos
        // ALUMBRAR ROJO
        rgbled_7.setColor(0, 255, 0, 0); // Establecer el color del LED RGB en rojo
        rgbled_7.show(); // Mostrar el color

        // Mover hacia adelante a toda velocidad
        move(1, 100 / 100.0 * 255);

      } else {
        // Si el oponente está a más de 10 cm
        motor_9.run(0);
        motor_10.run(0);
      }

      distancia_ring_linea = linefollower_2.readSensors(); // Leer nuevamente los sensores del seguidor de línea para detectar si se sacó al oponente
    }

    // TOCAR NOTA MUSICAL: DO
    buzzer.tone(65, 1 * 1000); // Reproducir una nota musical (C) durante 1 segundo
    _delay(0.02); // Retraso durante 0,02 segundos
    // ALUMBRAR VERDE
    rgbled_7.setColor(0, 140, 255, 0); // Establecer el color del LED RGB en verde
    rgbled_7.show(); // Mostrar el color

    // Girar a la izquierda para buscar el ring nuevamente
    move(3, 80 / 100.0 * 255);
    _delay(0.5);
    move(3, 80 / 100.0 * 255);
    _delay(0.2);
  }
}

void _loop() {
}

void loop() {
  _loop();
}
