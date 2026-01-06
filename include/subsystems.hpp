#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

extern Drive chassis;

inline pros::Motor bottomRoller(-16);
inline pros::Motor middleRoller(6);
inline pros::Motor topRoller(-7);

inline pros::adi::DigitalOut gate('D');
inline pros::adi::DigitalOut bottomLift('H');
inline pros::adi::DigitalOut descore('E');
inline pros::adi::DigitalOut parking('G');
inline pros::adi::DigitalOut flipdown('F');

inline pros::Optical color(2);

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
bool detectBall();
void detectionManager();

void intakeTeleControl();
void toggleAlliance();
void toggleColorSorting();

void setIntake(int state);
void setHighGoal(bool high);
void setAlliance(bool isBlue);
void setColorSorting(bool enabled);