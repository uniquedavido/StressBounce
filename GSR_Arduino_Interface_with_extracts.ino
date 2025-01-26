const int GSR_PIN = A0;
const float VCC = 5.0; // Supply voltage
const float ADC_RESOLUTION = 1024.0; // ADC resolution

void setup() {
  Serial.begin(9600);
}

void loop() {
  long sum = 0;
  int numReadings = 10;

  // Average 10 readings
  for (int i = 0; i < numReadings; i++) {
    int rawValue = analogRead(GSR_PIN);
    sum += rawValue;
    delay(5); // Small delay for ADC stability
  }

  int avgRawValue = sum / numReadings;

  // Calculate resistance from raw ADC value
  float resistance = ((ADC_RESOLUTION / (float)avgRawValue) - 1) * 10000; // Assuming a 10kOhm resistor

  // Calculate conductance in microsiemens
  float conductance = 1.0 / resistance * 1000000; // 1/S (S = siemens) * 1,000,000 to convert to microsiemens
  float standardizedConductance = conductance / 100;


  // Extract features
  static float readings[100]; // Store last 100 readings
  static int index = 0;
  readings[index] = conductance;
  index = (index + 1) % 100; // Circular buffer

  // Calculate mean and standard deviation
  float sumConductance = 0;
  float sumSquared = 0;
  for (int i = 0; i < 100; i++) {
    sumConductance += readings[i];
    sumSquared += readings[i] * readings[i];
  }

  float mean = sumConductance / 100;
  float standardizedMean = mean / 10; // To filter and remove noise
  float variance = (sumSquared / 100) - (mean * mean);
  float stdDev = sqrt(variance);
  float standardizedStdDev = stdDev/100; // To filter and remove noise

  // Print values to serial monitor
  Serial.print("Conductance (µS): ");
  //Serial.print(conductance);
  Serial.print(standardizedConductance);
  Serial.print("\tMean (µS): ");
  //Serial.print(mean);
  Serial.print(standardizedMean);
  Serial.print("\tStdDev (µS): ");
  //Serial.print(stdDev);
  Serial.print(standardizedStdDev);
  Serial.println();

  delay(1000); // Delay between readings
}
