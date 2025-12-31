#include "subsystems.hpp"
#include "main.h"
#include "pros/llemu.hpp"
#include "pros/optical.hpp"
#include <type_traits>

int intakeState = IDLE;
bool isHighGoal = true;

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

