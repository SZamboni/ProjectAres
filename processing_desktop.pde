
/**
 *  Computer-side program to receive informations from the Arduino program of Project ARES.
 *  This software press some specifics keyboard keys when the Arduino detect movements 
 *
 * @author Simone Zamboni
 * @date 2018-08-22
 */

import processing.serial.*; // imports library for serial communication
import java.awt.Robot; // imports library for key press or release simulation
import java.awt.event.KeyEvent; // imports library for the keys
import java.io.IOException;

Robot robot; // defines Object Robot
Serial myPort;  // Create object from Serial class
String val;     // Data received from the serial port
int DU_state; // head state DOWN(1) and UP(-1), 0 is neutral
int RL_state; // head state LEFT(1) and RIGHT(-1), 0 is neutral
int state_LL; // left leg state, 1 is left kick and 0 is no left kick
int state_RL; // left right state, 1 is right kick and 0 is no right kick
int state_LA; // left arm state, 1 is left punch and 0 is no left punch
int state_RA; // right arm state, 1 is right punch and 0 is no right punch

/**
 * Setup function
 */
void setup()  {
  // Robot Object initialization
    try   {
      robot = new Robot();
    }  catch (Exception e) {
      e.printStackTrace();
      exit();
    }

  // Serial communication setup
  // Open whatever port is the one you're using.
  String COM0 = Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, COM0, 115200); // the baud rate must be equal to the one on the Arduino 
  
  // head state initialization
  DU_state = 0;
  RL_state = 0;
  state_LL = 0;
  state_RL = 0;
  state_LA = 0;
  state_RA = 0;
}

/*
 * Main program loop that reads the moves_string on the serial and press keys based on that
 */
void draw() {  
  if ( myPort.available() > 0)  {  // If data is available,
    val = myPort.readStringUntil('\n');   // read it and store it in val
    
    if(val != null) {
      //println(val);
      
      if(val.length() > 10) {  // val needs to be at least 11 characters.
        // moves_string is send from Arduino and catch by this program and stored in val
        // moves_string format: RIGHT_ARM.LEFT_ARM.RIGHT_LEG.LEFT_LEG.DOWN_UP_STATE.RIGHT_LEFT_STATE.
        // where RIGHT_ARM,LEFT_ARM, RIGHT_LEG, LEFT_LEG  is 1 if there is that movement or 0 if there is nothing,
        // DOWN_UP_STATE is 1 if the DU_state from Arduino is UP, -1 if is DOWN and 0 if there is nothing
        // RIGHT_LEFT_STATE is 1 if the RL_state from Arduino is RIGHT, -1 if is LEFT and 0 if there is nothing
        
        // Moves from arts ('1') press a key and then the key is released when the input is back to '0' :
        // right_arm: v
        // left_arm: c
        // right_leg: x
        // left_leg = z
        
        // Moves from the head press a key when the state changes, and it is released when that state returns to 0
        // head up: w
        // head down = s
        // head right = d
        // head left = a
        
        char right_arm = val.charAt(0);
        
        if(right_arm == '1') {
           //print("RA   ");
           if(state_RA == 0) {
             state_RA = 1;
             robot.keyPress(KeyEvent.VK_V);
           } 
           
        } else if( right_arm == '0') {
          if(state_RA == 1) {
             state_RA = 0;
             robot.keyRelease(KeyEvent.VK_V);
          }
        }
        
        char left_arm = val.charAt(2);
        
        if(left_arm == '1') {
           //print("LA   ");
           if( state_LA == 0) {
               state_LA = 1;
               robot.keyPress(KeyEvent.VK_C);
           } 
        } else if( left_arm == '0') {
          if( state_LA == 1) {
             state_LA = 0;
             robot.keyRelease(KeyEvent.VK_C);
          }
        }
        
        char right_leg = val.charAt(4);
        
        if(right_leg == '1') {
           //print("RL   ");
           if( state_RL == 0) {
             state_RL = 1;
             robot.keyPress(KeyEvent.VK_X);
           }
        } else if( right_leg == '0') {
          if( state_RL == 1) {
             state_RL = 0;
             robot.keyRelease(KeyEvent.VK_X); 
          }
        }
        
        char left_leg = val.charAt(6);
        
        if(left_leg == '1') {
           //print("LL   ");
           if( state_LL == 0) {
              state_LL = 1;
              robot.keyPress(KeyEvent.VK_Z);   
           }
        } else if( left_leg == '0') {
           if( state_LL == 1) {
              state_LL = 0;
              robot.keyRelease(KeyEvent.VK_Z);
           }
        }
        
        char head_du = val.charAt(8);
        
        if(head_du == 'u') {
           //print("UP  ");
           
           if(DU_state == 0) {
              robot.keyPress(KeyEvent.VK_W);
              DU_state = 1;
           }
           
        } else if (head_du == 'd') {
           //print("DW  ");
           
           if(DU_state == 0) {
              robot.keyPress(KeyEvent.VK_S);
              DU_state = -1;
           }
           
        } else {
           if(DU_state == 1) {
              DU_state = 0;
              robot.keyRelease(KeyEvent.VK_W);
           } else if(DU_state == -1) {
              DU_state = 0;
              robot.keyRelease(KeyEvent.VK_S);
           }
        }
        
        char head_rl = val.charAt(10);
        
        if(head_rl == 'l') {
           //print("LF  ");
           
           if(RL_state == 0) {
              robot.keyPress(KeyEvent.VK_A);
              RL_state = -1;
           } 
           
        } else if (head_rl == 'r') {
           //print("RG  ");
           
           if(RL_state == 0) {
              robot.keyPress(KeyEvent.VK_D);
              RL_state = 1;
           } 
           
        } else {
           if(RL_state == 1) {
              RL_state = 0;
              robot.keyRelease(KeyEvent.VK_D);
           } else if(RL_state == -1) {
              RL_state = 0;
              robot.keyRelease(KeyEvent.VK_A);
           }
        }
        
        //println();
      }   
      
    }
    
  } 
  
}
