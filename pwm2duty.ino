const int signalpin = 2;

float duty_cycle = 0.0;

volatile int prev_time_rising = 0;
volatile int current_time_rising = 0;
volatile int last_event_time = 0;

volatile int hill_duration = 0;
volatile int total_duration = 0;

void rising() {
  noInterrupts();
  last_event_time = micros();
  current_time_rising = last_event_time;
  total_duration = current_time_rising - prev_time_rising;
  prev_time_rising = current_time_rising;
  interrupts();
}
 
void falling() {
  noInterrupts();
  last_event_time = micros();
  hill_duration = last_event_time - prev_time_rising;
  interrupts();
}
 
void setup() {
  Serial.begin(115200);
  //We use function digitalPinToInterrupt for convenience. On arduino Uno, interrupt 0 is pin D2
  attachInterrupt(digitalPinToInterrupt(signalpin), rising, RISING);
  attachInterrupt(digitalPinToInterrupt(signalpin), falling, FALLING);
}
 
void loop() {
  delay(500);

  noInterrupts();
  //For worst case on a 50hz signal 1/50*10^6 gives 20.000 micros
  if (micros() - last_event_time > 21000){
    duty_cycle = digitalRead(signalpin);
  } else {
    duty_cycle = hill_duration / total_duration;
  }
  interrupts();
  //Serial.print("Duty: ");
  Serial.println(duty_cycle*100, 0);

  
}
 
