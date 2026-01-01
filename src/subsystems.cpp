#include "subsystems.hpp"
#include "main.h"
#include "pros/llemu.hpp"
#include "pros/optical.hpp"
#include <type_traits>

pros::Controller master(pros::E_CONTROLLER_MASTER);

int intakeState = IDLE;
bool isHighGoal = true;
bool isBlueAlliance = true;
bool sorting = false;
bool colorSortingEnabled = true;

static uint32_t sortStartTime = 0;
const uint32_t SORT_EJECT_TIME = 300; // time to eject wrong ball
const uint32_t SORT_DELAY = 50; // delay before starting to sort

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
    gate.set_value(gateExtended);
    bottomLift.set_value(liftExtended);
}

bool detectBall()
{
    int proximity = color.get_proximity();
    int hue = color.get_hue();
    
    const int PROX_THRESHOLD = 40;

    if (proximity < PROX_THRESHOLD)
    {
        return false;
    }
    

    int minHue, maxHue;
    if (isBlueAlliance)
    {
        minHue = 200;
        maxHue = 230;
    }
    else
    {
        minHue = 0;
        maxHue = 30;
    }
    
    return (hue >= minHue && hue <= maxHue);
}

void detectionManager()
{
        while(true)
    {
        // detect during scoring
        if (colorSortingEnabled && intakeState == SCORING)
        {
            if (detectBall())
            {
                if (!sorting)
                {
                    sorting = true;
                    sortStartTime = pros::millis();
                    pros::lcd::print(7, "Ejecting!");
                }
            }
        }
        else
        {
            // reset flag
            if (intakeState != SCORING)
            {
                sorting = false;
            }
        }
        
        int proximity = color.get_proximity();
        pros::lcd::print(6, "Alliance: %s | Sort: %s | Prox: %d",
                        isBlueAlliance ? "Blue" : "Red",
                        colorSortingEnabled ? "ON" : "OFF",
                        proximity);
        
        pros::delay(10);
    }
}

void toggleAlliance()
{
    isBlueAlliance = !isBlueAlliance;
    master.rumble(isBlueAlliance ? "..." : "---");
}

void toggleColorSorting()
{
    colorSortingEnabled = !colorSortingEnabled;
    if (!colorSortingEnabled)
    {
        sorting = false;
    }
    master.rumble(colorSortingEnabled ? ".." : "--");
    pros::lcd::print(5, "Color Sort: %s", colorSortingEnabled ? "ON" : "OFF");
}

// if else hell
void intakeStateManager()
{
    if (sorting && colorSortingEnabled && intakeState == SCORING)
    {
        uint32_t sortElapsed = pros::millis() - sortStartTime;

        if (sortElapsed < SORT_DELAY)
        {
            if (isHighGoal)
            {
                setIntakeMotors(INTAKE_SPEED,INTAKE_SPEED,INTAKE_SPEED);
                setPistons(false, false);
            }
        }

        else if (sortElapsed < SORT_DELAY + SORT_EJECT_TIME)
        {
            if (isHighGoal)
            {
                setIntakeMotors(INTAKE_SPEED,INTAKE_SPEED,EJECT_SPEED);
                setPistons(true, false);
            }
            else
            {
                setIntakeMotors(INTAKE_SPEED,INTAKE_SPEED,INTAKE_SPEED);
                setPistons(false, false);
            }
        }
        
        else
        {
            sorting = false;
        }

        return;
    }

    if (intakeState == IDLE)
    {
        setIntakeMotors(0,0,0);
        setPistons(true,false);
    }
    else if (intakeState == INTAKING)
    {
        setIntakeMotors(INTAKE_SPEED,INTAKE_SPEED,0);
        setPistons(true,false);

    }
    else if (intakeState == OUTTAKING)
    {
        setIntakeMotors(EJECT_SPEED,EJECT_SPEED,EJECT_SPEED);
        setPistons(true,true);
    }
    else if(intakeState == SCORING)
    {
        if (isHighGoal)
        {
            setIntakeMotors(INTAKE_SPEED,INTAKE_SPEED,INTAKE_SPEED);
            setPistons(false,false);
        }
        else
        {
            setIntakeMotors(INTAKE_SPEED,INTAKE_SPEED,INTAKE_SPEED);
            setPistons(true,false);
        }
    }
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
    else if ((master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) && (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)))
    {
        intakeState = SCORING;
    }
    else
    {
        intakeState = IDLE;
    }

    // high/low toggle
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2))
    {
        isHighGoal = !isHighGoal;
        master.rumble(isHighGoal ? "." : "-");
    }

    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y))
    {
        toggleColorSorting();
    }
}