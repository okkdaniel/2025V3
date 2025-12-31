#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

extern Drive chassis;

inline pros::Motor bottomRoller(16);
inline pros::Motor middleRoller(-6);
inline pros::Motor topRoller(7);

inline pros::adi::DigitalOut gate('A');
inline pros::adi::DigitalOut bottomLift('A');

inline pros::Optical color(10);

extern pros::Controller master;

enum intakeStates
{
    IDLE = 0,
    INTAKING = 1,
    OUTTAKING = 2,
    SCORING = 3,
};

extern int intakeState;
extern bool isHighGoal;
extern bool isBlueAlliance;
extern bool sorting;
extern bool colorSortingEnabled;

void intakeStateManager();
void intakeTeleControl();
bool detectBall();
void detectionManager();
void toggleAlliance();
void toggleColorSorting();