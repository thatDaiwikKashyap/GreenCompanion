#include <Servo.h> 

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

Servo myservo;  // create servo object to control a servo

int moisture_Sensor;
unsigned long delay_time;
const int ms_powerpin = 8;                     // this is the port number for the VCC to be connected too
const unsigned long delay_watered = 43200000;            // a delay time of every 12 hours, this is after the plant has been watered.
const int threshold = 700;                     // defining the threshold for the wetness of the soil
const unsigned long delay_watering = 100;                // this delay is for when the plant is being watered. 

// change this to make the song slower or faster
int tempo_bday = 140;
int tempo_doom = 225;

// change this to whichever pin you want to use
int buzzer = 11;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody_bday[] = {

  // Happy Birthday
  // Score available at https://musescore.com/user/8221/scores/26906

  NOTE_C4,4, NOTE_C4,8, 
  NOTE_D4,-4, NOTE_C4,-4, NOTE_F4,-4,

 
};

const int melody_doom[] PROGMEM = {

  // At Doom's Gate (E1M1)
  // Score available at https://musescore.com/pieridot/doom

  NOTE_E2, 8, NOTE_E2, 8, NOTE_E3, 8, NOTE_E2, 8, NOTE_E2, 8, NOTE_D3, 8, NOTE_E2, 8, NOTE_E2, 8, //1
  NOTE_C3, 8, NOTE_E2, 8, NOTE_E2, 8, NOTE_AS2, 8, NOTE_E2, 8, NOTE_E2, 8, NOTE_B2, 8, NOTE_C3, 8,


};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes_bday = sizeof(melody_bday) / sizeof(melody_bday[0]) / 2;
int notes_doom = sizeof(melody_doom) / sizeof(melody_doom[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote_bday = (60000 * 4) / tempo_bday;
int wholenote_doom= (60000 * 4) / tempo_doom;


int divider = 0, noteDuration = 0;

int pos = 0;    // variable to store the servo position

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ms_powerpin, OUTPUT);

  myservo.attach(3);   // Set PWM pin 3 for Servo motor 1
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ms_powerpin, HIGH);              // turn the sensor on
  delay(10);                                    // a short delay
  moisture_Sensor = analogRead(A0);             // get the reading from the sensor
  digitalWrite(ms_powerpin, LOW);               // turn the sensor off
  Serial.println(moisture_Sensor);          

  // if the soil is wet
  if (moisture_Sensor > threshold){
    // play rickroll
    for (int thisNote = 0; thisNote < notes_bday * 2; thisNote = thisNote + 2) {
      // calculates the duration of each note
      divider = melody_bday[thisNote + 1];
      if (divider > 0) {
        // regular note, just proceed
        noteDuration = (wholenote_bday) / divider;
      } else if (divider < 0) {
        // dotted notes are represented with negative durations!!
        noteDuration = (wholenote_bday) / abs(divider);
        noteDuration *= 1.5; // increases the duration in half for dotted notes
      }

      // we only play the note for 90% of the duration, leaving 10% as a pause
      tone(buzzer, melody_bday[thisNote], noteDuration * 0.9);

      // Wait for the specief duration before playing the next note.
      delay(noteDuration);

      // stop the waveform generation before the next note.
      noTone(buzzer);

      for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
                          // waits 15ms for the servo to reach the position
        }
      for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
                        // waits 15ms for the servo to reach the position
      }
    }


    // set the delay_time as delay_watered
    delay_time = delay_watered;                // since the soil is still wet, we can come back and check in 12 hours. 
  }

  else{
    // play doom music
    // iterate over the notes of the melody.
    // Remember, the array is twice the number of notes (notes + durations)
    for (int thisNote = 0; thisNote < notes_doom * 2; thisNote = thisNote + 2) {

      // calculates the duration of each note
      divider = pgm_read_word_near(melody_doom+thisNote + 1);
      if (divider > 0) {
        // regular note, just proceed
        noteDuration = (wholenote_doom) / divider;
      } else if (divider < 0) {
        // dotted notes are represented with negative durations!!
        noteDuration = (wholenote_doom) / abs(divider);
        noteDuration *= 1.5; // increases the duration in half for dotted notes
      }

      // we only play the note for 90% of the duration, leaving 10% as a pause
      tone(buzzer, pgm_read_word_near(melody_doom+thisNote), noteDuration * 0.9);

      for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        }
        for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
                        // waits 15ms for the servo to reach the position
      }

      // Wait for the specief duration before playing the next note.
      delay(noteDuration);

      // stop the waveform generation before the next note.
      noTone(buzzer);
      Serial.print(moisture_Sensor);
    }
    // set the delay time to be delay_watering
    delay_time = delay_watering;                // since the soil is still wet, we can come back and check in 12 hours
  }

  delay(delay_time);                            

}
