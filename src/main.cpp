#include <Arduino.h>
#include <Wire.h>

#define ultr1_in
#define ultr1_out

#define ultr2_in
#define ultr2_out

#define ultr3_in
#define ultr3_out

#define ultr4_in
#define ultr4_out

#define adress 5
#define bit 11

#define P
#define I
#define D

byte mas[11];

int PID(int value)
{
}

void values_color()
{
  for (int i = 0; Wire.available(); ++i)
    mas[i] = Wire.read();
}

void setup()
{
  Wire.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
}