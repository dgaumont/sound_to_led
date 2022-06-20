#include <Adafruit_NeoPixel.h>

#define PIN      6
#define N_LEDS 16 
const int microphonePin = A0;
int microphoneDPin = 1;
int digitalState=0;
int sensorValue = 0;
int clear_counter = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

const int sampleWindow = 50; // 50 mS Sample window width (20Hz)
unsigned int sample;

void setup() {
  int i;
  //Serial.begin(9600);
  strip.begin();
  strip.clear();
  strip.fill(strip.Color(0, 255, 0));
  strip.show();
  //Serial.println("BOOTING...");
  // blink on board led
  pinMode(LED_BUILTIN, OUTPUT);
  for (i=0; i< 10; i++) {
    digitalWrite(LED_BUILTIN, HIGH);    
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);  
    delay(200);
  }
}


void loop()
{
    double volts = get_volts_from_micro();

    if ((volts > 0.6) && (volts < 0.8)) {
      show_leds(4);
      delay(50);
    }
    else if ((volts > 0.8) && (volts < 1.0)) {
      show_leds(8);
      delay(50);
    }
    else if ((volts > 1.0) && (volts < 1.6)) {
      show_leds(12);
      delay(50);
    }
    else if ((volts > 1.6)) {
      show_leds(N_LEDS);     
      delay(50);   
    }
    else {
      clear_leds();
    }
}

double get_volts_from_micro() {
    unsigned long startMillis = millis(); // Sample Window Start
    unsigned int peakToPeak = 0;   //
    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;

    // collect data for 50 mS
    while (millis() - startMillis < sampleWindow)
    {
        sample = analogRead(0);
        if (sample < 1024)  // Cast out spurious readings
        {
            if (sample > signalMax)
            {
                signalMax = sample;  // Save just the MAX levels
            }
            else if (sample < signalMin)
            {
                signalMin = sample;  // Save just the MIN levels
            }
        }
    }

    peakToPeak = signalMax - signalMin;
    // MAX – MIN = P-P Amplitude
    double volts = (peakToPeak * 5.0) / 1024;  // Convert to Volt
    
    //Serial.println(volts);
    return volts;
}

// clears all leds
void clear_leds() {
  int i;
  if (clear_counter > 3)
    return;
  for (i=0; i< N_LEDS; i++) {
    strip.setPixelColor(i , strip.Color(0, 0, 0));
  }
  strip.show();
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  clear_counter++;
}

// displays leds from to nb-1
void show_leds(int nb) {
  int i;
  for (i=0; i< nb; i++) {
    strip.setPixelColor(i , strip.Color(255, 0, 0));
  }
  for (i=nb; i< N_LEDS; i++) {
    strip.setPixelColor(i , strip.Color(0, 0, 0));
  }
  strip.show();
  digitalWrite(LED_BUILTIN, HIGH);
  clear_counter = 0;
}
