#include "vex.h"

using namespace vex;

brain Brain;

inertial BrainInertial = inertial();

motor LeftMotor = motor(PORT1, true);
motor RightMotor = motor(PORT5, false);

motor ClawMotor = motor(PORT2, false);
motor ClawMotor2 = motor(PORT4, false);

controller Controller = controller();
optical ColourSensor =  optical(PORT2);


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
ColourSensor.setLightPower(75, percent);
ColourSensor.setLight(ledState::on);

    moveClawOpen(50, 100);

    DriveForward(60, 700);
    turnLeft(65, 550);

    DriveForward(50, 700);
    if (ColourSensor.color() == color::green){
        Brain.Screen.print("Hello");
     moveClawClose(50, 500);
     DriveBackward(50, 100);
     moveClawArmUp(50, 400);
     DriveForward(50, 100);
     moveClawOpen(50, 100);
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
runAuton();
}
