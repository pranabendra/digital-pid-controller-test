#include <Arduino.h>
#include <ArduinoSTL.h>
#include <queue>    
#include <PWM.h>

float y1, y;
std::queue<float> y2;
std::queue<float> y3;
std::queue<float> x;

float P, I, D;
float N = 100.0, Ts = 0.0002;

std::queue::y2.push(0);
y3.push(0);
x.push(0);

unsigned long startTime = micros();

void setup() {
    pinMode(A0, INPUT);
}

void loop() {
    if (startTime - micros() >= 200) {
        P = 2.0;
        I = 1.0;
        D = 0.01;

        x.push(analogRead(A0));

        y1 = P*x[1];
        y2.push(x[0]*I*Ts + y2[0]);
        y3.push(y3[0]*(1 - N*Ts) + D*N*(x[1] - x[0]));

        y = y1 + y2[1] + y3[2];

        startTime = micros();
    }
}