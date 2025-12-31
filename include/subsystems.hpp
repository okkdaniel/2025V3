#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

extern Drive chassis;

inline pros::Motor bottomRoller(16);
inline pros::Motor middleRoller(-6);
inline pros::Motor topRoller(7);

inline pros::adi::DigitalOut gate('A');
inline pros::adi::DigitalOut bottomLift('A');

inline pros::Optical color('C');

extern pros::Controller master;

enum intakeStates
{
    IDLE = 0,
    INTAKING = 1,
    OUTTAKING = 2,
    SCORING = 3
};

extern int intakeState;
extern bool isHighGoal;
extern bool isBlueAlliance;
extern bool sorting;

void intakeStateManager();
void intakeTeleControl();
void detectBall();
void toggleAlliance();
void setIntakeMotors(int bottom, int middle, int top);
void setPiston(bool gateExtended, bool liftExtended);

void setIntake(int state);
void setHighGoal(bool high);
void setAlliance(bool isBlue);

void colorDetectionTask();