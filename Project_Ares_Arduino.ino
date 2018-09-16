/*
  Projet Ares Main program

 Wirings for every GY-521:
  VCC  5V (on parallel)
  GNS GND (on parallel)
  SCL A5 (on parallel)
  SDA A4 (on parallel)
  AD0  (2 = Right Leg, 3 = Left Leg, 4 = Head, 5 = Right Arm, 6 = Left Arm)

  @author Simone Zamboni and Michele Roselli
  @date 2018-08-22

*/

// Include libraries for the MPU6050 sensor
#include <Wire.h>
#include <MPU6050.h>

// Declaring a MPU6050 Object for every GY-521 sensor
MPU6050 mpu_la; // Left Arm MPU6050 Object
MPU6050 mpu_ra; // Right Arm MPU6050 Object
MPU6050 mpu_h; // Head MPU6050 Object
MPU6050 mpu_ll; // Left Leg MPU6050 Object
MPU6050 mpu_rl; // Right Leg MPU6050 Object

int sensor_being_read;  // which sensor have A0 on LOW and so we can communicate with that one

String moves_string; // String sent every 5 loop that contains every move registered

// Moves timer declaration
unsigned long time; // Variable that stores the time
unsigned long timer_la; // Left arm move timer
unsigned long timer_ra; // Right arm move timer
unsigned long timer_ll; // Left leg move timer
unsigned long timer_rl; // Right leg move timer
unsigned long timer_head_UD; // Head UP/DOWN move timer
unsigned long timer_head_RL; // Head RIGHT/LEFR timer

// Read time for every move declaration
int activation_time_ll;
int activation_time_rl;
int activation_time_la;
int activation_time_ra;
int activation_time_head_ud;
int activation_time_head_rl;
int deactivation_time_head_ud;
int deactivation_time_head_rl;

// Arts thresholds declararion
double arm_accx;
double leg_norm;

// Head thresholds declararion
double head_ud_ygyro_0_to_1;
double head_ud_ygyro_1_to_0;
double head_ud_ygyro_0_to_m1;
double head_ud_ygyro_m1_to_0;

double head_rl_ygyro_0_to_1;
double head_rl_ygyro_1_to_0;
double head_rl_ygyro_0_to_m1;
double head_rl_xgyro_m1_to_0;

// Head states declaration
int DU_state;  // down (1) / up (-1)
int RL_state; // right (1) / left (1)

/**
 * Setup function
 */
void setup() {
  // Serial initialization
  Serial.begin(115200);

  // Pins on Arduino where the AD0 pins of the GY-521 sensors are connected
  pinMode(2, OUTPUT); // Right leg AD0
  pinMode(3, OUTPUT); // Left leg AD0
  pinMode(4, OUTPUT); // Head AD0
  pinMode(5, OUTPUT); // Right arm AD0
  pinMode(6, OUTPUT); // Left arm AD0

  pinMode(7, INPUT); // Pin forthe start button

  // initialize all the pins to high
  digitalWrite(2,HIGH);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);

  // Start reading from the right leg
  sensor_being_read = 2;

  moves_string = "";

  // Head state initialization
  DU_state = 0;
  RL_state = 0;

  // Timer moves inizialization
  timer_la = 0;
  timer_ra = 0;
  timer_ll = 0;
  timer_rl = 0;
  timer_head_UD = 0;
  timer_head_RL = 0;

  // Activation time initialization
  activation_time_ll = 600;
  activation_time_rl = 600;
  activation_time_la = 400;
  activation_time_ra = 400;
  activation_time_head_ud = 300;
  activation_time_head_rl = 300;
  deactivation_time_head_ud = 2000;
  deactivation_time_head_rl = 2000;

  // Arts thresholds initialization
  arm_accx = 19;
  leg_norm = 18;

  // head thresholds initialization
  head_ud_ygyro_0_to_1 = 60;
  head_ud_ygyro_1_to_0 = -40;
  head_ud_ygyro_0_to_m1 = -60;
  head_ud_ygyro_m1_to_0 = 40;

  head_rl_ygyro_0_to_1 = 50;
  head_rl_ygyro_1_to_0 = -30;
  head_rl_ygyro_0_to_m1 = -50;
  head_rl_xgyro_m1_to_0 = 30;
  

  // Initialization loop for every MPU6050 Object 
  for(int i = 2; i <=6; i++) {
     // To communicate with the sensor we have to set AD0 to LOW
     digitalWrite(i,LOW);
     // initialize process for every MPU6050 Object
     
     if(i == 2) {
        //Serial.println("Initialize MPU6050 - 2");
        while(!mpu_rl.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))   {
          Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
          delay(500);
        }
        mpu_rl.calibrateGyro();
        mpu_rl.setThreshold(3);
     } else if(i == 3) {
        //Serial.println("Initialize MPU6050 - 3");
        while(!mpu_ll.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))   {
          Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
          delay(500);
        }
        mpu_ll.calibrateGyro();
        mpu_ll.setThreshold(3);
     } else if(i == 4) {
        //Serial.println("Initialize MPU6050 - 4");
        while(!mpu_h.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))   {
          Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
          delay(500);
        }
        mpu_h.calibrateGyro();
        mpu_h.setThreshold(3);
     } else if(i == 5) {
        //Serial.println("Initialize MPU6050 - 5");
        while(!mpu_ra.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))   {
          Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
          delay(500);
        }
        mpu_ra.calibrateGyro();
        mpu_ra.setThreshold(3);
     } else if(i == 6) {
        //Serial.println("Initialize MPU6050 - 6");
        while(!mpu_la.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))   {
          Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
          delay(500);
        }
        mpu_la.calibrateGyro();
        mpu_la.setThreshold(3);
     } 
     // Set the sensor AD0 back to HIGH
     digitalWrite(i,HIGH);
  }
  
  delay(2000);
}

/**
 * Function to chack the settings of one MPU6050 Object
 * This function can be useful but it's not critical
 */ /*
void checkSettings() {
  Serial.println();
  
  Serial.print(" * Sleep Mode:        ");
  Serial.println(mpu_rl.getSleepEnabled() ? "Enabled" : "Disabled");
  
  Serial.print(" * Clock Source:      ");
  switch(mpu_rl.getClockSource()) {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  
  Serial.print(" * Gyroscope:         ");
  switch(mpu_rl.getScale()) {
    case MPU6050_SCALE_2000DPS:        Serial.println("2000 dps"); break;
    case MPU6050_SCALE_1000DPS:        Serial.println("1000 dps"); break;
    case MPU6050_SCALE_500DPS:         Serial.println("500 dps"); break;
    case MPU6050_SCALE_250DPS:         Serial.println("250 dps"); break;
  } 
  
  Serial.print(" * Gyroscope offsets: ");
  Serial.print(mpu_rl.getGyroOffsetX());
  Serial.print(" / ");
  Serial.print(mpu_rl.getGyroOffsetY());
  Serial.print(" / ");
  Serial.println(mpu_rl.getGyroOffsetZ());
  
  Serial.println();
}*/

/**
 * Function that return 1 if there is a right kick or 0 otherwise
 */
int check_rl() {
  // check if are passed activation_time_rl milliseconds from the last right kick 
   if(time - timer_rl > activation_time_rl) {

    // read the accelleration from the sensor
    Vector normAccel = mpu_rl.readNormalizeAccel();
    double accx = normAccel.XAxis;
    double accy = normAccel.YAxis;
    double accz = normAccel.ZAxis;
    // norm calculation
    double norm = sqrt( accx*accx + accy*accy + accz*accz);
    
    if(norm > leg_norm) {
      //Serial.println("RIGHT KICK");
      timer_rl = time; // save the time of this move
      return 1;
    }
  } else if( time - timer_rl < 100) { // to click the button for 100m
    return 1; 
  }
  return 0;
}

/**
 * Function that return 1 if there is a left kick or 0 otherwise
 */
int check_ll() {
   // check if are passed activation_time_ll milliseconds from the last left kick 
   if(time - timer_ll > activation_time_ll) {

    // read the accelleration from the sensor
    Vector normAccel = mpu_ll.readNormalizeAccel();
    double accx = normAccel.XAxis;
    double accy = normAccel.YAxis;
    double accz = normAccel.ZAxis;
    // norm calculation
    double norm = sqrt( accx*accx + accy*accy + accz*accz);

    if(norm > leg_norm) {
      //Serial.println("LEFT KICK");
      timer_ll = time; // save the time of this move
      return 1;
    }
  } else if( time - timer_ll < 100) { // to click the button for 100m
    return 1;
  }
  return 0;
}

/**
 * Function that return 1 if there is a left punch or 0 otherwise
 */
int check_ra() {
  // check if are passed activation_time_ra milliseconds from the last right punch 
  if(time - timer_ra > activation_time_ra) {

    // read the accelleration from the sensor
    Vector normAccel = mpu_ra.readNormalizeAccel();
    double accx = normAccel.XAxis;
    
    if(accx > arm_accx) {
      //Serial.println("RIGHT PUNCH");
      timer_ra = time; // save the time of this move
      return 1;
    }
  } else if( time - timer_ra < 100) { // to click the button for 100m
    return 1;
  }
  return 0;
}

/**
 * Function that return 1 if there is a left punch or 0 otherwise
 */
int check_la() {
  // check if are passed activation_time_la milliseconds from the last left punch 
  if(time - timer_la > activation_time_la) {

    // read the accelleration from the sensor
    Vector normAccel = mpu_la.readNormalizeAccel();
    double accx = normAccel.XAxis;

    if(accx > arm_accx) {
      //Serial.println("LEFT PUNCH");
      timer_la = time; // save the time of this move
      return 1;
    }
  } else if( time - timer_la < 100) { // to click the button for 100m
    return 1;
  }
  return 0;
}

/**
 * Function that modify DU_state and RL_state based on the gyro
 */
void check_head() {
  // read the gyro from the sensor
  Vector normGyro = mpu_h.readNormalizeGyro();
  double xgyro = normGyro.XAxis;
  double ygyro = normGyro.YAxis;
  double zgyro = normGyro.ZAxis;

  // Up and Down state update
  if(time - timer_head_UD > activation_time_head_ud) {
    
    if(DU_state == 0){

      if(ygyro > head_ud_ygyro_0_to_1) {
        DU_state = 1;
        timer_head_UD = time; // save the time of this move
        //Serial.print("DU_state:  ");  Serial.println(DU_state);
      } else if( ygyro < head_ud_ygyro_0_to_m1) {
        DU_state = -1;
        timer_head_UD = time; // save the time of this move
        //Serial.print("DU_state:  ");  Serial.println(DU_state);
      }
    } else if( DU_state == 1) {

      if(ygyro < head_ud_ygyro_1_to_0 ) {
        DU_state = 0;
        timer_head_UD = time; // save the time of this move
        //Serial.print("DU_state:  "); Serial.println(DU_state);
      } 
    } else {
      if(ygyro > head_ud_ygyro_m1_to_0 ) {
        DU_state = 0;
        timer_head_UD = time; // save the time of this move
        //Serial.print("DU_state:  "); Serial.println(DU_state);
      } 
    }
  }

  // to avoid this state never change after deactivation_time_head_ud the UD state return to 0
  if(time - timer_head_UD > deactivation_time_head_ud) {
    DU_state = 0;
  }

  // Right and Left state update
  if(time - timer_head_RL > activation_time_head_rl) {
    if(RL_state == 0){

      if(xgyro > head_rl_ygyro_0_to_1) {
        RL_state = 1;
        timer_head_RL = time; // save the time of this move
        //Serial.print("RL_state:  ");Serial.println(RL_state);
      } else if( xgyro < head_rl_ygyro_0_to_m1) {
        RL_state = -1;
        timer_head_RL = time; // save the time of this move
        //Serial.print("RL_state:  "); Serial.println(RL_state);
      }
    } else if( RL_state == 1) {
      
      if(xgyro < head_rl_ygyro_1_to_0 ) {
        RL_state = 0;
        timer_head_RL = time; // save the time of this move
        //Serial.print("RL_state:  "); Serial.println(RL_state);
      } 
    } else {

      if(xgyro > head_rl_xgyro_m1_to_0 ) {
        RL_state = 0;
        timer_head_RL = time; // save the time of this move
        //Serial.print("RL_state:  ");  Serial.println(RL_state);      
       } 
    }
  }

  // to avoid this state never change after deactivation_time_head_ud the RL state return to 0
  if(time - timer_head_RL > deactivation_time_head_rl) {
    RL_state = 0;
  }
}

/**
 * Main Loop that calls the previous functions 
 */
void loop(){
   
   time = millis(); // save the current time

  // if we have to read the sensor on the pin 2, we set the previous sensor AD0 on HIGH, 
  // and we set the pin 2 on LOW, so that we can communicate with the sensor on the pin 2.
  // After that we wait 1 millisecond, we increase sensor_being_read, and then we call the 
  // function of the body part on which the sensor is on. 
  // Based on the result of the function we append a 1. to the moves_string if there is a action,
  // or 0. if there is no action. Exception to this rule is the head

  // the moves_string is sent to the processor software using the serial in the head if

  // moves_string format: RIGHT_ARM.LEFT_ARM.RIGHT_LEG.LEFT_LEG.DOWN_UP_STATE.RIGH_LEFT_STATE.\n
  
  if(sensor_being_read == 2) {
    digitalWrite(6, HIGH);  
    digitalWrite(2, LOW);
    delay(1);
    sensor_being_read++;
    if( check_rl() == 1) {
      moves_string += "1.";
    } else {
      moves_string += "0.";
    }
  } else if(sensor_being_read == 3){ 
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW); 
    delay(1);
    sensor_being_read++;
     if( check_ll() == 1) {
      moves_string += "1.";
    } else {
      moves_string += "0.";
    }
  } else if(sensor_being_read == 4){ 
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW); 
    delay(1);
    sensor_being_read++;
    check_head();
    
    // to create the moves_string on the head we check the DU_state and the RL_state
    if(DU_state == 1) {
      moves_string += "d."; // d = DOWN
    } else if(DU_state == -1) {
      moves_string += "u."; // u = UP
    } else {
      moves_string += "0.";
    }
  
    if(RL_state == 1) {
      moves_string += "r."; // r = RIGHT
    } else if(RL_state == -1) {
      moves_string += "l."; // l = LEFT
    } else {
      moves_string += "0.";
    }
    Serial.println(moves_string);
    moves_string = "";
  } else if(sensor_being_read == 5){ 
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW); 
    delay(1);
    sensor_being_read++;
    if( check_ra() == 1) {
      moves_string += "1.";
    } else {
      moves_string += "0.";
    }
  } else { 
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW); 
    delay(1);
    sensor_being_read = 2;
     if( check_la() == 1) {
      moves_string += "1.";
    } else {
      moves_string += "0.";
    }
  } 
  
  delay(4); // wait to avoid filling the serial receiving buffer

}
