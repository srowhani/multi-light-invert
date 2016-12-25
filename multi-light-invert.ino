/**
 * @author - Naveed Edalati, Seena Rowhani
 * @description - sound sensor digital state toggle
 *  e.g to address different lights w/ clap seq.
 */
const byte SOUND_IN = 2; // define D0 Sensor Interface
const byte MAX_RELAYS = 3;
const byte RELAY_CURSOR = 3; // pin to start relay group from
const byte READ_DELAY = 25; // ms to prevent false digital reads

const int CB_DELAY = 1500; // ms before cb fires

int mReference = 0;
bool mStates[MAX_RELAYS];
bool isReady = true;
long t_start;

void setup ()
{
  Serial.begin(9600);

  pinMode (SOUND_IN, INPUT) ;// output interface D0 is defined sensor
  for (int i = 0 ; i < MAX_RELAYS; i++) {
    pinMode(RELAY_CURSOR + i, OUTPUT);    
    mStates[i] = false;
  }
  t_start = -1;
}

void loop ()
{
  int dr = digitalRead(SOUND_IN);
  if (dr == HIGH && isReady) {
    if (t_start == -1)
      t_start = millis();
    // Serial.println("dr == HIGH && isReady");
    mReference++;
    delay(READ_DELAY);
  }
  else {
    isReady = true;
  }
 
  if (millis() - t_start >= CB_DELAY && t_start != -1) {
    if(mReference > 0)
      timer_cb(min(MAX_RELAYS, mReference));
    t_start = -1;
    mReference = 0;
  }
}

void timer_cb (int r) {
  int index = r - 1;
  mStates[index] = !mStates[index]; //invert relay
  // Serial.print("PIN: ");
  // Serial.println(RELAY_CURSOR + index);
  digitalWrite(RELAY_CURSOR + index, mStates[index] ? HIGH : LOW);
}

