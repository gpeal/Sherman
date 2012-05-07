#ifndef COMPASS_H
#define COMPASS_H

struct Orientation {
    short X;
    short Y;
    short Z;
};

void setupCompass();
struct Orientation readCompass();

#endif