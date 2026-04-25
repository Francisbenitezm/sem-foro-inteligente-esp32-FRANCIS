const byte pinSensor = 32;   //Botón 
const byte pinVerde = 21;    //LED verde PWM
const byte pinAmarillo = 22; //LED amarillo
const byte pinRojo = 23;     //LED rojo

hw_timer_t *timer = NULL;

volatile int contadorSegundos = 0;

int estado = 1; // 1 = verde, 2 = amarillo, 3 = rojo

void ARDUINO_ISR_ATTR onTimer() {
  contadorSegundos++;
}

void setup() {
  pinMode(pinSensor, INPUT_PULLUP);

  pinMode(pinVerde, OUTPUT);
  pinMode(pinAmarillo, OUTPUT);
  pinMode(pinRojo, OUTPUT);

  timer = timerBegin(1000000);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000000, true, 0);
}

void loop() {

  bool hayVehiculo = digitalRead(pinSensor) == LOW;

  int tiempoVerde;

  if (hayVehiculo == true) {
    tiempoVerde = 8;
  } else {
    tiempoVerde = 5;
  }

  if (estado == 1) {
    analogWrite(pinVerde, hayVehiculo ? 255 : 127);
    digitalWrite(pinAmarillo, LOW);
    digitalWrite(pinRojo, LOW);

    if (contadorSegundos >= tiempoVerde) {
      estado = 2;
      contadorSegundos = 0;
    }
  }

  else if (estado == 2) {
    analogWrite(pinVerde, 0);
    digitalWrite(pinAmarillo, HIGH);
    digitalWrite(pinRojo, LOW);

    if (contadorSegundos >= 2) {
      estado = 3;
      contadorSegundos = 0;
    }
  }

  else if (estado == 3) {
    analogWrite(pinVerde, 0);
    digitalWrite(pinAmarillo, LOW);
    digitalWrite(pinRojo, HIGH);

    if (contadorSegundos >= 5) {
      estado = 1;
      contadorSegundos = 0;
    }
  }
}
