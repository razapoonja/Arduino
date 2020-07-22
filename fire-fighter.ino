#include <Servo.h>

Servo servo1, servo2;

int motor1pin1 = 3, motor1pin2 = 4, motor1pwm = 5;
int motor2pin1 = 8, motor2pin2 = 2, motor2pwm = 11;

int T = 13, E = 12, led = 0;

int duration, distance, temp, randomNumber;

int pump = 6;
int isFlamePin = 7, isFlame = HIGH;
int pos1 = 0, pos2 = 135;



void setup()
{
  configureItems(motor1pin1, motor1pin2, motor1pwm);
  configureItems(motor2pin1, motor2pin2, motor2pwm);

  pinMode(T, OUTPUT);
  pinMode(E, INPUT);

  pinMode(led, OUTPUT);

  configureFire();
  
  Serial.begin(9600);
}



void loop()
{
  int distance = calculateDistance();
  isFlame = digitalRead(isFlamePin);
  
  if(isFlame == LOW)
  {
    stopCar();
    operateFire();
    isFlame = digitalRead(isFlamePin);
  }
  
  else if (distance > 20 && isFlame == HIGH)
  {
    digitalWrite(pump, LOW);

    runServos();
    
    setSpeeds(140, 140);
    moveForward();
  }
  
  else if (distance < 20 && isFlame == HIGH)
  {
    digitalWrite(pump, LOW);
    avoidObstacle();
  }
}



// ------------------------------------ Car Functions --------------------------------------------

void configureItems(int motorpin1, int motorpin2, int motorpwm)
{
  pinMode(motorpin1, OUTPUT);
  pinMode(motorpin2, OUTPUT);
  pinMode(motorpwm, OUTPUT);
  digitalWrite(motorpwm, HIGH);
}


void forward(int motorpin1, int motorpin2)
{
  digitalWrite(motorpin1, HIGH);
  digitalWrite(motorpin2, LOW);
}


void reverse(int motorpin1, int motorpin2)
{
  digitalWrite(motorpin1, LOW);
  digitalWrite(motorpin2, HIGH);
}


void right()
{
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);

  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
}


void left()
{
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
}


int calculateDistance()
{
  digitalWrite(T, LOW);
  delayMicroseconds(2);
  digitalWrite(T, HIGH);
  delayMicroseconds(25);
  digitalWrite(T, LOW);
  duration = pulseIn(E, HIGH);
  distance = duration / (2 * 29.1);

  if (distance != 0)
  {
    temp = distance;
  }

  Serial.println(temp);
  delay(300);

  return temp;
}


void setSpeeds(int one, int two)
{
  analogWrite(motor1pwm, one);
  analogWrite(motor2pwm, two);
}


void avoidObstacle()
{
  moveBackward();
  delay(1000);

  setSpeeds(150, 150);

  randomNumber = random(0, 255);

  if (randomNumber < 123)
  {
    right();
  }
  else if (randomNumber > 123)
  {
    left();
  }

  delay(500);

  moveForward();
}


void moveForward()
{
  digitalWrite(led, HIGH);
  
  forward(motor1pin1, motor1pin2);
  forward(motor2pin1, motor2pin2); 
}

void moveBackward()
{
  digitalWrite(led, LOW);
  
  reverse(motor1pin1, motor1pin2);
  reverse(motor2pin1, motor2pin2);
}

void stopCar()
{
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
}




// ------------------------------------------------ Fire Functions ------------------------------------------------

void configureFire()
{
  pinMode(pump, OUTPUT);
  pinMode(isFlamePin, INPUT);

  servo1.attach(9);
  servo2.attach(10);
}

void operateFire()
{
  servo1.write(pos1);
  servo2.write(pos2 + 5);
  digitalWrite(pump, HIGH);
}

void runServos()
{
  operateUp();
  operateMiddle();
  operateDown();
}

void moveLeftRight()
{
  while (pos1 <= 180)
  {
    if (pos1%20 == 0)
    {
      int distance = calculateDistance();

      if (distance < 20)
      {
        avoidObstacle();
      }
    }

    isFlame = digitalRead(isFlamePin);

    if (isFlame == LOW)
    {
      break;
    }

    pos1 += 1;
    servo1.write(pos1);
    delay(15);
  }

  while (pos1 >= 0)
  {
    if (pos1%20 == 0)
    {
      int distance = calculateDistance();

      if (distance < 20)
      {
        avoidObstacle();
      }
    }

    isFlame = digitalRead(isFlamePin);

    if (isFlame == LOW)
    {
      break;
    }
    
    pos1 -= 1;
    servo1.write(pos1);
    delay(15);
  }
}


void operateUp()
{
  while (pos2 >= 100)
  {
    isFlame = digitalRead(isFlamePin);
  
    if (isFlame == LOW)
    {
      break;
    }

    pos2 -= 1;
    servo2.write(pos2);
    delay(15);
  }

  moveLeftRight();
}


void operateMiddle()
{
  setRange(122);
}


void operateDown()
{
  setRange(135);
}


void setRange(int range)
{
  while (pos2 <= range)
  {
    isFlame = digitalRead(isFlamePin);

    if (isFlame == LOW)
    {
      break;
    }
    
    pos2 += 1;
    servo2.write(pos2);
    delay(15);
  }

  moveLeftRight();
}
