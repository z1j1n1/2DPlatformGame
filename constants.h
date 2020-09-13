#ifndef CONSTANTS_H
#define CONSTANTS_H

//程序运行所需的常数
class Constants{
public:
  static const double Gravity; //重力
  static const double MovableBrickSpeed; //可移动方块的速度
  static const double FindTrackGap; //可移动方块寻找轨道的间隔(帧数)
  static const double WandererShootTime; //怪物Wanderer射击的时间(帧数)
  static const double WandererShootGap; //怪物Wanderer射击的间隔(帧数)
  static const double MonsterSpeed; //怪物移动的速度
  static const double BulletSpeed; //子弹移动的速度
  static const double ActiveTrapShootGap; //主动陷阱射击的间隔
  static const double SpringBounceBackGap; //弹簧弹起的间隔(帧数)
  static const double HeroJumpVelocity; //主角跳起获得的初速度
  static const double InvisibleTime; //受伤后的无敌时间(帧数)
  static const double JumpLimit; //下落后仍可起跳的阈值
  static const double HeroMoveSpeed; //英雄移动速度
  static const double SpringVelocity; //弹簧给物体的初速度
  static const double HeroFireGap; //英雄发射子弹的间隔(帧数)
};
#endif // CONSTANTS_H
