#define SIGNAL_INVERTED 1

const int signalpin = 2;

float duty_cycle = 0.0;

volatile unsigned long prev_time_rising = 0;
volatile unsigned long current_time_rising = 0;
volatile unsigned long last_event_time = 0;

volatile unsigned long hill_duration = 0;
volatile unsigned long total_duration = 0;

void rising() {
  noInterrupts();
  last_event_time = micros();
  current_time_rising = last_event_time;
  total_duration = current_time_rising - prev_time_rising;
  prev_time_rising = current_time_rising;
  attachInterrupt(digitalPinToInterrupt(signalpin), falling, FALLING);
  interrupts();
}
 
void falling() {
  noInterrupts();
  last_event_time = micros();
  hill_duration = last_event_time - prev_time_rising;
  attachInterrupt(digitalPinToInterrupt(signalpin), rising, RISING);
  interrupts();
}
 
void setup() {
  Serial.begin(115200);
  pinMode(signalpin, INPUT_PULLUP);
  //We use function digitalPinToInterrupt for convenience. On arduino Uno, interrupt 0 is pin D2
  attachInterrupt(digitalPinToInterrupt(signalpin), rising, RISING);
}
 
void loop() {
  delay(100);

  noInterrupts();
  //For worst case on a 50hz signal 1/50*10^6 gives 20.000 micros
  if (micros() - last_event_time > 21000 || hill_duration < 1000 || total_duration < 1000){
    duty_cycle = (float) digitalRead(signalpin);
  } else {
    duty_cycle = (float) hill_duration / total_duration;
  }
  if(SIGNAL_INVERTED)
    duty_cycle = 1 - duty_cycle;
  interrupts();
  //Serial.print("Duty: ");
  Serial.println(duty_cycle*100, 0);
}
 
