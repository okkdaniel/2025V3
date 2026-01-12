#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

extern Drive chassis;

inline pros::Motor bottomRoller(-16);
inline pros::Motor middleRoller(6);
inline pros::Motor topRoller(-7);

inline ez::Piston gate('D');
inline ez::Piston bottomLift('H');
inline ez::Piston wing('E');
inline ez::Piston parking('G');
inline ez::Piston flipdown('F');

inline pros::Optical color(2);

extern pros::Controller master;

enum intakeStates
{
    IDLE,
    INTAKING,
    OUTTAKING,
    HIGH_SCORING,
    LOW_SCORING
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
void printData();