#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

extern Drive chassis;

inline pros::Motor bottomRoller(-16);
inline pros::Motor middleRoller(6);
inline pros::Motor topRoller(-7);

inline pros::adi::DigitalOut gate('D');
inline pros::adi::DigitalOut bottomLift('H');
inline pros::adi::DigitalOut parking('G');
inline ez::Piston descore('E');
inline ez::Piston flipdown('F');

inline pros::Optical color(2);

extern pros::Controller master;

enum intakeStates
{
    IDLE = 0,
    INTAKING = 1,
    OUTTAKING = 2,
    SCORINGHIGH = 3,
    SCORINGMID = 4,
    SCORING = 5,
};

extern int intakeState;
extern bool isHighGoal;
extern bool isBlueAlliance;
extern bool sorting;
extern bool colorSortingEnabled;

void intakeStateManager();
bool detectBall();
void detectionManager();
void colorSortingTask();

void intakeTeleControl();
void toggleAlliance();
void toggleColorSorting();

void setIntake(int state);
void setHighGoal(bool high);
void setAlliance(bool isBlue);
void setColorSorting(bool enabled);