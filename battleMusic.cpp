//Source for playNote and sith song. 
//https://learn.sparkfun.com/tutorials/mbed-starter-kit-experiment-guide/experiment-9-pwm-sounds

//We figured out the got notes and how to make the hit sound ourselves:
// We used http://en.wikipedia.org/wiki/Piano_key_frequencies to convert frequencies to music notes
// Based on the "speaker_demo_PWM" program by Jim Hamblen

#include "battleMusic.h"

PwmOut pwm_pin(D3);

// Plays a sound with the defined frequency, duration, and volume
void playNote(float frequency, float duration, float vol) {
    pwm_pin.period(1.0/frequency);
    pwm_pin = vol;
    wait(duration);
    pwm_pin = 0.0;
}
//Sheet Music Source: https://www.musicnotes.com/sheetmusic/mtd.asp?ppn=MN0017607
void sith()
{
    float beat_duration;

    // Calculate duration of a quarter note from bpm
    beat_duration = 60.0 / 100;

    // First measure
        playNote(NOTE_G4, (beat_duration - 0.1), VOLUME);//Quarter Note
        wait(0.1);
        playNote(NOTE_G4, (beat_duration - 0.1), VOLUME);
        wait(0.1);
        playNote(NOTE_G4, (beat_duration - 0.1), VOLUME);
        wait(0.1);
        playNote(NOTE_DS4, (0.75 * beat_duration), VOLUME);
        playNote(NOTE_AS4, (0.25 * beat_duration), VOLUME);

        // Second measure
        playNote(NOTE_G4, (beat_duration - 0.1), VOLUME);
        wait(0.1);
        playNote(NOTE_DS4, (0.75 * beat_duration), VOLUME);
        playNote(NOTE_AS4, (0.25 * beat_duration), VOLUME);
        playNote(NOTE_G4, ((2 * beat_duration) - 0.1), VOLUME); //Half Note
        wait(0.1);

        // Third measure
        playNote(NOTE_D5, (beat_duration - 0.1), VOLUME);
        wait(0.1);
        playNote(NOTE_D5, (beat_duration - 0.1), VOLUME);
        wait(0.1);
        playNote(NOTE_D5, (beat_duration - 0.1), VOLUME);
        wait(0.1);
        playNote(NOTE_DS5, (0.75 * beat_duration), VOLUME);
        playNote(NOTE_AS4, (0.25 * beat_duration), VOLUME);

        // Fourth measure
        playNote(NOTE_FS4, (beat_duration - 0.1), VOLUME);
        wait(0.1);
        playNote(NOTE_DS4, (0.75 * beat_duration), VOLUME);
        playNote(NOTE_AS4, (0.25 * beat_duration), VOLUME);
        playNote(NOTE_G4, ((2 * beat_duration) - 0.1), VOLUME);
        wait(0.1);
}
/**Creates a sound that resembles an enemy's reaction to being hurt**/
void hitNotes(){
    playNote(NOTE_C4, .05, 0.20);  
    playNote(NOTE_D4, .05, 0.20);
    playNote(NOTE_C4, .02, 0.10);
    playNote(NOTE_D4, .02, 0.10);
    playNote(NOTE_C4, .02, 0.05);
    playNote(NOTE_D4, .02, 0.05);
    playNote(NOTE_C4, .02, 0.02);
    playNote(NOTE_D4, .02, 0.02);
    playNote(NOTE_C4, .01, 0.02);
    playNote(NOTE_D4, .01, 0.02);
    playNote(NOTE_C4, .01, 0.02);
    playNote(NOTE_D4, .01, 0.02);
}
/**Plays first few measures of the Game of Thrones theme song when winning. 
Sheet music source: https://musescore.com/user/158751/scores/2163051**/
void got(){
    float beat_duration;
    
    // Calculate duration of a quarter note from bpm
    beat_duration = 60.0 / 168; //BPM of game of thrones theme song is 168
    //Measures 1-4
    for(int i = 0; i<4; i++){
         playNote(NOTE_G4, (beat_duration), VOLUME);//Quarter Note
        playNote(NOTE_C4, (beat_duration), VOLUME);//Quarter Note
        playNote(NOTE_DS4, (beat_duration*0.5), VOLUME);//Eigth Note
        playNote(NOTE_F4, (beat_duration*0.5), VOLUME);//Eigth Note
    }
    //Measures 5-8
    for(int i = 0; i<4; i++){
         playNote(NOTE_G4, (beat_duration), VOLUME);//Quarter Note
        playNote(NOTE_C4, (beat_duration), VOLUME);//Quarter Note
        playNote(NOTE_E4, (beat_duration*0.5), VOLUME);//8th Note
        playNote(NOTE_F4, (beat_duration*0.5), VOLUME);//8th Note
    }
    //Measures 9-11
    playNote(NOTE_G4, (beat_duration*2.5), VOLUME); 
    playNote(NOTE_C4, (beat_duration*2.5), VOLUME); 
    playNote(NOTE_DS4, (beat_duration*0.5), VOLUME);
    playNote(NOTE_F4, (beat_duration*0.5), VOLUME); 
    playNote(NOTE_G4, (beat_duration*2.0), VOLUME);
    
    //Measure 12
    playNote(NOTE_C4, (beat_duration*2.0), VOLUME); 
    playNote(NOTE_DS4, (beat_duration*0.5), VOLUME);
    playNote(NOTE_F4, (beat_duration*0.5), VOLUME); 
    
    //Measure 13-15
    for(int i = 0; i<3; i++)
    {
        playNote(NOTE_D4, (beat_duration), (VOLUME*1.5));
        playNote(NOTE_G3, (beat_duration), VOLUME);
        playNote(NOTE_AS3, (beat_duration*0.5), VOLUME);
        playNote(NOTE_C4, (beat_duration*0.5), VOLUME); 
    }
    //Measure 16
        playNote(NOTE_D4, (beat_duration), (VOLUME*1.5));
        playNote(NOTE_G3, (beat_duration-0.05), VOLUME);
        wait(0.05); 
        playNote(NOTE_AS3, (beat_duration-0.05), VOLUME);


    }
