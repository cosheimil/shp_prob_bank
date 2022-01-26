#include <Arduino.h>
#include <Wire.h>

#define ultr1_in 11
#define ultr1_out 12
#define ultr2_in 10
#define ultr2_out 9
#define napr_a 4
#define napr_b 7
#define speed_a 5
#define speed_b 6
#define rast_do_banki 25
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
long duration, cm;
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
  long long i;
  i += error;
  if (i < Min)
    i = Min;
  if (i > Max)
    i = Max;

  long long res = p + i + d;
  if (res > Max)
    return Max;
  if (res < Min)
    return Min;
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
  if (value > Max)
    value = Max;
  if (value < Min)
    value = Min;

  digitalWrite(napr_b, value > 0);
  analogWrite(speed_b, value);
  digitalWrite(napr_a, value < 0);
  analogWrite(speed_b, value);
}

int dist1()
{
  digitalWrite(ultr1_out, LOW);
  delayMicroseconds(5);
  digitalWrite(ultr1_out, HIGH);

  // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
  delayMicroseconds(10);
  digitalWrite(ultr1_out, LOW);

  //  Время задержки акустического сигнала на эхолокаторе.
  duration = pulseIn(ultr1_in, HIGH);

  // Теперь осталось преобразовать время в расстояние
  cm = (duration / 2) / 29.1;
  return cm;
}

int dist2()
{
  digitalWrite(ultr2_out, LOW);
  delayMicroseconds(5);
  digitalWrite(ultr2_out, HIGH);

  // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
  delayMicroseconds(10);
  digitalWrite(ultr2_out, LOW);

  //  Время задержки акустического сигнала на эхолокаторе.
  duration = pulseIn(ultr2_in, HIGH);

  // Теперь осталось преобразовать время в расстояние
  cm = (duration / 2) / 29.1;
  return cm;
}

void setup()
{
  Wire.begin();

  pinMode(ultr1_in, INPUT);
  pinMode(ultr1_out, OUTPUT);
  pinMode(ultr2_in, INPUT);
  pinMode(ultr2_out, OUTPUT);

  pinMode(napr_a, OUTPUT);
  pinMode(napr_b, OUTPUT);
  pinMode(speed_a, OUTPUT);
  pinMode(speed_a, OUTPUT);
}

void loop()
{
  int banks_mas[4][2];
  while (1)
  {
    int s;
    motors(PID(values_color(), int((speed_a + speed_b) / 2)));
    if (mas[8] >= 220)
    {
      s++;
      if (s == 4)
        break;

      banks_mas[0][0] = int(dist1() > rast_do_banki);
      banks_mas[0][1] = int(dist2() > rast_do_banki);
      digitalWrite(napr_a, 1);
      digitalWrite(napr_b, 1);
      analogWrite(speed_b, 50);
      analogWrite(speed_b, 50);
      delay(100);
      digitalWrite(napr_a, 0);
      digitalWrite(napr_b, 0);
      analogWrite(speed_b, 0);
      analogWrite(speed_b, 0);
    }
  }

  while (1)
  {
    values_color();
    digitalWrite(napr_a, 0);
    digitalWrite(napr_b, 1);
    analogWrite(speed_a, 0);
    analogWrite(speed_b, 255);
    if (mas[4] >= 220)
      break;
  }
}