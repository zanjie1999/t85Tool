// Tiny85 来电开机
// Sparkle
// v1.0

int sparklePwm = 0;
int sparkleLed = 1;
void setup() {
    pinMode(sparkleLed, OUTPUT);
    pinMode(sparklePwm, OUTPUT);
    digitalWrite(sparklePwm, LOW);
    digitalWrite(sparkleLed, HIGH);
    delay(1000);
    pinMode(sparklePwm, INPUT);
    digitalWrite(sparkleLed, LOW);
}

void loop() {
    digitalWrite(sparkleLed, !digitalRead(sparklePwm));
}
