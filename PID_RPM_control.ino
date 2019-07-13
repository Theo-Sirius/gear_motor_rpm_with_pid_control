volatile unsigned long count;
float PWM;
float rpm;
float average0p25;
float setpoint;
float error;
float integral;
float derivative;
float previous_error;
float Kp;
float Ki;
float Kd;
float valpin = A0;
float valpin1 = A1;
float valpin2 = A2;
float valpin3 = A3;
float val;

void setup()
{
  //pinMode(11,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  
  //setpoint = 100;
  analogWrite(11, 0);
  //analogWrite(10, 0);

  Serial.begin(115200);

  attachInterrupt(0, counter, RISING);
  attachInterrupt(1, counter, RISING);

  Serial.print("rpm");
  Serial.print("\t");
  Serial.print("error");
  Serial.print("\t");
  Serial.print("setp");
  Serial.print("\t");
  Serial.print("Kp");
  Serial.print("\t");
  Serial.print("Ki");
  Serial.print("\t");
  Serial.print("Kd");
  Serial.print("\t");

  //  Serial.print("error");
  //  Serial.print("\t");
  //  Serial.print("itg");
  //  Serial.print("\t");
  //  Serial.print("drv");
  //  Serial.print("\t");
  Serial.print("PWM");
  Serial.print("\t");
  Serial.println("ms from start");
}
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop()
{
  float value = analogRead(valpin);
  setpoint = mapfloat(value, 0, 1023, 0, 100);

  float value1 = analogRead(valpin1);
  Kp = mapfloat(value1, 0, 1023, 0, 50);

  float value2 = analogRead(valpin2);
  Ki = mapfloat(value2, 0, 1023, 0, 50);

  float value3 = analogRead(valpin3);
  Kd = mapfloat(value3, 0, 1023, 0, 50);

  count = 0;
  delay(50);
  rpm = count * 3.75; //(10*60/2)/80=3.75 //(10*60/2)/34 = 8.82
  average0p25 += (rpm - average0p25) * 0.25;

  error = setpoint - average0p25;
  integral += error;
  if (error == 0)
    integral = 0;
  if (integral >= 240)
    integral = 240;
  if (integral <= -240)
    integral = -240;
  //float Kp;  //=4.0;
  //float Ki;  //=0.1;
  //float Kd;  //=5.0;
  derivative = error - previous_error;
  previous_error = error;

  PWM = Kp * error + Ki * integral + Kd * derivative;
  if (PWM < 0)
    PWM = 0;
  if (PWM > 255)
    PWM = 255;

  analogWrite(11, PWM);
  digitalWrite(9,HIGH);
  digitalWrite(10,LOW);
  //analogWrite(10,0);

  // 0.94 0.1 0.49 80
  Serial.print(average0p25);
  Serial.print("\t");
  Serial.print(error);
  Serial.print("\t");
  //  Serial.print(integral);
  //  Serial.print("\t");
  //  Serial.print(derivative);
  //  Serial.print("\t");

  Serial.print(setpoint);
  Serial.print("\t");
  Serial.print(Kp);
  Serial.print("\t");
  Serial.print(Ki);
  Serial.print("\t");
  Serial.print(Kd);
  Serial.print("\t");
  Serial.print(PWM);
  Serial.print("\t");

  Serial.println(millis());
}
void counter()
{
  count++;
}

