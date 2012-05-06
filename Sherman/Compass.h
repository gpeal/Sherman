#ifndef COMPASS_H
#define COMPASS_H

struct Orientation {
    double X;
    double Y;
    double Z;
};

void setupCompass();
struct Orientation readCompass();

#endif