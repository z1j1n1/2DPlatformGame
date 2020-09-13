#include "maininterface.h"
#include "ui_maininterface.h"

extern WindowManager w;

MainInterface::MainInterface(QWidget *parent, QObject* manager)
  : QDialog(parent)
  , ui(new Ui::MainInterface)
{
  ui->setupUi(this);
  //设置加载动画
  ui->label->setPixmap(QPixmap(":/universal/mario-images/loading.jpg").scaled(ui->label->rect().size()));
  QGraphicsOpacityEffect *pButtonOpacity = new QGraphicsOpacityEffect(this);
  pButtonOpacity->setOpacity(1);
  ui->label->setGraphicsEffect(pButtonOpacity);
  //透明度动画
  QPropertyAnimation *pOpacityAnimation = new QPropertyAnimation(pButtonOpacity, "opacity");
  pOpacityAnimation->setDuration(1000);
  pOpacityAnimation->setStartValue(1);
  pOpacityAnimation->setEndValue(0);
  //平移动画，防止label覆盖按钮
  QPropertyAnimation *pPosAnimation1 = new QPropertyAnimation(ui->label, "pos");
  pPosAnimation1->setDuration(1);
  pPosAnimation1->setStartValue(QPoint(0,0));
  pPosAnimation1->setEndValue(QPoint(-1350,0));
  pPosAnimation1->setEasingCurve(QEasingCurve::InOutQuad);
  //播放动画
  QSequentialAnimationGroup *pPosGroup = new QSequentialAnimationGroup(this);
  pPosGroup->addPause(2000);
  pPosGroup->addAnimation(pOpacityAnimation);
  pPosGroup->addAnimation(pPosAnimation1);
  pPosGroup->start();
  
  //设置按钮的窗口转换功能
  QSignalMapper* WindowConversion =new QSignalMapper(this);
  QObject::connect(ui->ButtonEdit,SIGNAL(clicked()),WindowConversion,SLOT(map()));
  WindowConversion->setMapping(ui->ButtonEdit,1);
  QObject::connect(ui->ButtonPlay,SIGNAL(clicked()),WindowConversion,SLOT(map()));
  WindowConversion->setMapping(ui->ButtonPlay,3);
  QObject::connect(ui->ButtonBoard,SIGNAL(clicked()),WindowConversion,SLOT(map()));
  WindowConversion->setMapping(ui->ButtonBoard,6);
  
  QObject::connect(ui->ButtonQuit,SIGNAL(clicked()),this,SLOT(close()));
  QObject::connect(WindowConversion,SIGNAL(mapped(int)),manager,SLOT(switchTo(int)));
}

MainInterface::~MainInterface()
{
  delete ui;
}

void MainInterface::paintEvent(QPaintEvent* ev){
  QPainter painter(this);
  //绘制背景
  painter.drawPixmap(rect(),QPixmap(":/universal/mario-images/background.png"));
  //绘制Go图案
  painter.drawPixmap(QRect(400,50,550,250),QPixmap(":/universal/mario-images/go2.png"));
} 
