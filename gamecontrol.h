#ifndef GControl_H
#define GControl_H

#include "GameClasses.h"
#include "constants.h"

class Hero;
class MovableTarget;
class StableTarget;
class BaseGameTarget;
class BumpSolver;

#include <QWidget>
#include <QFile>
#include <string>
#include <vector>
#include <QElapsedTimer>
#include <cmath>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QPaintEvent>
#include <QPainter>
#include <QSoundEffect>
using namespace std;

//游戏管理器
class GControl : public QObject
{
  Q_OBJECT
public:
  explicit GControl(QWidget *parent = nullptr);
  ~GControl();
  
  vector <MovableTarget*> MovableList; //可移动对象的列表
  StableTarget* StableBoard[47][32]; //不可移动对象的数组
  vector <BaseGameTarget*> MemoryPool; //所有被创建对象的指针列表
  map <string,QSoundEffect> Sounds; //音效
  int Jumpable,Shootable,InvisibleFrame,Life,WinFlag,Frame,GameScore; //主角是否可跳跃,主角是否可射击,主角上一次受伤的帧，主角的生命，游戏是否结束，当前帧，游戏的得分(不计时间)
  int APressed,DPressed,FirePressed; //A是否被按下,D是否被按下,开火是否被按下
  BumpSolver* BumpRule; //碰撞处理器指针
  QWidget* m_parent; //父亲指针
  
  int getFrame(); //获取当前帧
  void addActivate(MovableTarget*); //添加可移动对象
  void addStable(StableTarget*,int,int); //添加不可移动对象
  void killMonster(); //处理击杀怪物
  void getHurtByMonster(); //处理受到怪物伤害
  void winGame(); //处理游戏胜利
  void forfeitGame(); //处理放弃游戏
  void loseGame(); //处理游戏失败
  void collectMushroom(); //处理捡拾蘑菇
  void collectFlower(); //处理捡拾花
  void getHurtByTrap(); //处理受到陷阱伤害
  void crackWall(); //处理墙壁破裂
  void getNextFrame(); //计算下一帧
  void startGame(int SelectedMap); //载入指定地图
  Hero* getHero(); //获取主角指针
signals:
  void quit(); //退出
  void FrameCalculated(); //帧计算完毕
  void win(); //胜利
};

#endif // GControl_H
