#include <Arduino.h>
#include <Wire.h>

#define ultr_in 11
#define ultr_out 12

#define napr_a 4
#define napr_b 7
#define speed_a 5
#define speed_b 6

#define adress 5
#define bit 11

float P = 1.0;
float I = 1.0;
float D = 1.0;
long long error;
long long p_error;
long long guess = 0;
long long Min = -255;
long long Max = 255;
int speed;

byte mas1[11], mas[9];

int PID(int value, int speed)
{
  long long delta = value - guess;
  // P part
  long long p = delta * P;

  // D part
  error = delta;

  long long d = (error - p_error) * D;

  p_error = error;
  // I part
  long long i; i += error;
  if (i < Min) i = Min;
  if (i > Max) i = Max;

  long long res = p + i + d;
  if (res > Max) return Max;
  if (res < Min) return Min;
  return res; 
}

int values_color()
{
  for (int i = 0; Wire.available(); ++i)
    mas1[i] = Wire.read();
  
  for (int i = 0; i < 8; ++i)
    mas[i] = mas1[i];
  
  mas[8] = (mas1[8] + mas1[9] + mas1[10]) / 3;

  long long sum = 0;
  sum = (mas[0] * 1 + mas[1] * 256 + mas[2] * 512 + mas[3] * 768 + mas[4] * 1024 + mas[5] * 1280 + mas[6] * 1536 + mas[7] * 1792 + mas[8] * 2048) / (mas[0] + mas[1] + mas[2] + mas[3] + mas[4] + mas[5] + mas[6] + mas[7] + mas[8]);
  sum -= 1024;
  return sum;
}

int motors(int value)
{
  if (value > Max) value = Max;
  if (value < Min) value = Min;

  digitalWrite(napr_b, value > 0);
  analogWrite(speed_b, value);
  digitalWrite(napr_a, value > 0);
  analogWrite(speed_b, value);
}
void setup()
{
  Wire.begin();

  pinMode(ultr_in, INPUT);
  pinMode(ultr_out, OUTPUT);

  pinMode(napr_a, OUTPUT);
  pinMode(napr_b, OUTPUT);
  pinMode(speed_a, OUTPUT);
  pinMode(speed_a, OUTPUT);
}
void loop()
{
}