#include "bumpsolver.h"

BumpSolver::BumpSolver(GControl* _parent){
  parent=_parent;
}

void BumpSolver::DealOverlapActive(MovableTarget* T1, MovableTarget* T2, int type){
  if (type==0) T2->setPositionY(T1->getPositionY()+30);else
  if (type==1) T2->setPositionX(T1->getPositionX()-30);else
  if (type==2) T2->setPositionY(T1->getPositionY()-30);else
    T2->setPositionX(T1->getPositionX()+30);
}

void BumpSolver::DealOverlapPassive(MovableTarget* T1, BaseGameTarget* T2, int type){
  double PX,PY;
  if (dynamic_cast<MovableTarget*>(T2)!=nullptr)
    PX=dynamic_cast<MovableTarget*>(T2)->getPositionX(),PY=dynamic_cast<MovableTarget*>(T2)->getPositionY();
  else
    PX=dynamic_cast<StableTarget*>(T2)->getPositionX()*30-15,PY=dynamic_cast<StableTarget*>(T2)->getPositionY()*30-15;
  if (type==0) T1->setPositionY(PY-30);else
  if (type==1) T1->setPositionX(PX+30);else
  if (type==2) T1->setPositionY(PY+30);else
    T1->setPositionX(PX-30);
  if (!(type&1)){
    T1->setVelocityY(0);
    if (type==0&&dynamic_cast<Hero*>(T1)!=nullptr)
      parent->Jumpable=1;
  }
}

//type==0 T1在T2上
//type==1 T1在T2右
//type==2 T1在T2下
//type==3 T1在T2坐
void BumpSolver::Bump(MovableTarget* T1, BaseGameTarget* T2, int type){
  if ((!T1->Enabled)||(!T2->Enabled)) return;
  if (dynamic_cast<MovableBrick*>(T1)!=nullptr){
    if (dynamic_cast<Bullet*>(T2)!=nullptr){
      T2->Enabled=0;
    }else
    if (dynamic_cast<MovableTarget*>(T2)!=nullptr&&dynamic_cast<MovableBrick*>(T2)==nullptr){
      MovableTarget* T2M=dynamic_cast<MovableTarget*>(T2);
      DealOverlapActive(T1,T2M,type);
      if ((type&1)&&(dynamic_cast<MonsterWanderer*>(T2)!=nullptr||dynamic_cast<MonsterFloater*>(T2)!=nullptr))
        T2M->setVelocityX(-T2M->getVelocityX());
      if (!(type&1)&&(dynamic_cast<MonsterFloater*>(T2)!=nullptr))
        T2M->setVelocityY(-T2M->getVelocityY());
    }
  }
  if (dynamic_cast<Hero*>(T1)!=nullptr){
    if (dynamic_cast<Brick*>(T2)!=nullptr||dynamic_cast<MovableBrick*>(T2)!=nullptr||dynamic_cast<Monster*>(T2)!=nullptr)
      DealOverlapPassive(T1,T2,type);
    if (dynamic_cast<Monster*>(T2)!=nullptr){
      if (type==0) T2->Enabled=0,parent->killMonster();else
        parent->getHurtByMonster();
    }
    if (dynamic_cast<EnemyBullet*>(T2)!=nullptr){
      parent->getHurtByMonster();
      T2->Enabled=0;
    }
    if (dynamic_cast<Peach*>(T2)!=nullptr){
      parent->winGame();
    }
    if (dynamic_cast<Mushroom*>(T2)!=nullptr){
      parent->collectMushroom();
      T2->Enabled=0;
    }
    if (dynamic_cast<Flower*>(T2)!=nullptr){
      parent->collectFlower();
      T2->Enabled=0;
    }
    if (dynamic_cast<Trap*>(T2)!=nullptr){
      parent->getHurtByTrap();
    }
    if (dynamic_cast<Spring*>(T2)!=nullptr&&type==0){
      T1->setVelocityY(-2*Constants::HeroJumpVelocity);
      dynamic_cast<Spring*>(T2)->lastPressedStampFrame=parent->getFrame();
      dynamic_cast<Spring*>(T2)->Play("press");
      parent->Jumpable=0;
    }
  }
  
  if (dynamic_cast<MonsterWanderer*>(T1)!=nullptr){
    if (dynamic_cast<Brick*>(T2)!=nullptr||dynamic_cast<MovableBrick*>(T2)!=nullptr||dynamic_cast<Monster*>(T2)!=nullptr){
      DealOverlapPassive(T1,T2,type);
      if (type&1)
        T1->setVelocityX(-T1->getVelocityX());
    } 
    if (dynamic_cast<Spring*>(T2)!=nullptr&&type==0){
      T1->setVelocityY(-2*Constants::HeroJumpVelocity);
      dynamic_cast<Spring*>(T2)->lastPressedStampFrame=parent->getFrame();
      dynamic_cast<Spring*>(T2)->Play("press");
      parent->Jumpable=0;
    }
  }
  
  if (dynamic_cast<MonsterChaser*>(T1)!=nullptr){
    if (dynamic_cast<Brick*>(T2)!=nullptr||dynamic_cast<MovableBrick*>(T2)!=nullptr||dynamic_cast<Monster*>(T2)!=nullptr)
      DealOverlapPassive(T1,T2,type);
    if (dynamic_cast<Spring*>(T2)!=nullptr&&type==0){
      T1->setVelocityY(-2*Constants::HeroJumpVelocity);
      dynamic_cast<Spring*>(T2)->lastPressedStampFrame=parent->getFrame();
      dynamic_cast<Spring*>(T2)->Play("press");
      parent->Jumpable=0;
    }
  }
  
  if (dynamic_cast<MonsterFloater*>(T1)!=nullptr){
    if (dynamic_cast<Brick*>(T2)!=nullptr||dynamic_cast<MovableBrick*>(T2)!=nullptr||dynamic_cast<Monster*>(T2)!=nullptr){ 
      DealOverlapPassive(T1,T2,type);
      if (type&1)
        T1->setVelocityX(-T1->getVelocityX());else
        T1->setVelocityY(-T1->getVelocityY());
    }
  }
  
  if (dynamic_cast<AllyBullet*>(T1)!=nullptr){
    if (dynamic_cast<Brick*>(T2)!=nullptr||dynamic_cast<MovableBrick*>(T2)!=nullptr||dynamic_cast<Monster*>(T2)!=nullptr){     
      T1->Enabled=0;
      if (dynamic_cast<Monster*>(T2))
        T2->Enabled=0,parent->killMonster();
      if (dynamic_cast<DestroyableBrick*>(T2)!=nullptr)
        T2->Enabled=0,parent->crackWall();
    }
  }
    
  if (dynamic_cast<EnemyBullet*>(T1)!=nullptr){
    if (dynamic_cast<Brick*>(T2)!=nullptr||dynamic_cast<MovableBrick*>(T2)!=nullptr||dynamic_cast<Monster*>(T2)!=nullptr){
      T1->Enabled=0;
      if (dynamic_cast<DestroyableBrick*>(T2)!=nullptr)
        T2->Enabled=0,parent->crackWall();
    }
  }
}
