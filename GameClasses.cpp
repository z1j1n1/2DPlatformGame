#include "GameClasses.h"

BaseGameTarget::BaseGameTarget(GControl* _parent){
  parent=_parent;
  Enabled=1;
  PlayedFrame=0;
}

//播放对象的指定动画
void BaseGameTarget::PlayAni(string input){
  if (CurrentlyPlaying!=input) PlayedFrame=0;
  CurrentlyPlaying=input;  
}

//播放对象的指定动画与声音
void BaseGameTarget::Play(string input){
  if (CurrentlyPlaying!=input) PlayedFrame=0;
  CurrentlyPlaying=input;
  if (Sound.count(input))
    Sound[input].play();
}

//获取对象动画的下一帧
void BaseGameTarget::NextFrame(){
  PlayedFrame=(PlayedFrame+1)%Animation[CurrentlyPlaying].size();
}

//获取对象动画当前的帧
const QPixmap& BaseGameTarget::getFrame(){
  return(Animation[CurrentlyPlaying][PlayedFrame]);
}

MovableTarget::MovableTarget(GControl* parent,double _PX,double _PY):BaseGameTarget(parent){
  PositionX=_PX;PositionY=_PY;
  VelocityX=0;VelocityY=0;
}

//设置X轴速度
void MovableTarget::setVelocityX(double input){
  VelocityX=input;
}

//设置Y轴速度
void MovableTarget::setVelocityY(double input){
  VelocityY=input;
}

//设置X轴坐标
void MovableTarget::setPositionX(double input){
  PositionX=input;
}

//设置Y轴坐标
void MovableTarget::setPositionY(double input){
  PositionY=input;
}

//获取X轴速度
double MovableTarget::getVelocityX(){
  return(VelocityX);
}

//获取Y轴速度
double MovableTarget::getVelocityY(){
  return(VelocityY);
}

//获取X轴坐标
double MovableTarget::getPositionX(){
  return(PositionX);
}

//获取Y轴坐标
double MovableTarget::getPositionY(){
  return(PositionY);
}

//做出水平移动
void MovableTarget::makeMoveHorizontal(){
  setPositionX(getPositionX()+getVelocityX());
}

//做出竖直移动
void MovableTarget::makeMoveVertical(){
  setPositionY(getPositionY()+getVelocityY());
}

StableTarget::StableTarget(GControl* parent,int _PX,int _PY):BaseGameTarget(parent){
  PositionX=_PX;PositionY=_PY;
}

//获取X轴方格数
int StableTarget::getPositionX(){
  return(PositionX);
}

//获取Y轴方格数
int StableTarget::getPositionY(){
  return(PositionY);
}

void StableTarget::act(){
}

Hero::Hero(GControl* parent, double PX, double PY):MovableTarget(parent,PX,PY){
  //主角的跳跃动画与声音
  vector <QPixmap> Jump;
  Jump.clear();
  Jump.push_back(QPixmap(":/play/PlayMode/mario_stop.png"));
  Animation["jump"]=Jump;
  Sound["jump"].setSource(QUrl("qrc:/wav/wavaudios/jump.wav"));
  
  //主角向左移动的动画
  vector <QPixmap> MoveLeft;
  MoveLeft.clear();
  MoveLeft.push_back(QPixmap(":/play/PlayMode/mario_walk_sprites_left1.png"));
  MoveLeft.push_back(QPixmap(":/play/PlayMode/mario_walk_sprites_left2.png"));  
  MoveLeft.push_back(QPixmap(":/play/PlayMode/mario_walk_sprites_left3.png"));  
  MoveLeft.push_back(QPixmap(":/play/PlayMode/mario_walk_sprites_left4.png"));  
  MoveLeft.push_back(QPixmap(":/play/PlayMode/mario_walk_sprites_left5.png"));
  MoveLeft.push_back(QPixmap(":/play/PlayMode/mario_walk_sprites_left6.png"));  
  Animation["moveleft"]=MoveLeft;
  
  //主角向右移动的动画
  vector <QPixmap> MoveRight;
  MoveRight.clear();
  MoveRight.push_back(QPixmap(":/play/PlayMode/mario_walk_sprites1.png"));
  MoveRight.push_back(QPixmap(":/play/PlayMode/mario_walk_sprites2.png"));  
  MoveRight.push_back(QPixmap(":/play/PlayMode/mario_walk_sprites3.png"));  
  MoveRight.push_back(QPixmap(":/play/PlayMode/mario_walk_sprites4.png"));  
  MoveRight.push_back(QPixmap(":/play/PlayMode/mario_walk_sprites5.png"));
  MoveRight.push_back(QPixmap(":/play/PlayMode/mario_walk_sprites6.png"));  
  Animation["moveright"]=MoveRight;
  
  //主角停止的动画
  vector <QPixmap> Stop;
  Stop.clear();
  Stop.push_back(QPixmap(":/play/PlayMode/mario_stop.png"));
  Animation["stop"]=Stop;
  
  //主角死亡的动画
  vector <QPixmap> Die;
  Die.clear();
  Die.push_back(QPixmap(":/play/PlayMode/mario_die.png"));
  Animation["die"]=Die;
    
  Play("stop");
}

//主角的竖直移动受到重力影响
void Hero::makeMoveVertical(){
  setVelocityY(getVelocityY()+Constants::Gravity);
  setPositionY(getPositionY()+getVelocityY());
}

//主角可以被创建
void Hero::CreatableIcon(){
}

//可移动方块
MovableBrick::MovableBrick(GControl* parent, double PX, double PY, int Type):MovableTarget(parent,PX,PY){
  //方块的停止动画
  vector <QPixmap> Stop;
  Stop.clear();
  Stop.push_back(QPixmap(":/play/PlayMode/floor_bottom.jpg"));
  Animation["stop"]=Stop;
  
  Play("stop");
  FindTrackStampFrameVertical=0;
  FindTrackStampFrameHorizontal=0;
  
  if (Type==0) setVelocityY(-Constants::MovableBrickSpeed); //Type为0，方块初始向上移动
  if (Type==1) setVelocityX(Constants::MovableBrickSpeed); //Type为1,方块初始向右移动
  if (Type==2) setVelocityY(Constants::MovableBrickSpeed); //Type为2,方块初始向下移动
  if (Type==3) setVelocityX(-Constants::MovableBrickSpeed); //Type为3，方块初始向左移动
}

//可移动方块的竖直移动需要在一定间隔寻找轨道
void MovableBrick::makeMoveVertical(){
  if (parent->getFrame()-FindTrackStampFrameVertical==Constants::FindTrackGap){//是寻找轨道的帧
    int PosX=getPositionX()/90,PosY=getPositionY()/90;
    StableTarget* ptr=parent->StableBoard[PosX+1][PosY+1];
    if (dynamic_cast<Track*>(ptr)!=nullptr){
      if (dynamic_cast<TrackUp*>(ptr)!=nullptr) setVelocityY(-Constants::MovableBrickSpeed);else
      if (dynamic_cast<TrackDown*>(ptr)!=nullptr) setVelocityY(Constants::MovableBrickSpeed);else
      setVelocityY(0);
    }else
      setVelocityY(0);
    FindTrackStampFrameVertical=parent->getFrame();
  }
  setPositionY(getPositionY()+getVelocityY());
}

//可移动方块的水平移动需要在一定间隔寻找轨道
void MovableBrick::makeMoveHorizontal(){
  if (parent->getFrame()-FindTrackStampFrameHorizontal==Constants::FindTrackGap){//是寻找轨道的帧
    int PosX=getPositionX()/90,PosY=getPositionY()/90;
    StableTarget* ptr=parent->StableBoard[PosX+1][PosY+1];
    if (dynamic_cast<Track*>(ptr)!=nullptr){
      if (dynamic_cast<TrackLeft*>(ptr)!=nullptr) setVelocityX(-Constants::MovableBrickSpeed);else
      if (dynamic_cast<TrackRight*>(ptr)!=nullptr) setVelocityX(Constants::MovableBrickSpeed);else
      setVelocityX(0);
    }else
      setVelocityX(0);
    FindTrackStampFrameHorizontal=parent->getFrame();
  }  
  setPositionX(getPositionX()+getVelocityX());
}

//设置为可创建
void MovableBrick::CreatableIcon(){
}

Monster::Monster(GControl* parent, double PX, double PY):MovableTarget(parent,PX,PY){ 
}

MonsterWanderer::MonsterWanderer(GControl* parent, double PX, double PY):Monster(parent,PX,PY){
  ShootBeginStampFrame=0;
  ShootEndStampFrame=-1e9;  
  setVelocityX(-Constants::MonsterSpeed);
  setVelocityY(0);
  
  //怪物Wanderer向左移动动画
  vector <QPixmap> MoveLeft;
  MoveLeft.clear();
  MoveLeft.push_back(QPixmap(":/play/PlayMode/dark_eater.png"));
  Animation["moveleft"]=MoveLeft;
  
  //怪物Wanderer向右移动动画
  vector <QPixmap> MoveRight;
  MoveRight.clear();
  MoveRight.push_back(QPixmap(":/play/PlayMode/dark_eater_right.png"));
  Animation["moveright"]=MoveRight;
  
  //怪物Wanderer向左发射动画
  vector <QPixmap> ShootLeft;
  ShootLeft.clear();
  ShootLeft.push_back(QPixmap(":/play/PlayMode/dark_eater_fire.png"));
  Animation["shootleft"]=ShootLeft;
  Sound["shootleft"].setSource(QUrl("qrc:/wav/wavaudios/fire.wav"));
  
  //怪物Wanderer向右发射动画
  vector <QPixmap> ShootRight;
  ShootRight.clear();
  ShootRight.push_back(QPixmap(":/play/PlayMode/dark_eater_right_fire.png"));
  Animation["shootright"]=ShootRight;
  Sound["shootright"].setSource(QUrl("qrc:/wav/wavaudios/fire.wav"));
  
  Play("moveleft");
}

//怪物Wanderer水平移动需要判定是否发射
void MonsterWanderer::makeMoveHorizontal(){
  setPositionX(getPositionX()+getVelocityX());
  if (parent->getFrame()-ShootEndStampFrame==Constants::WandererShootGap){//达到了应当发射的帧
    ShootBeginStampFrame=parent->getFrame();
    if (getVelocityX()<0)
      parent->addActivate(new EnemyBullet(parent,getPositionX()-90,getPositionY(),3)); //向左发射子弹
    else
      parent->addActivate(new EnemyBullet(parent,getPositionX()+90,getPositionY(),1)); //向右发射子弹
    Sound["shootleft"].play(); //发出发射的声音
   }
  if (parent->getFrame()-ShootBeginStampFrame==Constants::WandererShootTime){//达到了应当停止发射的帧
    ShootEndStampFrame=parent->getFrame();
  }
  if (ShootEndStampFrame!=-1e9&&ShootBeginStampFrame>ShootEndStampFrame){
    if (getVelocityX()<0) //处在发射状态，按照速度判定发射动画
      PlayAni("shootleft");else
      PlayAni("shootright");
  }else{
    if (getVelocityX()<0) //处在移动状态，按照速度判定移动动画
      PlayAni("moveleft");else
      PlayAni("moveright");
  }
}

//怪物Wanderer竖直方向的移动受重力影响
void MonsterWanderer::makeMoveVertical(){
  setVelocityY(getVelocityY()+Constants::Gravity);
  setPositionY(getPositionY()+getVelocityY());
}

//设置为可被创建
void MonsterWanderer::CreatableIcon(){
}

MonsterChaser::MonsterChaser(GControl* parent, double PX, double PY):Monster(parent,PX,PY){
  //怪物Chaser向左移动的动画
  vector <QPixmap> MoveLeft;
  MoveLeft.clear();
  MoveLeft.push_back(QPixmap(":/play/PlayMode/dark_eater_die.png"));
  Animation["moveleft"]=MoveLeft;
  
  //怪物Chaser向右移动的动画
  vector <QPixmap> MoveRight;
  MoveRight.clear();
  MoveRight.push_back(QPixmap(":/play/PlayMode/dark_eater_die_right.png"));
  Animation["moveright"]=MoveRight;
  
  Play("moveleft");
}

//怪物Chaser水平移动受到与主角相对位置的影响
void MonsterChaser::makeMoveHorizontal(){
  if (parent->getHero()->getPositionX()<getPositionX())
    setVelocityX(-Constants::MonsterSpeed);else//主角在怪物左边
    setVelocityX(Constants::MonsterSpeed);//主角在怪物右边
  if (getVelocityX()<0)
    Play("moveleft");else
    Play("moveright");
  setPositionX(getPositionX()+getVelocityX());
}

//怪物Chaser竖直移动受到重力影响
void MonsterChaser::makeMoveVertical(){
  setVelocityY(getVelocityY()+Constants::Gravity);
  setPositionY(getPositionY()+getVelocityY());
}

//设置为可以创建
void MonsterChaser::CreatableIcon(){
}

MonsterFloater::MonsterFloater(GControl* parent, double PX, double PY):Monster(parent,PX,PY){
  //怪物Floater的移动动画
  vector <QPixmap> Move;
  Move.clear();
  Move.push_back(QPixmap(":/play/PlayMode/fantom_sprite1.png"));
  Move.push_back(QPixmap(":/play/PlayMode/fantom_sprite2.png"));
  Move.push_back(QPixmap(":/play/PlayMode/fantom_sprite3.png"));
  Move.push_back(QPixmap(":/play/PlayMode/fantom_sprite4.png"));
  Animation["move"]=Move; 
  Play("move");
  
  setVelocityX(-Constants::MonsterSpeed);
  setVelocityY(-Constants::MonsterSpeed);
}

//设置为可以被创建
void MonsterFloater::CreatableIcon(){
}

Bullet::Bullet(GControl* parent, double PX, double PY):MovableTarget(parent,PX,PY){
}

AllyBullet::AllyBullet(GControl* parent, double PX, double PY,int type):Bullet(parent,PX,PY){
  if (type==0) setVelocityY(-Constants::BulletSpeed);else //Type为0，向上移动
  if (type==1) setVelocityX(Constants::BulletSpeed);else  //Type为1，向右移动
  if (type==2) setVelocityY(Constants::BulletSpeed);else //Type为2，向下移动
    setVelocityX(-Constants::BulletSpeed); //Type为3，向左移动
  
  //友方子弹的移动动画
  vector<QPixmap> Move;
  Move.clear();
  Move.push_back(QPixmap(":/play/PlayMode/fire_ball.png"));  
  Animation["move"]=Move;
  
  Play("move");
}

//设置为可以被创建
void AllyBullet::CreatableIcon(){
}

EnemyBullet::EnemyBullet(GControl* parent, double PX, double PY,int type):Bullet(parent,PX,PY){
  //敌方子弹的向上移动动画
  vector<QPixmap> MoveUp;
  MoveUp.clear();
  MoveUp.push_back(QPixmap(":/play/PlayMode/missile_up.png"));  
  Animation["moveup"]=MoveUp;
  
  //敌方子弹的向右移动动画
  vector<QPixmap> Moveright;
  Moveright.clear();
  Moveright.push_back(QPixmap(":/play/PlayMode/missile_right.png"));  
  Animation["moveright"]=Moveright;
  
  //敌方子弹的向下移动动画
  vector<QPixmap> MoveDown;
  MoveDown.clear();
  MoveDown.push_back(QPixmap(":/play/PlayMode/missile_down.png"));  
  Animation["movedown"]=MoveDown;
  
  //敌方子弹的向左移动动画
  vector<QPixmap> MoveLeft;
  MoveLeft.clear();
  MoveLeft.push_back(QPixmap(":/play/PlayMode/missile_left.png"));  
  Animation["moveleft"]=MoveLeft;
  
  
  if (type==0) setVelocityY(-Constants::BulletSpeed),Play("moveup");else   //Type为0，向上移动 
  if (type==1) setVelocityX(Constants::BulletSpeed),Play("moveright");else //Type为1，向右移动
  if (type==2) setVelocityY(Constants::BulletSpeed),Play("movedown");else //Type为2，向下移动 
    setVelocityX(-Constants::BulletSpeed),Play("moveleft"); //Type为3，向左移动
}

//设置为可以创建
void EnemyBullet::CreatableIcon(){
}

Collectible::Collectible(GControl* parent, int PX, int PY):StableTarget(parent,PX,PY){
}

Peach::Peach(GControl* parent, int PX, int PY):Collectible(parent,PX,PY){
  //目标的动画
  vector <QPixmap> Stop;
  Stop.clear();
  Stop.push_back(QPixmap(":/play/PlayMode/peach_sprite1.png"));
  Stop.push_back(QPixmap(":/play/PlayMode/peach_sprite2.png"));
  Stop.push_back(QPixmap(":/play/PlayMode/peach_sprite3.png"));
  Stop.push_back(QPixmap(":/play/PlayMode/peach_sprite4.png"));
  Animation["stop"]=Stop;
  
  Play("stop");
}

//设置为可以创建
void Peach::CreatableIcon(){
}

Mushroom::Mushroom(GControl* parent, int PX, int PY):Collectible(parent,PX,PY){
  //蘑菇的静止动画
  vector <QPixmap> Stop;
  Stop.clear();
  Stop.push_back(QPixmap(":/play/PlayMode/champ_gris.png"));
  Animation["stop"]=Stop;
  
  Play("stop"); 
}

//设置为可以创建
void Mushroom::CreatableIcon(){
}

Flower::Flower(GControl* parent, int PX, int PY):Collectible(parent,PX,PY){
  //花的静止动画
  vector <QPixmap> Stop;
  Stop.clear();
  Stop.push_back(QPixmap(":/play/PlayMode/flower.png"));
  Animation["stop"]=Stop;
  
  Play("stop"); 
}

//设置为可以创建
void Flower::CreatableIcon(){
}

Trap::Trap(GControl* parent, int PX, int PY):StableTarget(parent,PX,PY){
}

ActiveTrap::ActiveTrap(GControl* parent, int PX, int PY):Trap(parent,PX,PY){
  //主动陷阱的静止动画
  vector <QPixmap> Stop;
  Stop.clear();
  Stop.push_back(QPixmap(":/play/PlayMode/mysticTree.png"));
  Animation["stop"]=Stop;
  
  //主动陷阱的出发动画
  vector <QPixmap> Activate;
  Activate.clear();
  Activate.push_back(QPixmap(":/play/PlayMode/mysticTree_die.png"));
  Animation["activate"]=Activate;
  
  //设置发射的音效
  Sound["shoot"].setSource(QUrl("qrc:/wav/wavaudios/fire.wav"));
  
  Play("stop");
  LastFireStampFrame=0;
}

//判断主角是否触发主动陷阱
void ActiveTrap::act(){
  if (fabs(parent->getHero()->getPositionY()-(getPositionY()*90-45))<90){//被触发
    Play("activate");//播放被触发动画
    if (parent->getFrame()-LastFireStampFrame>=Constants::ActiveTrapShootGap){//距离上一次触发超过触发间隔
      parent->addActivate(new EnemyBullet(parent,getPositionX()*90-45-90,getPositionY()*90-45,3));//向左发射子弹
      parent->addActivate(new EnemyBullet(parent,getPositionX()*90-45+90,getPositionY()*90-45,1));//向右发射子弹
      LastFireStampFrame=parent->getFrame();
      Sound["shoot"].play();//触发发射音效
    }
  }else{//未被触发
    Play("stop");
  }
}

//设置为可创建
void ActiveTrap::CreatableIcon(){
}

PassiveTrap::PassiveTrap(GControl* parent, int PX, int PY):Trap(parent,PX,PY){
  //被动陷阱的静止动画
  vector <QPixmap> Stop;
  Stop.clear();
  Stop.push_back(QPixmap(":/play/PlayMode/fire.png"));
  Stop.push_back(QPixmap(":/play/PlayMode/fire2.png"));
  Stop.push_back(QPixmap(":/play/PlayMode/fire3.png"));
  Stop.push_back(QPixmap(":/play/PlayMode/fire4.png"));
  Animation["stop"]=Stop;
  
  Play("stop");
}

//设置为可以创建
void PassiveTrap::CreatableIcon(){
} 

Nothing::Nothing(GControl* parent, int PX, int PY):StableTarget(parent,PX,PY){
  //占位方格的透明动画
  vector <QPixmap> Stop;
  Stop.clear();
  Stop.push_back(QPixmap(":/play/PlayMode/blank.png"));
  Animation["stop"]=Stop;
  
  Play("stop");
}

//设置为可以创建
void Nothing::CreatableIcon(){
}

Brick::Brick(GControl* parent, int PX, int PY):StableTarget(parent,PX,PY){
}

Spring::Spring(GControl* parent, int PX, int PY):Brick(parent,PX,PY){
  //弹簧的静止动画
  vector <QPixmap> Stop;
  Stop.clear();
  Stop.push_back(QPixmap(":/play/PlayMode/spring.png"));
  Animation["stop"]=Stop;
  
  //弹簧的触发动画
  vector <QPixmap> Press;
  Press.clear();
  Press.push_back(QPixmap(":/play/PlayMode/spring__pressed.png"));
  Animation["press"]=Press;
  Sound["press"].setSource(QUrl("qrc:/wav/wavaudios/spring.wav"));
  
  Play("stop");
}

//弹簧需要检验是否达到弹起的时间
void Spring::act(){
  if (parent->getFrame()-lastPressedStampFrame>=Constants::SpringBounceBackGap)
    Play("stop");
}

//设置为可以创建
void Spring::CreatableIcon(){
}

NormalBrick::NormalBrick(GControl* parent, int PX, int PY):Brick(parent,PX,PY){
  //普通砖块的静止动画
  vector <QPixmap> Stop;
  Stop.clear();
  Stop.push_back(QPixmap(":/play/PlayMode/floor_uni.png"));
  Animation["stop"]=Stop;
  
  Play("stop");
}

//设置为可以创建
void NormalBrick::CreatableIcon(){
}

DestroyableBrick::DestroyableBrick(GControl* parent, int PX, int PY):Brick(parent,PX,PY){
  //可摧毁方块的静止动画
  vector <QPixmap> Stop;
  Stop.clear();
  Stop.push_back(QPixmap(":/play/PlayMode/floor2.png"));
  Animation["stop"]=Stop;
  
  Play("stop");
}

//设置为可以创建
void DestroyableBrick::CreatableIcon(){
}

Track::Track(GControl* parent, int PX, int PY):StableTarget(parent,PX,PY){
}

TrackUp::TrackUp(GControl* parent, int PX, int PY):Track(parent,PX,PY){
  //向上轨道的静止动画
  vector <QPixmap> Stop;
  Stop.clear();
  Stop.push_back(QPixmap(":/play/PlayMode/rail_up.png"));
  Animation["stop"]=Stop;
  
  Play("stop");
}

//设置为可以创建
void TrackUp::CreatableIcon(){
}

TrackRight::TrackRight(GControl* parent, int PX, int PY):Track(parent,PX,PY){
  //向右轨道的静止动画
  vector <QPixmap> Stop;
  Stop.clear();
  Stop.push_back(QPixmap(":/play/PlayMode/rail_right.png"));
  Animation["stop"]=Stop;
  
  Play("stop");
}

//设置为可以创建
void TrackRight::CreatableIcon(){
}

TrackDown::TrackDown(GControl* parent, int PX, int PY):Track(parent,PX,PY){
  //向下轨道的静止动画
  vector <QPixmap> Stop;
  Stop.clear();
  Stop.push_back(QPixmap(":/play/PlayMode/rail_down.png"));
  Animation["stop"]=Stop;
  
  Play("stop");
}

//设置为可以创建
void TrackDown::CreatableIcon(){
}

TrackLeft::TrackLeft(GControl* parent, int PX, int PY):Track(parent,PX,PY){
  //向左轨道的静止动画
  vector <QPixmap> Stop;
  Stop.clear();
  Stop.push_back(QPixmap(":/play/PlayMode/rail_left.png"));
  Animation["stop"]=Stop;
  
  Play("stop");
}

//设置为可以创建
void TrackLeft::CreatableIcon(){
}

BumpSolver::BumpSolver(GControl* _parent){
  parent=_parent;
}

//T1与T2相互碰撞且T1会推开T2
void BumpSolver::DealOverlapActive(MovableTarget* T1, MovableTarget* T2, int type){
  if (type==0) T2->setPositionY(T1->getPositionY()+90);else//type为0，T1在T2上侧
  if (type==1) T2->setPositionX(T1->getPositionX()-90);else//type为1，T1在T2右侧
  if (type==2) T2->setPositionY(T1->getPositionY()-90);else//type为2，T1在T2下侧
    T2->setPositionX(T1->getPositionX()+90);//type为3，T1在T2左侧
}

//T1与T2相互碰撞且T2会弹开T1
void BumpSolver::DealOverlapPassive(MovableTarget* T1, BaseGameTarget* T2, int type){
  double PX,PY;
  if (dynamic_cast<MovableTarget*>(T2)!=nullptr)//T2是可移动目标
    PX=dynamic_cast<MovableTarget*>(T2)->getPositionX(),PY=dynamic_cast<MovableTarget*>(T2)->getPositionY();//获取T2坐标
  else//T2是不可移动目标
    PX=dynamic_cast<StableTarget*>(T2)->getPositionX()*90-45,PY=dynamic_cast<StableTarget*>(T2)->getPositionY()*90-45;//计算T2实坐标
  if (type==0) T1->setPositionY(PY-90);else//type为0，T1在T2上侧
  if (type==1) T1->setPositionX(PX+90);else//type为1，T1在T2右侧
  if (type==2) T1->setPositionY(PY+90);else//type为2，T1在T2下侧
    T1->setPositionX(PX-90);//type为3，T1在T2左侧
  if (!(type&1)){
    T1->setVelocityY(0);//竖直方向碰撞导致速度归0
    if (type==0&&dynamic_cast<Hero*>(T1)!=nullptr)//主角下方踩到物体
      parent->Jumpable=1;//设置为可以跳跃
  }
}

void BumpSolver::Bump(MovableTarget* T1, BaseGameTarget* T2, int type){
  if ((!T1->Enabled)||(!T2->Enabled)) return;//判断两物体是否仍然存在
  if (dynamic_cast<MovableBrick*>(T1)!=nullptr){//T1是可移动砖块
    if (dynamic_cast<Bullet*>(T2)!=nullptr){
      T2->Enabled=0;//可移动砖块使子弹消失
    }else
    if (dynamic_cast<MovableTarget*>(T2)!=nullptr&&dynamic_cast<MovableBrick*>(T2)==nullptr){
      //T2是非可移动砖块的可移动目标
      MovableTarget* T2M=dynamic_cast<MovableTarget*>(T2);
      DealOverlapActive(T1,T2M,type);//可移动砖块推开T2
      if ((type&1)&&(dynamic_cast<MonsterWanderer*>(T2)!=nullptr||dynamic_cast<MonsterFloater*>(T2)!=nullptr))
        T2M->setVelocityX(-T2M->getVelocityX()); //怪物Wanderer和怪物Floater水平转向
      if (!(type&1)&&(dynamic_cast<MonsterFloater*>(T2)!=nullptr))
        T2M->setVelocityY(-T2M->getVelocityY()); //怪物Floater竖直转向
    }
  }
  if (dynamic_cast<Hero*>(T1)!=nullptr){//T1是主角
    if (dynamic_cast<Brick*>(T2)!=nullptr||dynamic_cast<MovableBrick*>(T2)!=nullptr||dynamic_cast<Monster*>(T2)!=nullptr)
      DealOverlapPassive(T1,T2,type);//撞到怪物或砖块被弹开
    if (dynamic_cast<Monster*>(T2)!=nullptr){
      if (type==0) T2->Enabled=0,parent->killMonster();else//踩到怪物头部消灭怪物
        if (parent->getFrame()-parent->InvisibleFrame>Constants::InvisibleTime)//判断是否处在无敌状态
          parent->getHurtByMonster();//受到伤害
    }
    if (dynamic_cast<EnemyBullet*>(T2)!=nullptr){
      if (parent->getFrame()-parent->InvisibleFrame>Constants::InvisibleTime)//判断是否处在无敌状态
        parent->getHurtByMonster();
      T2->Enabled=0;//敌方子弹消失
    }
    if (dynamic_cast<Peach*>(T2)!=nullptr){
      parent->winGame();//到达目标，获得胜利
    }
    if (dynamic_cast<Mushroom*>(T2)!=nullptr){
      parent->collectMushroom();//捡拾蘑菇
      T2->Enabled=0;//蘑菇消失
    }
    if (dynamic_cast<Flower*>(T2)!=nullptr){
      parent->collectFlower();//捡拾花
      T2->Enabled=0;//花消失
    }
    if (dynamic_cast<Trap*>(T2)!=nullptr){
      if (parent->getFrame()-parent->InvisibleFrame>Constants::InvisibleTime)//判断是否处在无敌状态
        parent->getHurtByTrap();//受到陷阱伤害
    }
    if (dynamic_cast<Spring*>(T2)!=nullptr)
      if (type==0&&T1->getVelocityY()>=0){//从上方触碰弹簧并触发
        T1->setVelocityY(-Constants::SpringVelocity);//获得竖向速度
        dynamic_cast<Spring*>(T2)->lastPressedStampFrame=parent->getFrame();
        dynamic_cast<Spring*>(T2)->Play("press");//弹簧被按下
        parent->Jumpable=0;//主角不可跳跃
    }
  }
  
  if (dynamic_cast<MonsterWanderer*>(T1)!=nullptr){//T1是怪物Wanderer
    if (dynamic_cast<Brick*>(T2)!=nullptr||dynamic_cast<MovableBrick*>(T2)!=nullptr||dynamic_cast<Monster*>(T2)!=nullptr){
      DealOverlapPassive(T1,T2,type);//撞到怪物或砖块被弹开
      if (type&1)
        T1->setVelocityX(-T1->getVelocityX());//水平碰撞导致转向
    } 
    if (dynamic_cast<Spring*>(T2)!=nullptr&&!(type&1)&&T1->getPositionY()<dynamic_cast<Spring*>(T2)->getPositionY()*90-45&&T1->getVelocityY()>=0){//从上方触碰弹簧并触发
      T1->setVelocityY(-Constants::SpringVelocity);//获得竖向速度
      dynamic_cast<Spring*>(T2)->lastPressedStampFrame=parent->getFrame();
      dynamic_cast<Spring*>(T2)->Play("press");//弹簧被按下
    }
    if (dynamic_cast<Hero*>(T2)!=nullptr){
      DealOverlapActive(T1,dynamic_cast<MovableTarget*>(T2),type);//推开主角
      if (parent->getFrame()-parent->InvisibleFrame>Constants::InvisibleTime)//判断主角是否处在无敌状态
        parent->getHurtByMonster();//主角受到怪物伤害
    }
  }
  
  if (dynamic_cast<MonsterChaser*>(T1)!=nullptr){//T1是怪物Chaser
    if (dynamic_cast<Brick*>(T2)!=nullptr||dynamic_cast<MovableBrick*>(T2)!=nullptr||dynamic_cast<Monster*>(T2)!=nullptr)
      DealOverlapPassive(T1,T2,type);//撞到怪物或砖块被弹开
    if (dynamic_cast<Spring*>(T2)!=nullptr&&!(type&1)&&T1->getPositionY()<dynamic_cast<Spring*>(T2)->getPositionY()*90-45&&T1->getVelocityY()>=0){//从上方触碰弹簧并触发
      T1->setVelocityY(-Constants::SpringVelocity);//获得竖向速度
      dynamic_cast<Spring*>(T2)->lastPressedStampFrame=parent->getFrame();
      dynamic_cast<Spring*>(T2)->Play("press");//弹簧被按下
    }
    if (dynamic_cast<Hero*>(T2)!=nullptr){
      DealOverlapActive(T1,dynamic_cast<MovableTarget*>(T2),type);//推开主角
      if (parent->getFrame()-parent->InvisibleFrame>Constants::InvisibleTime)//判断主角是否处在无敌状态
        parent->getHurtByMonster();//主角受到怪物伤害
    }
  }
  
  if (dynamic_cast<MonsterFloater*>(T1)!=nullptr){
    if (dynamic_cast<Brick*>(T2)!=nullptr||dynamic_cast<MovableBrick*>(T2)!=nullptr||dynamic_cast<Monster*>(T2)!=nullptr){ 
      DealOverlapPassive(T1,T2,type);//撞到怪物或砖块被弹开
      //按照碰撞方向确定碰撞后速度
      if (type==0)
        T1->setVelocityY(-Constants::MonsterSpeed);else
      if (type==1)
        T1->setVelocityX(Constants::MonsterSpeed);else
      if (type==2)
        T1->setVelocityY(Constants::MonsterSpeed);else
        T1->setVelocityX(-Constants::MonsterSpeed);
    }
    if (dynamic_cast<Hero*>(T2)!=nullptr){//T2是主角
      DealOverlapActive(T1,dynamic_cast<MovableTarget*>(T2),type);//推开主角
      if (parent->getFrame()-parent->InvisibleFrame>Constants::InvisibleTime)//判断主角是否处于无敌状态
        parent->getHurtByMonster();//主角受到怪物伤害
    }
  }
  
  if (dynamic_cast<AllyBullet*>(T1)!=nullptr){//T1是友方子弹
    if (dynamic_cast<Brick*>(T2)!=nullptr||dynamic_cast<MovableBrick*>(T2)!=nullptr||dynamic_cast<Monster*>(T2)!=nullptr){     
      T1->Enabled=0;//射到砖块或怪物，消失
      if (dynamic_cast<Monster*>(T2))
        T2->Enabled=0,parent->killMonster();//T2是怪物，消灭怪物
      if (dynamic_cast<DestroyableBrick*>(T2)!=nullptr)
        T2->Enabled=0,parent->crackWall();//T2是可破坏砖块，破坏砖块
    }
  }
    
  if (dynamic_cast<EnemyBullet*>(T1)!=nullptr){//T1是地方子弹
    if (dynamic_cast<Brick*>(T2)!=nullptr||dynamic_cast<MovableBrick*>(T2)!=nullptr||dynamic_cast<Monster*>(T2)!=nullptr||dynamic_cast<Hero*>(T2)!=nullptr){
      T1->Enabled=0;//射到砖块或怪物或主角，消失
      if (dynamic_cast<DestroyableBrick*>(T2)!=nullptr)
        T2->Enabled=0,parent->crackWall();//T2是可破坏砖块，破坏砖块
      if (dynamic_cast<Hero*>(T2)!=nullptr)//T2是主角
        if (parent->getFrame()-parent->InvisibleFrame>Constants::InvisibleTime)//判断主角是否处于无敌状态
          parent->getHurtByMonster();//主角受到伤害
    }
  }
}
