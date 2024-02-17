#include <Arduino.h>
#include "EscTelemetry.h"

EscTelemetry escTelemetry1(&Serial1);

void setup(){
    Serial1.begin(115200);
}

void loop(){
    escTelemetry1.updateEscTelemetry();
}