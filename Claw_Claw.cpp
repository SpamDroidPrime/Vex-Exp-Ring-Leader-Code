#include "vex.h"

using namespace vex;

brain Brain;

inertial BrainInertial = inertial();

motor LeftMotor = motor(PORT1, true);
motor RightMotor = motor(PORT5, false);

motor ClawMotor = motor(PORT2, false);
motor ClawMotor2 = motor(PORT4, false);

controller Controller = controller();

int main()
{
    float deadband = 3.0;
    float turning = 0.65;
    while (true)
    {
        if (Controller.ButtonR1.pressing())
    {
        ClawMotor.spin(forward, 100, percent);
        Brain.Screen.print("Pressed");
    }
    else if (Controller.ButtonR2.pressing())
    {
        ClawMotor.spin(reverse, 100, percent);
    }
    else 
    {
        ClawMotor.stop(brakeType::coast); 
    }

    if (Controller.ButtonL1.pressing())
    {
        ClawMotor2.spin(forward, 100, percent);
    }
    else if (Controller.ButtonL2.pressing())
    {
        ClawMotor2.spin(reverse, 100, percent);
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
    

