#include "vex.h"

using namespace vex;

brain Brain;

//assign ports
inertial BrainInertial = inertial();

motor LeftMotor = motor(PORT1, true);
motor RightMotor = motor(PORT5, false);

motor ClawMotor = motor(PORT2, false); 
motor ClawMotor2 = motor(PORT4, false);

controller Controller = controller();
optical ColourSensor =  optical(PORT3); //fixed port



void DriveForward(int Speed, int timeMS){
LeftMotor.spin(forward, Speed, percent);
RightMotor.spin(forward, Speed, percent);

wait(timeMS, msec);

LeftMotor.stop(brakeType::hold);
RightMotor.stop(brakeType::hold);

}
 void DriveBackward(int Speed, int timeMS){

LeftMotor.spin(reverse, Speed, percent);
RightMotor.spin(reverse, Speed, percent);

wait(timeMS, msec);

LeftMotor.stop(brakeType::hold);
RightMotor.stop(brakeType::hold);
}

void turnLeft(int Speed, int timeMS){
LeftMotor.spin(reverse, Speed, percent);
RightMotor.spin(forward, Speed, percent);

wait(timeMS, msec);

LeftMotor.stop(brakeType::hold);
RightMotor.stop(brakeType::hold);

}

void turnLeft90(){
    BrainInertial.resetRotation();
    LeftMotor.spin(reverse, 25, percent);
    RightMotor.spin(forward, 25, percent);
    while (BrainInertial.rotation() > -85.0) {
        wait(10, msec);
    }
    LeftMotor.stop(brakeType::hold);
    RightMotor.stop(brakeType::hold);
}

void turnRight90(){
    BrainInertial.resetRotation();// the interial sensor starts of at 0. 
    LeftMotor.spin(forward, 25, percent);
    RightMotor.spin(reverse, 25, percent); 
    while (BrainInertial.rotation() < 85.0) { // stops the motor right on time so it has a little bit time to reposition itself so it reaches perfect 90.
        wait(10, msec);
    }
    LeftMotor.stop(brakeType::hold);
    RightMotor.stop(brakeType::hold);
}

void turnRight(int Speed, int timeMS){
LeftMotor.spin(forward, Speed, percent);
RightMotor.spin(reverse, Speed, percent);

wait(timeMS, msec);

LeftMotor.stop(brakeType::hold);
RightMotor.stop(brakeType::hold);

}

void moveClawArmUp(int Speed, int timeMS){

ClawMotor2.spin(forward, Speed, percent);

wait(timeMS, msec);

ClawMotor2.stop(brakeType::hold); 

}
void moveClawArmDown(int Speed, int timeMS){

ClawMotor2.spin(reverse, Speed, percent);

wait(timeMS, msec);

ClawMotor2.stop(brakeType::hold);

}
void moveClawOpen(int Speed, int timeMS){

ClawMotor.spin(reverse, Speed, percent);

wait(timeMS, msec);

ClawMotor.stop(brakeType::hold);

}
void moveClawClose(int Speed, int timeMS){

ClawMotor.spin(forward, Speed, percent);

wait(timeMS, msec);

ClawMotor.stop(brakeType::hold);

}

void runAuton(){
ClawMotor.setStopping(brakeType::hold);
ColourSensor.setLightPower(75, percent);
ColourSensor.setLight(ledState::on);
 bool greenDetected = false;
bool blueDetected =  false;
bool redDetected = false;
    
    moveClawOpen(50, 225);

    DriveForward(60, 2850);
    turnLeft90();
    DriveForward(60, 1800);


    for (int i = 0; i < 150; i++) { // runs this for 150 times
        int hue = ColourSensor.hue();
        Brain.Screen.clearScreen();
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("Hue: ");
        Brain.Screen.print(hue); // prints the hue on the brain
        
        if (hue >= 60 && hue <= 180) { // if this is green
            greenDetected = true;
            break;
        }
        if (hue >= 60 && hue <= 180) { // if this is green
            blueDetected = true;
            break;
        }
        if (hue >= 60 && hue <= 180) { // if this is green
            redDetected = true;
            break;
        }
        wait(20, msec);
    }
    if (greenDetected){
     moveClawClose(50, 500);
     DriveBackward(50, 100);
     moveClawArmUp(50, 700);
     DriveForward(50, 100);
     moveClawOpen(50, 100);
    }
    if (blueDetected){
        moveClawOpen(50, 500);
        DriveBackward(50, 100);
        turnRight90();
        DriveForward(60, 1670);
    }
     
    if(redDetected){
       moveClawClose(50,500);
       DriveBackward(50, 100);
       moveClawArmUp(50, 700);
       turnRight90();
     
    }
}

void Driver(){
    float deadband = 3.0;
    float turning = 1;
    while (true)
    {
        if (Controller.ButtonR1.pressing())
    {
        ClawMotor.spin(forward, 50, percent);
        Brain.Screen.print("Pressed");
    }
    else if (Controller.ButtonR2.pressing())
    {
        ClawMotor.spin(reverse, 60, percent);
    }
    else 
    {
        ClawMotor.stop(brakeType::hold); 
    }

    if (Controller.ButtonL1.pressing())
    {
        ClawMotor2.spin(forward, 60, percent);
    }
    else if (Controller.ButtonL2.pressing())
    {
        ClawMotor2.spin(reverse, 60, percent);
    }
    else
    {
        ClawMotor2.stop(brakeType::hold);
    }

            if (abs(Controller.Axis3.position()) + abs(Controller.Axis1.position()) > deadband)
            {
                LeftMotor.setVelocity(Controller.Axis3.position() + Controller.Axis1.position() * turning, percent);
                RightMotor.setVelocity(Controller.Axis3.position() - Controller.Axis1.position() * turning, percent);
            }
            else 
            {
                LeftMotor.setVelocity(0, percent);
                RightMotor.setVelocity(0, percent);
            }
            LeftMotor.spin(forward);
            RightMotor.spin(forward);

             wait(10, msec);
        }
}
    
int main()
{
    BrainInertial.calibrate();
    while (BrainInertial.isCalibrating()) {
        wait(20, msec);
    }
    runAuton();
}