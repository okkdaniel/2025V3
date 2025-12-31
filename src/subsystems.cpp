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

static uint32_t sortStartTime = 0;
const uint32_t SORT_EJECT_TIME = 300; // time to eject wrong ball
const uint32_t SORT_DELAY = 50; // delay before ejecting

void setIntakeMotors(int bottom, int middle, int top)
{
    bottomRoller.move(bottom);
    middleRoller.move(middle);
    topRoller.move(top);
}

void setPistons(bool gateExtended, bool liftExtended)
{
    gate.set_value(gateExtended);
    bottomLift.set_value(liftExtended);
}

void detectBall()
{
    while(true)
    {
        int proximity = color.get_proximity();
        int hue = color.get_hue();

        const int PROX_THRESHOLD = 40;

        int minHue, maxHue;
        if (isBlueAlliance)
        {
            minHue = 0;
            maxHue = 30;
        }
        else
        {
            minHue = 200;
            maxHue = 230;
        }

        pros::lcd::print(6, "Alliance: %s | Prox: %d | Hue: %d",
                        isBlueAlliance ? "Blue" : "Red", proximity, hue);
        
        
        if (intakeState == INTAKING && proximity > PROX_THRESHOLD)
        {
            if (hue >= minHue && hue <= maxHue)
            {
                if (!sorting)
                {
                    sorting = true;
                    sortStartTime = pros::millis();
                    pros::lcd::print(7, "Ejecting!");
                }
            }
        }
        pros::delay(10);
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
        master.rumble(isHighGoal ? "." : "-"); // vibration indicator
    }
}

