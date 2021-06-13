
#ifndef NBODIES_LANDAUFUNCTIONS_H
#define NBODIES_LANDAUFUNCTIONS_H

extern "C" {
  #include "dd.h"
};

int forceSub(dd * forceSub11, dd * body112, dd * body213);
int newState(dd * newState17, dd * bodies118);

#endif // NBODIES_LANDAUFUNCTIONS_H
