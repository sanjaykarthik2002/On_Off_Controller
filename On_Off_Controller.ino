const int trigPin = 9;
const int echoPin = 10;
const int inflowRelayPin = 8;
const int outflowRelayPin = 7;

long duration;
int distance;

const int tankHeight = 20; // tank height

int SP1 = 50; // Desired water level percentage

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(inflowRelayPin, OUTPUT);
  pinMode(outflowRelayPin, OUTPUT);

  digitalWrite(inflowRelayPin, LOW);
  digitalWrite(outflowRelayPin, LOW);

  Serial.println("Enter desired water level percentage (0-100): ");
}

void loop() {
  if (Serial.available() > 0) {
    SP1 = Serial.parseInt();
    
    if (SP1 < 0) {
      SP1 = 0;
    } else if (SP1 > 100) {
      SP1 = 100;
    }

    Serial.print("Desired water level percentage: ");
    Serial.print(SP1);
    Serial.println("%");
  }

  // Calculate the process variable (PV) distance based on the desired percentage
  int  SP = tankHeight * (100 - SP1) / 100;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in cm
  distance = duration * 0.034 / 2;
  PV=((tankHeight-distance)/tankHeight)*100;

  // Print the calculated PV to the Serial Monitor
  Serial.print("Water Level: ");
  Serial.println(PV);

  if (distance > SP) {
    digitalWrite(inflowRelayPin, HIGH);
    digitalWrite(outflowRelayPin, LOW);
    Serial.println("Inflow Pump ON, Outflow Pump OFF");
  } else if (distance < SP) {
    digitalWrite(inflowRelayPin, LOW);
    digitalWrite(outflowRelayPin, HIGH);
    Serial.println("Inflow Pump OFF, Outflow Pump ON");
  } else {
    digitalWrite(inflowRelayPin, LOW);
    digitalWrite(outflowRelayPin, LOW);
    Serial.println("Both Pumps OFF");
  }

  delay(1000);
}
