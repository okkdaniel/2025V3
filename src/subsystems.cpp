#include "subsystems.hpp"
#include "main.h"
#include "pros/llemu.hpp"
#include "pros/optical.hpp"
#include <type_traits>

pros::Controller master(pros::E_CONTROLLER_MASTER);

int intakeState = IDLE;
// bool isHighGoal = true;
// bool isBlueAlliance = true;
// bool sorting = false;
// bool colorSortingEnabled = true;

// static uint32_t sortStartTime = 0;
// const uint32_t SORT_EJECT_TIME = 300; // time to eject wrong ball
// const uint32_t SORT_DELAY = 50; // delay before starting to sort

const int INTAKE_SPEED = 127;
const int EJECT_SPEED = -127;

void setIntakeMotors(int bottom,int middle,int top)
{
    bottomRoller.move(bottom);
    middleRoller.move(middle);
    topRoller.move(top);
}


void setPistons(bool gateExtended,bool liftExtended)
{
    gate.set(gateExtended);
    bottomLift.set(liftExtended);
}

// bool detectBall()
// {
//     int proximity = color.get_proximity();
//     int hue = color.get_hue();
    
//     const int PROX_THRESHOLD = 40;

//     if (proximity < PROX_THRESHOLD)
//     {
//         return false;
//     }
    

//     int minHue, maxHue;
//     if (isBlueAlliance)
//     {
//         minHue = 200;
//         maxHue = 230;
//     }
//     else
//     {
//         minHue = 0;
//         maxHue = 30;
//     }
    
//     return (hue >= minHue && hue <= maxHue);
// }

// void detectionManager()
// {
//         while(true)
//     {
//         // detect during scoring
//         if (colorSortingEnabled && intakeState == SCORING)
//         {
//             if (detectBall())
//             {
//                 if (!sorting)
//                 {
//                     sorting = true;
//                     sortStartTime = pros::millis();
//                     pros::lcd::print(7, "Ejecting!");
//                 }
//             }
//         }
//         else
//         {
//             // reset flag
//             if (intakeState != SCORING)
//             {
//                 sorting = false;
//             }
//         }
        
//         int hue = color.get_hue();
//         pros::lcd::print(6, "Alliance: %s | Sort: %s | Hue: %d",
//                         isBlueAlliance ? "Blue" : "Red",
//                         colorSortingEnabled ? "ON" : "OFF",
//                         hue);
        
//         pros::delay(10);
//     }
// }

// void toggleAlliance()
// {
//     isBlueAlliance = !isBlueAlliance;
//     master.rumble(isBlueAlliance ? "..." : "---");
// }

// void toggleColorSorting()
// {
//     colorSortingEnabled = !colorSortingEnabled;
//     if (!colorSortingEnabled)
//     {
//         sorting = false;
//     }
//     master.rumble(colorSortingEnabled ? ".." : "--");
//     // pros::lcd::print(5, "Color Sort: %s", colorSortingEnabled ? "ON" : "OFF");
// }

// if else hell
void intakeStateManager()
{
    // while(true)
    // {
        // if (sorting && colorSortingEnabled && intakeState == SCORING)
        // {
        //     uint32_t sortElapsed = pros::millis() - sortStartTime;

        //     if (sortElapsed < SORT_DELAY)
        //     {
        //         if (isHighGoal)
        //         {
        //             setIntakeMotors(INTAKE_SPEED,INTAKE_SPEED,INTAKE_SPEED);
        //             setPistons(false, false);
        //         }
        //     }

        //     else if (sortElapsed < SORT_DELAY + SORT_EJECT_TIME)
        //     {
        //         if (isHighGoal)
        //         {
        //             setIntakeMotors(INTAKE_SPEED,INTAKE_SPEED,EJECT_SPEED);
        //             setPistons(true, false);
        //         }
        //         else
        //         {
        //             setIntakeMotors(INTAKE_SPEED,INTAKE_SPEED,INTAKE_SPEED);
        //             setPistons(false, false);
        //         }
        //     }
            
        //     else
        //     {
        //         sorting = false;
        //     }

        //     return;
        // }

        if (intakeState == IDLE)
        {
            setIntakeMotors(0,0,0);
            setPistons(true,false);
        }
        else if (intakeState == INTAKING)
        {
            setIntakeMotors(INTAKE_SPEED,INTAKE_SPEED,INTAKE_SPEED);
            setPistons(true,false);

        }
        else if (intakeState == OUTTAKING)
        {
            setIntakeMotors(EJECT_SPEED,EJECT_SPEED,EJECT_SPEED);
            setPistons(true,true);
        }
        else if(intakeState == HIGH_SCORING)
        {
            setIntakeMotors(INTAKE_SPEED,INTAKE_SPEED,INTAKE_SPEED);
            setPistons(false,false);
        }
        else if(intakeState == LOW_SCORING)
        {
            setIntakeMotors(INTAKE_SPEED,INTAKE_SPEED,EJECT_SPEED);
            setPistons(true,false);    
        }

        // pros::delay(20);
    // }
}

void intakeTeleControl()
{
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
    {
        intakeState = INTAKING;
    }
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
    {
        intakeState = OUTTAKING;
    }
    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
    {
        intakeState = HIGH_SCORING;
    }
    else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
    {
        intakeState = LOW_SCORING;
    }
    else
    {
        intakeState = IDLE;
    }

    // high/low toggle

    // if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y))
    // {
    //     toggleColorSorting();
    // }
}

void setIntake(int state)
{
    intakeState = state;
}

void setHighGoal(bool high)
{
    isHighGoal = high;
}

void setColorSorting(bool enabled)
{
    colorSortingEnabled = enabled;
    if (!enabled)
    {
        sorting = false;
    }
}

void printData()
{
    pros::lcd::print(6, "Intake State: %d", intakeState);
    // pros::lcd::print(7, "High Goal: %s", isHighGoal ? "HIGH" : "LOW");

    // pros::lcd::print(2, "Color Sort: %s", colorSortingEnabled ? "ON" : "OFF");
    // pros::lcd::print(3, "Sorting: %s", sorting ? "YES" : "NO");
}