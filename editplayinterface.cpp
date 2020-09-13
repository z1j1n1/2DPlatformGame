#include "editplayinterface.h"
#include "ui_editplayinterface.h"
#include "windowmanager.h"

EditPlayInterface::EditPlayInterface(QWidget *parent, WindowManager* manager) :
  QDialog(parent),
  ui(new Ui::EditPlayInterface)
{
  ui->setupUi(this);
  m_manager=manager;
  Controller=nullptr;
  QObject::connect(manager,SIGNAL(startEditGame()),this,SLOT(loadGame()));
}

//退回到编辑界面
void EditPlayInterface::switchback(){
  QM->stop(); //停止BGM
  RefreshTimer->stop(); //停止窗口重绘的计时器
  FrameTimer->stop(); //停止记录计算下一帧的计时器
  FinalTime=Timer->elapsed(); //记录最终时间
  FinalScore=Controller->GameScore+10000-Timer->elapsed()/10; //记录最终分数
  m_manager->switchTo(2); //切换窗口到编辑页面
}

//帧数计时器到时对应的槽
void EditPlayInterface::timeout(){
  Controller->getNextFrame();
}

//重新加载地图
void EditPlayInterface::loadGame(){
  //重新创建Controller创建新游戏
  if (Controller!=nullptr) delete Controller;
  Controller=new GControl(this);
  
  //播放BGM
  QM= new QMediaPlayer();
  QM->setMedia(QUrl("qrc:/audio/mario-audios/bgm.mp3"));
  QM->setVolume(20);
  QM->play();
  
  //开始游戏
  Controller->startGame(m_manager->getSelectedMap());
  update();
  
  //设置记录窗口重绘的计时器
  RefreshTimer=new QTimer();
  RefreshTimer->start(50);
  QObject::connect(RefreshTimer,SIGNAL(timeout()),this,SLOT(update()));
  QObject::connect(RefreshTimer,SIGNAL(timeout()),RefreshTimer,SLOT(start()));  
  
  //设置记录计算下一帧的计时器
  FrameTimer=new QTimer();
  FrameTimer->start(50);
  QObject::connect(FrameTimer,SIGNAL(timeout()),this,SLOT(timeout()));
  QObject::connect(FrameTimer,SIGNAL(timeout()),FrameTimer,SLOT(start()));
  
  Timer=new QElapsedTimer();
  Timer->start();
}

EditPlayInterface::~EditPlayInterface()
{
  delete ui;
}


void EditPlayInterface::paintEvent(QPaintEvent* ev){
  QPainter painter(this);
  //绘制背景
  painter.drawPixmap(rect(),QPixmap(":/universal/mario-images/background.png")); 
  
  //绘制不可移动对象
  for (int PosX=1;PosX<=15;PosX++)
    for (int PosY=1;PosY<=10;PosY++)
      if (Controller->StableBoard[PosX][PosY]->Enabled){
        Controller->StableBoard[PosX][PosY]->NextFrame();
        painter.drawPixmap(QRect(PosX*90-90,PosY*90-90,90,90),Controller->StableBoard[PosX][PosY]->getFrame());
      }
  
  //绘制主角，同时判定是否处于无敌状态的闪烁
  Controller->MovableList[0]->NextFrame();
  if (Controller->getFrame()-Controller->InvisibleFrame>=Constants::InvisibleTime||Controller->getFrame()%2)
    painter.drawPixmap(QRect(Controller->MovableList[0]->getPositionX()-45,Controller->MovableList[0]->getPositionY()-45,90,90),Controller->MovableList[0]->getFrame());        
  
  //绘制其他可移动对象
  for (int i=1;i<(int)Controller->MovableList.size();i++)
    if (Controller->MovableList[i]->Enabled){
      Controller->MovableList[i]->NextFrame();
      painter.drawPixmap(QRect(Controller->MovableList[i]->getPositionX()-45,Controller->MovableList[i]->getPositionY()-45,90,90),Controller->MovableList[i]->getFrame());    
    }
  
  QFont font = painter.font();
  font.setPixelSize(48); 
  painter.setFont(font);
  //绘制生命
  for (int i=0;i<Controller->Life;i++)
    painter.drawPixmap(QRect(i*60,0,60,60),QPixmap(":/play/PlayMode/heart.png"));
  if (!Controller->WinFlag){//游戏未结束，绘制时间与分数
    painter.drawText(QRect(500,0,350,60),"Time:"+QString::number(Timer->elapsed()/1000.0,'f',2)+"s");
    painter.drawText(QRect(1000,0,350,60),"score:"+QString::number((int)(Controller->GameScore+10000-Timer->elapsed()/10)));  
  }else{//游戏已结束，绘制时间与分数为Finished
    painter.drawText(QRect(500,0,350,60),"Time:Finished");
    painter.drawText(QRect(1000,0,350,60),"score:Finished");      
  }
}

//键盘按键事件
void EditPlayInterface::keyPressEvent(QKeyEvent *ev){
  if (ev->key()==Qt::Key_A)
  if (!ev->isAutoRepeat()){//按下A
    Controller->APressed=1;
  } 
  if (ev->key()==Qt::Key_D)
  if (!ev->isAutoRepeat()){//按下D
    Controller->DPressed=1;
  } 
  if (Controller->APressed^Controller->DPressed){//A与D中只有一个键按下
    if (Controller->APressed) Controller->getHero()->Play("moveleft"),Controller->getHero()->setVelocityX(-Constants::HeroMoveSpeed);else//向左移动
      Controller->getHero()->Play("moveright"),Controller->getHero()->setVelocityX(Constants::HeroMoveSpeed);//向右移动
  }else
    Controller->getHero()->Play("stop"),Controller->getHero()->setVelocityX(0);//A与D按下0个或2个，不移动
  if (ev->key()==Qt::Key_W)
    if (Controller->Jumpable&&fabs(Controller->getHero()->getVelocityY())<Constants::JumpLimit){//可以跳跃并且下坠速度在阈值内
      Controller->Jumpable=0;//设置为不可跳跃
      Controller->getHero()->setVelocityY(-Constants::HeroJumpVelocity);//设置跳跃速度
      Controller->getHero()->Sound["jump"].play();//播放跳跃音效
    }
  if (ev->key()==Qt::Key_I)
    if (Controller->getFrame()-Controller->FirePressed>=Constants::HeroFireGap)//未受到开火间隔限制
    if (Controller->Shootable){//可以发射子弹
      Controller->FirePressed=Controller->getFrame();
      Controller->Sounds["fire"].play();//播放开火声音
      Controller->addActivate(new AllyBullet(Controller,Controller->getHero()->getPositionX(),Controller->getHero()->getPositionY()-30,0));//增加子弹
    }
  if (ev->key()==Qt::Key_L)
    if (Controller->getFrame()-Controller->FirePressed>=Constants::HeroFireGap)//未受到开火间隔限制
    if (Controller->Shootable){//可以发射子弹
      Controller->FirePressed=Controller->getFrame();
      Controller->Sounds["fire"].play();//播放开火声音
      Controller->addActivate(new AllyBullet(Controller,Controller->getHero()->getPositionX()+30,Controller->getHero()->getPositionY(),1));//增加子弹
    }
  if (ev->key()==Qt::Key_K)
    if (Controller->getFrame()-Controller->FirePressed>=Constants::HeroFireGap)//未受到开火间隔限制
    if (Controller->Shootable){//可以发射子弹
      Controller->FirePressed=Controller->getFrame();
      Controller->Sounds["fire"].play();//播放开火声音
      Controller->addActivate(new AllyBullet(Controller,Controller->getHero()->getPositionX(),Controller->getHero()->getPositionY()+30,2));//增加子弹
    }
  if (ev->key()==Qt::Key_J)
    if (!ev->isAutoRepeat())
      if (Controller->getFrame()-Controller->FirePressed>=Constants::HeroFireGap)//未受到开火间隔限制
    if (Controller->Shootable){//可以发射子弹
      Controller->FirePressed=Controller->getFrame();
      Controller->Sounds["fire"].play();//播放开火声音
      Controller->addActivate(new AllyBullet(Controller,Controller->getHero()->getPositionX()-30,Controller->getHero()->getPositionY(),3));//增加子弹
    }
  if (ev->key()==Qt::Key_Escape){//按下Esc放弃游戏
    Controller->forfeitGame();
  }
}

void EditPlayInterface::keyReleaseEvent(QKeyEvent *ev){
  if (ev->key()==Qt::Key_A)
   if (!ev->isAutoRepeat()){//松开A
     Controller->APressed=0;
  }
  if (ev->key()==Qt::Key_D)
    if (!ev->isAutoRepeat()){//松开D
      Controller->DPressed=0;
  }
  if (Controller->APressed^Controller->DPressed){//A与D中只有一个键按下
    if (Controller->APressed) Controller->getHero()->Play("moveleft"),Controller->getHero()->setVelocityX(-Constants::HeroMoveSpeed);else//向左移动
      Controller->getHero()->Play("moveright"),Controller->getHero()->setVelocityX(Constants::HeroMoveSpeed);//向右移动
  }else
    Controller->getHero()->Play("stop"),Controller->getHero()->setVelocityX(0);//A与D按下0个或2个，不移动
}

void EditPlayInterface::win(){
  //弹出统计姓名对话框
  EditLeaderBoardDialog* Dialog=new EditLeaderBoardDialog(Controller->GameScore+10000-Timer->elapsed()/10,this);
  if (Dialog->exec()==QDialog::Accepted){
    //这是编辑转到的游玩模式，并不记录积分榜
  }
}
