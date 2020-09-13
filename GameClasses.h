#ifndef GAMECLASSES_H
#define GAMECLASSES_H

class GControl;
#include "gamecontrol.h"
#include "constants.h"

#include <map>
#include <string>
#include <vector>
using namespace std;
#include <QPixmap>
#include <QMediaPlayer>
#include <QSoundEffect>

//所有游戏对象类的基类
class BaseGameTarget: public QObject{
Q_OBJECT
public:
  BaseGameTarget(GControl*);
  void Play(string); //播放对象的指定动画与声音
  void PlayAni(string); //播放对象的指定动画
  void NextFrame(); //获取对象动画的下一帧
  int Enabled;  //若对象已消失为0，否则为1
  map<string,vector <QPixmap>> Animation; //对象的动画
  map<string,QSoundEffect> Sound; //对象声音
  string CurrentlyPlaying; //当前指定播放的动画
  int PlayedFrame; //当前指定播放的帧
  GControl* parent; //管理该对象的游戏管理器
  const QPixmap& getFrame(); //获取对象动画当前的帧
private:
  virtual void CreatableIcon()=0; //标志当前对象是否可被创建的纯虚函数
};

//所有可移动游戏对象的基类
class MovableTarget:public BaseGameTarget{
  Q_OBJECT  
public:
  MovableTarget(GControl* parent,double _PX,double _PY);
  void setVelocityX(double); //设置X轴速度
  void setVelocityY(double); //设置Y轴速度
  double getPositionX(); //获取X轴坐标
  double getPositionY(); //获取Y轴坐标
  void setPositionX(double); //设置X轴坐标
  void setPositionY(double); //设置Y轴坐标
  double getVelocityX(); //获取X轴速度
  double getVelocityY(); //获取Y轴速度
  virtual void makeMoveHorizontal(); //做出水平移动
  virtual void makeMoveVertical(); //做出竖直移动
private:
  double VelocityX,VelocityY,PositionX,PositionY; //X轴速度，Y轴速度，X轴位置，Y轴位置
};

//所有不可移动对象的基类
class StableTarget:public BaseGameTarget{
  Q_OBJECT  
public:
  StableTarget(GControl* parent,int _PX,int _PY);  
  int getPositionX(); //获取X轴方格数
  int getPositionY(); //获取Y轴方格数
  virtual void act(); //做出响应
private:
  int PositionX,PositionY; //X轴方格数,Y轴方格数
};

//主角
class Hero:public MovableTarget{
  Q_OBJECT  
public:
  Hero(GControl* parent, double,double);
  void makeMoveVertical() override; //竖直移动
private:
  void CreatableIcon() override; //设置为可被创建
};

//可移动的砖块
class MovableBrick:public MovableTarget{
  Q_OBJECT  
public:
  MovableBrick(GControl* parent, double,double,int Type);
  void makeMoveVertical() override;//竖直移动
  void makeMoveHorizontal() override;//水平移动
private:
  int FindTrackStampFrameVertical,FindTrackStampFrameHorizontal;//上一次竖直移动寻找轨道的帧，上一次水平移动寻找轨道的帧
  void CreatableIcon() override;//设置为可被创建
};

//怪物类的基类
class Monster:public MovableTarget{
  Q_OBJECT  
public:
  Monster(GControl* parent, double,double);
};

//怪物Wanderer
class MonsterWanderer:public Monster{
  Q_OBJECT  
public:
  MonsterWanderer(GControl* parent, double,double);
  void makeMoveHorizontal() override; //水平移动
  void makeMoveVertical() override;  //竖直移动 
private:
  int ShootBeginStampFrame; //开始射击的帧
  int ShootEndStampFrame; //结束射击的帧
  void CreatableIcon() override;  //设置为可被创建
};

//怪物Chaser
class MonsterChaser:public Monster{
  Q_OBJECT  
public:
  MonsterChaser(GControl* parent, double,double);
  void makeMoveHorizontal() override; //水平移动
  void makeMoveVertical() override; //竖直移动
private:
  void CreatableIcon() override; //设置为可被创建
};

//怪物Floater
class MonsterFloater:public Monster{
  Q_OBJECT  
public:
  MonsterFloater(GControl* parent, double,double);
private:
  void CreatableIcon() override; //设置为可被创建
};

//所有子弹的基类
class Bullet:public MovableTarget{
  Q_OBJECT  
public:
  Bullet(GControl* parent, double,double);
};

//友方子弹
class AllyBullet:public Bullet{
  Q_OBJECT  
public:
  AllyBullet(GControl* parent, double,double,int);
private:
  void CreatableIcon() override; //设置为可以创建
};

//敌方子弹
class EnemyBullet:public Bullet{
  Q_OBJECT  
public:
  EnemyBullet(GControl* parent, double,double,int);
private:
  void CreatableIcon() override; //设置为可以创建
};

//所有可收集品的基类
class Collectible:public StableTarget{
  Q_OBJECT  
public:
  Collectible(GControl* parent, int,int);
};

//目标
class Peach:public Collectible{
  Q_OBJECT  
public:
  Peach(GControl* parent, int,int);
private:
  void CreatableIcon() override; //设置为可以创建
};

//蘑菇
class Mushroom:public Collectible{
  Q_OBJECT  
public:
  Mushroom(GControl* parent, int,int);
private:
  void CreatableIcon() override; //设置为可以创建
};

//花
class Flower:public Collectible{
  Q_OBJECT  
public:
  Flower(GControl* parent, int,int);
private:
  void CreatableIcon() override; //设置为可以创建
};

//所有陷阱的基类
class Trap:public StableTarget{
  Q_OBJECT  
public:
  Trap(GControl* parent, int,int); 
};

//主动陷阱
class ActiveTrap:public Trap{
  Q_OBJECT  
public:
  ActiveTrap(GControl* parent, int,int);
  void act() override; //处理触发状态的转换
private:
  int LastFireStampFrame; //上一次发射子弹的帧
  void CreatableIcon() override; //设置为可以创建
};

//被动陷阱
class PassiveTrap:public Trap{
  Q_OBJECT  
public:
  PassiveTrap(GControl* parent, int,int);
private:
  void CreatableIcon() override; //设置为可以创建
};

//空白方格
class Nothing:public StableTarget{
  Q_OBJECT  
public:
  Nothing(GControl* parent, int,int);
private:
  void CreatableIcon() override; //设置为可以创建
};

//所有砖块类的基类
class Brick:public StableTarget{
  Q_OBJECT  
public:
  Brick(GControl* parent, int,int);
};

//弹簧
class Spring:public Brick{
  Q_OBJECT  
public:
  Spring(GControl* parent, int,int);
  void act() override; //处理弹起事件
  int lastPressedStampFrame; //上次被按下的帧
private:
  void CreatableIcon() override; //设置为可以创建
};

//普通砖块
class NormalBrick:public Brick{
  Q_OBJECT  
public:
  NormalBrick(GControl* parent, int,int);
private:
  void CreatableIcon(); //设置为可以创建
};

//可破坏砖块
class DestroyableBrick:public Brick{
  Q_OBJECT  
public:
  DestroyableBrick(GControl* parent, int,int);
private:
  void CreatableIcon(); //设置为可以创建
};

//所有轨道的基类
class Track:public StableTarget{
  Q_OBJECT  
public:
  Track(GControl* parent, int,int);
};

//向上的轨道
class TrackUp:public Track{
  Q_OBJECT  
public:
  TrackUp(GControl* parent, int,int);
private:
  void CreatableIcon() override; //设置为可以创建
};

//向右的轨道
class TrackRight:public Track{
  Q_OBJECT  
public:
  TrackRight(GControl* parent, int,int);
private:
  void CreatableIcon() override; //设置为可以创建
};

//向下的轨道
class TrackDown:public Track{
  Q_OBJECT  
public:
  TrackDown(GControl* parent, int,int);
private:
  void CreatableIcon() override; //设置为可以创建
};

//向左的轨道
class TrackLeft:public Track{
  Q_OBJECT  
public:
  TrackLeft(GControl* parent, int,int);
private:
  void CreatableIcon() override; //设置为可以创建
};

//两物体相撞处理器
class BumpSolver
{
public:
  BumpSolver(GControl* _parent);
  void Bump(MovableTarget*,BaseGameTarget*,int); //处理两物体按给定的方向碰撞
private:
  GControl* parent; //管理该对象的游戏管理器
  void DealOverlapActive(MovableTarget*,MovableTarget*,int); //重叠时第一个对象将第二个对象推开
  void DealOverlapPassive(MovableTarget*,BaseGameTarget*,int); //重叠时第一个对象被第二个对象弹开
};
#endif // GAMECLASSES_H
