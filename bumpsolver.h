#ifndef BUMPSOLVER_H
#define BUMPSOLVER_H

#include "GameClasses.h"
#include "gamecontrol.h"

class BumpSolver
{
public:
  BumpSolver(GControl* parent);
  void Bump(MovableTarget*,BaseGameTarget*,int);
private:
  GControl* parent;
  void DealOverlapActive(MovableTarget*,MovableTarget*,int);
  void DealOverlapPassive(MovableTarget*,BaseGameTarget*,int);
};

#endif // BUMPSOLVER_H
