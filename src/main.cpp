#include <Arduino.h>
#include <Wire.h>

#define ultr1_in 11
#define ultr1_out 12
#define ultr2_in 5
#define ultr2_out 4
#define ultr3_in 10
#define ultr3_out 13
#define ultr4_in 6
#define ultr4_out 7

#define adress 5
#define bit 11

float P = 1.0;
float I = 1.0;
float D = 1.0;
long long error;
long long p_error;
long long guess;
long long Min;
long long Max;
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

void setup()
{
  Wire.begin();

  pinMode(ultr1_in, INPUT);
  pinMode(ultr2_in, INPUT);
  pinMode(ultr3_in, INPUT);
  pinMode(ultr4_in, INPUT);

  pinMode(ultr1_out, OUTPUT);
  pinMode(ultr2_out, OUTPUT);
  pinMode(ultr3_out, OUTPUT);
  pinMode(ultr4_out, OUTPUT);
}

void loop()
{
}