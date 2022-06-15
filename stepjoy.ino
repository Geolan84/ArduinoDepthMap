#include <Stepper.h>
#include <Servo.h>
#include <SharpIR.h>

#define IR A2 //ИК-датчик
#define model 1080 //Модель датчика в библиотеке.
#define PIN_TRIG_UP 5
#define PIN_ECHO_UP 4
#define PIN_TRIG_LEFT 3
#define PIN_ECHO_LEFT 2
#define PIN_TRIG_RIGHT 7
#define PIN_ECHO_RIGHT 6
long duration, cm;
long cm_UP, cm_LEFT, cm_RIGHT;

SharpIR SharpIR(IR, model);
Servo servo2;

#define Y A4
#define STEPS 32
#define IN1  35
#define IN2  34
#define IN3   32
#define IN4   33
#define joystick A3
#define swPin 8
Stepper stepper(STEPS, IN4, IN2, IN3, IN1);

int servoVal;
int positionSer = 80;
bool flag = true;
 
void setup()
{
  Serial.begin(9600);
  pinMode(swPin, INPUT);
  digitalWrite(swPin, HIGH);
  servo2.attach(9);
  pinMode(PIN_TRIG_UP, OUTPUT);
  pinMode(PIN_ECHO_UP, INPUT);
  pinMode(PIN_TRIG_LEFT, OUTPUT);
  pinMode(PIN_ECHO_LEFT, INPUT);
  pinMode(PIN_TRIG_RIGHT, OUTPUT);
  pinMode(PIN_ECHO_RIGHT, INPUT);
}
 
void loop()
{
  if(flag){
  Serial.println("");
  }
  
  if (digitalRead(swPin) == LOW)
  {
    Serial.println("click!");
    delay(500);
    if(flag==true)
    flag = false;
    else flag = true;
  }
  if(flag){
  //Блок управления сервоприводом(вверх-вниз).
  int val = analogRead(joystick);
  servoVal = analogRead(Y);
  
  while(servoVal > 510 && positionSer<=160)
    {
      servo2.write(++positionSer);
      delay(50);
      servoVal = analogRead(Y);
    }
    while(servoVal < 450 && positionSer>=0)
    {
      servo2.write(--positionSer);
      delay(50);
      servoVal = analogRead(Y);
    }
  
  //Блок управления шаговым двигателем(влево-вправо).

  //Если джойстик в среднем положении, то не двигаемся.
  if(  (val > 500) && (val < 523) )
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
 
  else
  {
    while (val >= 523)
    {
      int speed_ = map(val, 523, 1023, 5, 500);
      stepper.setSpeed(speed_);
      stepper.step(1);
      val = analogRead(joystick);
    }
    while (val <= 500)
    {
      int speed_ = map(val, 500, 0, 5, 500);
      stepper.setSpeed(speed_);
      stepper.step(-1);
      val = analogRead(joystick);
    }
  }
  }
  else
  {
  int dis=SharpIR.distance();

  digitalWrite(PIN_TRIG_UP, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG_UP, HIGH);
  // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG_UP, LOW);
  //  Время задержки акустического сигнала на эхолокаторе.
  duration = pulseIn(PIN_ECHO_UP, HIGH);
  // Теперь осталось преобразовать время в расстояние
  cm_UP = (duration / 2) / 29.1;
  
  delay(250);

  digitalWrite(PIN_TRIG_LEFT, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG_LEFT, HIGH);
  // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG_LEFT, LOW);
  //  Время задержки акустического сигнала на эхолокаторе.
  duration = pulseIn(PIN_ECHO_LEFT, HIGH);
  // Теперь осталось преобразовать время в расстояние
  cm_LEFT = (duration / 2) / 29.1;

  delay(250);

  digitalWrite(PIN_TRIG_RIGHT, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG_RIGHT, HIGH);
  // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG_RIGHT, LOW);
  //  Время задержки акустического сигнала на эхолокаторе.
  duration = pulseIn(PIN_ECHO_RIGHT, HIGH);
  // Теперь осталось преобразовать время в расстояние
  cm_RIGHT = (duration / 2) / 29.1;
  //Serial.print("Расстояние до объекта RIGHT: ");
  Serial.print(String(cm_RIGHT));
  Serial.print("  ");
  Serial.print(String(cm_UP));
  Serial.print("  ");
  Serial.print(String(cm_LEFT));
  Serial.print("  ");
  Serial.println(String(dis));  

  delay(250);

  }
}
