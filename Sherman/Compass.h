#ifndef COMPASS_H
#define COMPASS_H

struct Orientation {
    short int X;
    short int Y;
    short int Z;
};

void setupCompass();
struct Orientation readCompass();

#endif