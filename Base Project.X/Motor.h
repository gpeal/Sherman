#ifndef MOTOR_H
#define MOTOR_H

//Sets up a motor controller.
void setupMotor(int id, int isAnalogue, int timer, int output);

//Controls motor functions
void controlMotor(int id);

#endif