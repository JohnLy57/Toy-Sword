
#include "mbed.h"
#include "FXOS8700Q.h"
#include <string>
#include "FXAS21000.h"
#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif
#include "math.h"
#include "stdlib.h"
#include "pwm.h"
#include "battleMusic.h"

FXOS8700Q_acc combo_acc(D14, D15, FXOS8700CQ_SLAVE_ADDR0);
FXOS8700Q_mag combo_mag(D14, D15, FXOS8700CQ_SLAVE_ADDR0);
FXAS21000 gyro(D14, D15);

DigitalOut ledpin(LED1);
PwmOut blue2(D13);
PwmOut green2(D12);
PwmOut red2(D11);
PwmOut blue1(D10);
PwmOut green1(D9);
PwmOut red1(D7);

Serial pc(USBTX, USBRX);

struct orientation {
    float pitch; 
    float yaw; 
    float roll; 
};
struct enemy{
    int health;
    int location;  
    enemy* nextEnemy;   
};
//Floats to deal with single-precision to double-precision issues
float one = 1.000; 
float negOne = -1.000;
float half = -0.500;
float halfCircle = 180.000; 
float pointTwo = 0.200; 
float deltaT = 0.016;
enemy* mobs; //Apparently this is a gaming term
enemy* mobsTail; 
/**Updates global variables for lights to change LED values for the sword**/
void blinky(double redOne, double redTwo, double greenOne, double greenTwo, double blueOne, double blueTwo){
    red1 = redOne; 
    red2 = redTwo; 
    green1 = greenOne; 
    green2 = greenTwo; 
    blue1 = blueOne; 
    blue2 = blueTwo; 
}
/**Returns the enemy's health decremented by one, blinks red light, and plays "enemy hit" noise**/
int hit(int enemy_health){
    enemy_health -= 1; 
    blinky(0, 0, 1, 1, 1, 1); 
    ledpin = 1;
    hitNotes();  
    blinky(0, 0, 0, 0, 0, 0); 
    printf("Enemy hit\r\n");
    ledpin = 0; 
    return enemy_health;
}
/**Adds the new enemy "current" to the end of the mobs linked list**/
void enqueue (enemy* current){
    if(mobs == NULL){
        mobs = current; 
    }
    else{
        mobsTail -> nextEnemy = current; 
    }
    mobsTail = current; 
    current -> nextEnemy = NULL; 
}
/**Removes and returns the first item in the mobs queue
Returns null if the queue is empty**/
enemy* dequeue(){
    if(mobs == NULL) return NULL; 
    enemy* newEnemy = mobs; 
    mobs = newEnemy-> nextEnemy; 
    return newEnemy; 
}

int main()
{
    pc.baud(115200);
    printf("\r\nStarting\r\n\r\n");
   // printf("FXOS8700 Combo mag = %X\r\n", combo_mag.whoAmI());
    printf("FXOS8700 Combo acc = %X\r\n", combo_acc.whoAmI());
    printf("FXAS21000 Gyro = %X\r\n", gyro.getWhoAmI());
    
    
    srand(time(0)); //Needed to make sure the random generator works
    int num = (rand()%4)+2; //2-5 enemies 
    //int enemies[num]; 
    for (int i = 0; i<num; i++){
        enemy* current = (enemy*)malloc(sizeof(enemy)); 
        if(current){//Did malloc work
            current -> health = (rand() %12) + 3;//3-15 hp
            current -> location = (rand() %8); //0-7 location
            enqueue(current); 
        }
    }
    printf("Number of enemies: %6d\r\n", num);   
    //Mallocing the various orientation structs  
    orientation *o = (orientation*) malloc(sizeof(orientation));
    orientation *accOri = (orientation*) malloc(sizeof(orientation));
    orientation *gyrOri = (orientation*) malloc(sizeof(orientation));

    o -> pitch = accOri->pitch = 0;
    gyrOri -> pitch = 0; 
    o -> yaw = accOri->yaw = 0;
    gyrOri -> yaw = 0; 
    o -> roll = accOri->roll = 0;
    gyrOri -> roll = 0;
 
    
    float gyro_data[3];
    MotionSensorDataUnits adata;
    combo_acc.enable();
 
    //int16_t acc_raw[3];
     
    printf("\r\nStarting Game\r\n\r\n");
    for(int n = 0; n<num; n++){
    
    wait(1);
    
    float accAv[3] = {0, 0, 0};
    

    enemy* currentEnemy = dequeue();
    //This means the game has just started, so play the music
   
    blinky(1, 1, 1, 1, 0 , 0);
    if(currentEnemy){
        sith(); 
    }
    while(currentEnemy) {
            //sword color blue
        blinky(1, 1, 1, 1,blue1 , blue2);
        pwm(blue1);
        pwm(blue2);

        ledpin = 0;
        
        printf("\r\n");
        gyro.ReadXYZ(gyro_data);
        printf("FXAS21000 Gyro: X:%6.2f Y:%6.2f Z:%6.2f\r\n", gyro_data[0], gyro_data[1], gyro_data[2]);
        gyrOri -> roll = gyro_data[2] * deltaT;
        gyrOri -> pitch = gyro_data[0] * deltaT;
        gyrOri -> yaw = gyro_data[1] * deltaT;
        o->yaw = ((o->yaw + gyrOri->yaw));
        //maintain yaw range of 0-360 degrees
        if (abs(o->yaw) > 360){
          o->yaw = (int)o->yaw%360 + (o->yaw - (int)o->yaw);
        }    
        if (o->yaw<0){ 
            o->yaw+=360;
        }
        printf("Yaw: %6.2f \r\n", o->yaw); 
        //As we've divided the user's location into 8 "quadrants", 
        //dividing yaw by 45 will give us a value between 0-7, which are the 8 different potential quadrants the enemy can be in
        int userLocation = (int)(o -> yaw)/45; 
       
       
        //If this is true, begin stage 2 fighting phase       
        if(userLocation == currentEnemy -> location){
                //sword color white
            blinky(0, 0, 0, 0, 0, 0); 
        while(currentEnemy -> health != 0){
            
            //Small low-pass filter to make sure our accelerometer data is as clean as can be
            for(int i = 0; i<15; i++){
                combo_acc.getAxis(adata);
                accAv[0] += adata.x; 
                accAv[1] += adata.y; 
                accAv[2] += adata.z; 
                }
            accAv[0] = accAv[0]/15;
            accAv[1] = accAv[1]/15;
            accAv[2] = accAv[2]/15;
            //Calculating roll, as this is used to determine if the sword has moved 90 degrees from start position
            o -> roll = halfCircle * atan2(accAv[0],sqrt(pow(accAv[2], 2)+pow(accAv[1], 2)))/M_PI; 
            printf("FXOS8700 Acc:   X:%6.3f Y:%6.3f Z:%6.3f\r\n", accAv[0], accAv[1], accAv[2]);
            printf("Roll: %6.2f", o -> roll);
            //First part of if statement: Has the sword gone from straight up to straight down quickly
            //Second, has the sword moved to the side, then swung inward
            if (((o -> roll < -70.000) && accAv[0] < negOne)|| (accAv[0]<-0.7 && (abs(accAv[1])+abs(accAv[2]))>one)){
                currentEnemy -> health = hit(currentEnemy -> health);
            }
        }
        printf("Enemy Killed\r\n"); 
        for(int light = 0; light<3; light++){
            blinky(1, 1, 0, 0, 1, 1); 
            wait(0.2);
            blinky(0, 0, 0, 0, 0, 0); 
            wait(0.2);
        }
        currentEnemy = dequeue(); 
        accAv[0] = accAv[1] = accAv[2]= 0;  
        }
       
    } 
    
    }
    
    //All Enemies Defeated. Victory!!
    got(); 
    while(1){
        //Flash LED's to go from one color to white to another forever
        blinky(0, 0, 1, 1, 0, 0); 
        wait(0.5); 
        blinky(0, 0, 0, 0, 0, 0);  
        wait(0.5);
        blinky(1, 1, 0, 0, 0, 0); 
        wait(0.5); 
        blinky(0, 0, 0, 0, 0, 0);
        wait(0.5); 
        blinky(0, 0, 0, 0, 1, 1); 
        wait(0.5); 
        blinky(0, 0, 0, 0, 0, 0); 
        wait(0.5); 
    }
}

