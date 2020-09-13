#include "gamecontrol.h"
#include "gameinterface.h"

int GControl::getFrame(){
  return(Frame);
}

//添加可移动对象
void GControl::addActivate(MovableTarget* Target){
  MemoryPool.push_back(dynamic_cast<BaseGameTarget*>(Target)); //加入被创建对象的指针列表
  MovableList.push_back(Target);//加入可移动对象列表
}

//添加不可移动对象
void GControl::addStable(StableTarget* Target, int PX, int PY){
  MemoryPool.push_back(dynamic_cast<BaseGameTarget*>(Target)); //加入被创建对象的指针列表
  StableBoard[PX][PY]=Target; //加入不可移动对象数组
}

//击杀怪物
void GControl::killMonster(){
  Sounds["kill"].play(); //播放击杀音效
  GameScore+=5000; //获得得分
}

//受到怪物伤害
void GControl::getHurtByMonster(){
  Sounds["hurtbymonster"].play(); //播放声音
  GameScore-=3000; //失去游戏分数
  InvisibleFrame=getFrame(); //进入无敌状态
  if (--Life==0) loseGame(); //若生命为0，游戏失败
}

//游戏胜利
void GControl::winGame(){
  if (WinFlag) return; //确保只进行一次
  WinFlag=1;
  Sounds["win"].play();
  emit win();
  emit quit();
}

//放弃游戏
void GControl::forfeitGame(){
  if (WinFlag) return; //确保只进行一次
  WinFlag=1;
  Sounds["lose"].play();
  QMessageBox message(QMessageBox::NoIcon,"", "你放弃了", QMessageBox::Ok);
  message.exec();
  emit quit();
}

//游戏失败
void GControl::loseGame(){
  if (WinFlag) return; //确保只进行一次
  WinFlag=1;
  getHero()->Play("die");
  Sounds["lose"].play();
  QMessageBox message(QMessageBox::NoIcon,"", "你输了", QMessageBox::Ok);
  message.exec();
  emit quit();
}

//收集蘑菇
void GControl::collectMushroom(){
  Life++; //获得生命
  GameScore+=2000; //游戏得分增加
  Sounds["getmushroom"].play(); //播放声音
}

//收集花
void GControl::collectFlower(){
  Life++; //获得生命
  GameScore+=3000; //游戏得分增加
  Shootable=1; //可以开火
  Sounds["getflower"].play(); //播放声音
}

//受到陷阱伤害
void GControl::getHurtByTrap(){
  Sounds["hurtbytrap"].play(); //播放声音
  GameScore-=3000; //失去游戏得分
  InvisibleFrame=getFrame(); //进入无敌状态
  if (--Life==0) loseGame(); //如果生命为0游戏失败
}

//墙壁裂开
void GControl::crackWall(){
  Sounds["crack"].play(); //播放声音
}

//获取英雄
Hero* GControl::getHero(){
  return(dynamic_cast<Hero*>(MovableList[0]));//英雄一定在可移动列表的第一个
}

//计算下一帧
void GControl::getNextFrame(){
  if (WinFlag) return;
  int DifXList[9]={0,0,-1,1,0,-1,-1,1,1};
  int DifYList[9]={0,1,0,0,-1,-1,1,-1,1};
  for (int i=0;i<(int)MovableList.size();i++)
    if (MovableList[i]->Enabled&&!WinFlag){//判定可移动列表第i位的碰撞
      MovableList[i]->makeMoveHorizontal();//可移动列表第i位水平移动
      for (int j=0;j<(int)MovableList.size();j++)
        if (MovableList[j]->Enabled&&i!=j&&!WinFlag)
          if (fabs(MovableList[i]->getPositionX()-MovableList[j]->getPositionX())<90&&
              fabs(MovableList[i]->getPositionY()-MovableList[j]->getPositionY())<90){//可移动列表第i位与可移动列表第j位碰撞
            if (MovableList[i]->getVelocityX()>0)//判断碰撞方向并交由BumpRule处理
              BumpRule->Bump(MovableList[i],MovableList[j],3);else
              BumpRule->Bump(MovableList[i],MovableList[j],1);
          }
      //判定与不可移动目标的碰撞
      int PosX=MovableList[i]->getPositionX()/90+1,PosY=MovableList[i]->getPositionY()/90+1;
      for (int k=0;k<9;k++){//枚举物体中心所在方块的周围
        int DifX=DifXList[k],DifY=DifYList[k];
        if (!WinFlag&&PosX+DifX>=0&&PosX+DifX<=16&&PosY+DifY>=0&&PosY+DifY<=11){
          if (fabs(MovableList[i]->getPositionX()-((PosX+DifX)*90-45))<90&&
             fabs(MovableList[i]->getPositionY()-((PosY+DifY)*90-45))<90){//发生碰撞
            if (MovableList[i]->getVelocityX()>0)//判断碰撞方向并交由BumpRule处理
              BumpRule->Bump(MovableList[i],StableBoard[PosX+DifX][PosY+DifY],3);else
              BumpRule->Bump(MovableList[i],StableBoard[PosX+DifX][PosY+DifY],1);
          }   
        }
      }
          
      MovableList[i]->makeMoveVertical();//可移动列表第i位竖直移动
      PosX=MovableList[i]->getPositionX()/90+1,PosY=MovableList[i]->getPositionY()/90+1;
      for (int j=0;j<(int)MovableList.size();j++)
        if (MovableList[j]->Enabled&&i!=j&&!WinFlag)
          if (fabs(MovableList[i]->getPositionX()-MovableList[j]->getPositionX())<90&&
              fabs(MovableList[i]->getPositionY()-MovableList[j]->getPositionY())<90){//可移动列表第i位与可移动列表第j位碰撞
            if (MovableList[i]->getVelocityY()>0)//判断碰撞方向并交由BumpRule处理
              BumpRule->Bump(MovableList[i],MovableList[j],0);else
              BumpRule->Bump(MovableList[i],MovableList[j],2);
          }
      //判定与不可移动目标的碰撞      
      for (int k=0;k<9;k++){//枚举物体中心所在方块的周围
        int DifX=DifXList[k],DifY=DifYList[k];        
        if (!WinFlag&&PosX+DifX>=0&&PosX+DifX<=16&&PosY+DifY>=0&&PosY+DifY<=11){
          if (fabs(MovableList[i]->getPositionX()-((PosX+DifX)*90-45))<90&&
              fabs(MovableList[i]->getPositionY()-((PosY+DifY)*90-45))<90){//发生碰撞
            if (MovableList[i]->getVelocityY()>0)//判断碰撞方向并交由BumpRule处理
              BumpRule->Bump(MovableList[i],StableBoard[PosX+DifX][PosY+DifY],0);else
              BumpRule->Bump(MovableList[i],StableBoard[PosX+DifX][PosY+DifY],2);
          }
        }
     }
  }
  
  //不可移动目标做出响应
  if (!WinFlag)
  for (int i=1;i<=15;i++)
    for (int j=1;j<=10;j++)
      StableBoard[i][j]->act(); 
  
  //清除无效的可移动目标
  vector <MovableTarget*> NewList;
  NewList.clear();
  for (int i=0;i<(int)MovableList.size();i++)
    if (MovableList[i]->Enabled)
      NewList.push_back(MovableList[i]);
  MovableList=NewList;
  Frame++;
}

GControl::GControl(QWidget *parent) : QObject(parent){
  QObject::connect(this,SIGNAL(quit()),parent,SLOT(switchback()));
  QObject::connect(this,SIGNAL(win()),parent,SLOT(win()));
  BumpRule=new BumpSolver(this);
  m_parent=parent;
}

GControl::~GControl(){
  //释放所有资源
  for (int i=0;i<(int)MemoryPool.size();i++)
    delete MemoryPool[i];
}

void GControl::startGame(int SelectedMap){
  //加载音效
  Sounds["crack"].setSource(QUrl("qrc:/wav/wavaudios/crack.wav"));
  Sounds["hurtbytrap"].setSource(QUrl("qrc:/wav/wavaudios/steponfire.wav"));
  Sounds["getflower"].setSource(QUrl("qrc:/wav/wavaudios/collectflower.wav"));
  Sounds["getmushroom"].setSource(QUrl("qrc:/wav/wavaudios/collectmushroom.wav"));  
  Sounds["win"].setSource(QUrl("qrc:/wav/wavaudios/win.wav"));
  Sounds["lose"].setSource(QUrl("qrc:/wav/wavaudios/lose.wav"));
  Sounds["kill"].setSource(QUrl("qrc:/wav/wavaudios/killmonster.wav"));  
  Sounds["hurtbymonster"].setSource(QUrl("qrc:/wav/wavaudios/hitymonster.wav"));
  Sounds["fire"].setSource(QUrl("qrc:/wav/wavaudios/fire.wav"));
  APressed=0;
  DPressed=0;
  FirePressed=0;
  
  //从文件读入地图
  QString Temp="./maps/";
  int SM=SelectedMap;
  if (SM<10) Temp+=QChar(SM+'0');else Temp+="10";
  Temp+=".txt";
  QFile* file=new QFile(Temp);
  file->open(QIODevice::ReadWrite);
  string StringMap=QString(file->readAll()).toStdString();
  file->close();
  int CurrentPostion=0;
  //按照地图信息创建游戏对象
  for (int PosY=1;PosY<=10;PosY++)
    for (int PosX=1;PosX<=15;PosX++){
      int type=10*(StringMap[CurrentPostion]-'0')+StringMap[CurrentPostion+1]-'0';
      Hero* tmp;
      if (type==1)
        tmp=new Hero(this,PosX*90-45,PosY*90-45);        
      CurrentPostion+=2;
      if (type<0||type>20) 
        qDebug() << "Error File";
      switch (type) {
      case 0:
        addStable(new Nothing(this,PosX,PosY),PosX,PosY);
        break;
      case 1:
        MovableList.insert(MovableList.begin(),tmp);        
        MemoryPool.push_back(dynamic_cast<BaseGameTarget*>(tmp));
        addStable(new Nothing(this,PosX,PosY),PosX,PosY);        
        break;
      case 2:
        addStable(new Peach(this,PosX,PosY),PosX,PosY);
        break;
      case 3:
        addStable(new TrackRight(this,PosX,PosY),PosX,PosY);
        break;
      case 4:
        addStable(new TrackDown(this,PosX,PosY),PosX,PosY);
        break;
      case 5:
        addStable(new TrackUp(this,PosX,PosY),PosX,PosY);
        break;
      case 6:
        addStable(new TrackLeft(this,PosX,PosY),PosX,PosY);
        break;
      case 7:
        addActivate(new MovableBrick(this,PosX*90-45,PosY*90-45,1));
        addStable(new Nothing(this,PosX,PosY),PosX,PosY);        
        break;
      case 8:
        addActivate(new MovableBrick(this,PosX*90-45,PosY*90-45,2));
        addStable(new Nothing(this,PosX,PosY),PosX,PosY);        
        break;
      case 9:
        addActivate(new MovableBrick(this,PosX*90-45,PosY*90-45,0));
        addStable(new Nothing(this,PosX,PosY),PosX,PosY);        
        break;
      case 10:
        addActivate(new MovableBrick(this,PosX*90-45,PosY*90-45,3));
        addStable(new Nothing(this,PosX,PosY),PosX,PosY);        
        break;
      case 11:
        addActivate(new MonsterWanderer(this,PosX*90-45,PosY*90-45));
        addStable(new Nothing(this,PosX,PosY),PosX,PosY);        
        break;
      case 12:
        addActivate(new MonsterChaser(this,PosX*90-45,PosY*90-45));
        addStable(new Nothing(this,PosX,PosY),PosX,PosY);        
        break;
      case 13:
        addActivate(new MonsterFloater(this,PosX*90-45,PosY*90-45));
        addStable(new Nothing(this,PosX,PosY),PosX,PosY);        
        break;
      case 14:
        addStable(new ActiveTrap(this,PosX,PosY),PosX,PosY);
        break;
      case 15:
        addStable(new PassiveTrap(this,PosX,PosY),PosX,PosY);
        break;
      case 16:
        addStable(new Mushroom(this,PosX,PosY),PosX,PosY);
        break;
      case 17:
        addStable(new Flower(this,PosX,PosY),PosX,PosY);
        break;
      case 18:
        addStable(new NormalBrick(this,PosX,PosY),PosX,PosY);
        break;
      case 19:
        addStable(new DestroyableBrick(this,PosX,PosY),PosX,PosY);
        break;
      case 20:
        addStable(new Spring(this,PosX,PosY),PosX,PosY);
        break;
      }
    }
  
  //在地图周围添加普通砖块
  for (int i=0;i<=16;i++)
    addStable(new NormalBrick(this,i,0),i,0),
    addStable(new NormalBrick(this,i,11),i,11);    
  for (int i=1;i<=10;i++)
    addStable(new NormalBrick(this,0,i),0,i),
    addStable(new NormalBrick(this,16,i),16,i);    
  
  //初始化变量
  WinFlag=0;
  Jumpable=0;
  Shootable=0;
  InvisibleFrame=-1000;
  Life=1;
  Frame=0;
  GameScore=0;
}
