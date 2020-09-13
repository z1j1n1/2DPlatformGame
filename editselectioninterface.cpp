#include "editselectioninterface.h"
#include "ui_editselectioninterface.h"

EditSelectionInterface::EditSelectionInterface(QWidget *parent, QObject* manager) :
  QDialog(parent),
  ui(new Ui::EditSelectionInterface)
{
  ui->setupUi(this);
  
  //加载缩略图
  reloadImgs();
  //连接窗口管理器要求重新加载缩略图与加载缩略图
  QObject::connect(manager,SIGNAL(reloadImgs()),this,SLOT(reloadImgs()));
  
  //连接窗口切换要求
  QSignalMapper* WindowConversion =new QSignalMapper(this);
  QObject::connect(ui->ButtonQuit,SIGNAL(clicked()),WindowConversion,SLOT(map()));
  WindowConversion->setMapping(ui->ButtonQuit,0);
  QObject::connect(WindowConversion,SIGNAL(mapped(int)),manager,SLOT(switchTo(int)));
  
  QObject::connect(ui->Button1,SIGNAL(clicked()),WindowConversion,SLOT(map()));
  WindowConversion->setMapping(ui->Button1,2);
  QObject::connect(ui->Button2,SIGNAL(clicked()),WindowConversion,SLOT(map()));
  WindowConversion->setMapping(ui->Button2,2);
  QObject::connect(ui->Button3,SIGNAL(clicked()),WindowConversion,SLOT(map()));
  WindowConversion->setMapping(ui->Button3,2);
  QObject::connect(ui->Button4,SIGNAL(clicked()),WindowConversion,SLOT(map()));
  WindowConversion->setMapping(ui->Button4,2);
  QObject::connect(ui->Button5,SIGNAL(clicked()),WindowConversion,SLOT(map()));
  WindowConversion->setMapping(ui->Button5,2);
  QObject::connect(ui->Button6,SIGNAL(clicked()),WindowConversion,SLOT(map()));
  WindowConversion->setMapping(ui->Button6,2);
  QObject::connect(ui->Button7,SIGNAL(clicked()),WindowConversion,SLOT(map()));
  WindowConversion->setMapping(ui->Button7,2);
  QObject::connect(ui->Button8,SIGNAL(clicked()),WindowConversion,SLOT(map()));
  WindowConversion->setMapping(ui->Button8,2);
  QObject::connect(ui->Button9,SIGNAL(clicked()),WindowConversion,SLOT(map()));
  WindowConversion->setMapping(ui->Button9,2);
  QObject::connect(ui->Button10,SIGNAL(clicked()),WindowConversion,SLOT(map()));
  WindowConversion->setMapping(ui->Button10,2);
  
  //连接选择地图按钮
  QSignalMapper* MapSelectConversion=new QSignalMapper(this);
  QObject::connect(ui->Button1,SIGNAL(clicked()),MapSelectConversion,SLOT(map()));
  MapSelectConversion->setMapping(ui->Button1,1);
  QObject::connect(ui->Button2,SIGNAL(clicked()),MapSelectConversion,SLOT(map()));
  MapSelectConversion->setMapping(ui->Button2,2);
  QObject::connect(ui->Button3,SIGNAL(clicked()),MapSelectConversion,SLOT(map()));
  MapSelectConversion->setMapping(ui->Button3,3);
  QObject::connect(ui->Button4,SIGNAL(clicked()),MapSelectConversion,SLOT(map()));
  MapSelectConversion->setMapping(ui->Button4,4);
  QObject::connect(ui->Button5,SIGNAL(clicked()),MapSelectConversion,SLOT(map()));
  MapSelectConversion->setMapping(ui->Button5,5);
  QObject::connect(ui->Button6,SIGNAL(clicked()),MapSelectConversion,SLOT(map()));
  MapSelectConversion->setMapping(ui->Button6,6);
  QObject::connect(ui->Button7,SIGNAL(clicked()),MapSelectConversion,SLOT(map()));
  MapSelectConversion->setMapping(ui->Button7,7);
  QObject::connect(ui->Button8,SIGNAL(clicked()),MapSelectConversion,SLOT(map()));
  MapSelectConversion->setMapping(ui->Button8,8);
  QObject::connect(ui->Button9,SIGNAL(clicked()),MapSelectConversion,SLOT(map()));
  MapSelectConversion->setMapping(ui->Button9,9);
  QObject::connect(ui->Button10,SIGNAL(clicked()),MapSelectConversion,SLOT(map()));
  MapSelectConversion->setMapping(ui->Button10,10);
  QObject::connect(MapSelectConversion,SIGNAL(mapped(int)),manager,SLOT(SelectMap(int)));
 }

EditSelectionInterface::~EditSelectionInterface()
{
  delete ui;
}

void EditSelectionInterface::paintEvent(QPaintEvent* ev){
  QPainter painter(this);
  //绘制背景
  painter.drawPixmap(rect(),QPixmap(":/universal/mario-images/background.png"));
  QFont font = painter.font();
  font.setPixelSize(48);
  painter.setFont(font);
  //绘制文字
  painter.drawText(QRect(0,0,1350,200),Qt::AlignCenter,"请选择想要修改的地图");
}

//重新加载按钮上的缩略图
void EditSelectionInterface::reloadImgs(){
  ui->Button1->setIcon(QPixmap("./maps/1.png").scaled(ui->Button1->rect().size()));
  ui->Button2->setIcon(QPixmap("./maps/2.png").scaled(ui->Button2->rect().size()));  
  ui->Button3->setIcon(QPixmap("./maps/3.png").scaled(ui->Button3->rect().size()));
  ui->Button4->setIcon(QPixmap("./maps/4.png").scaled(ui->Button4->rect().size()));
  ui->Button5->setIcon(QPixmap("./maps/5.png").scaled(ui->Button5->rect().size()));
  ui->Button6->setIcon(QPixmap("./maps/6.png").scaled(ui->Button6->rect().size()));
  ui->Button7->setIcon(QPixmap("./maps/7.png").scaled(ui->Button7->rect().size()));
  ui->Button8->setIcon(QPixmap("./maps/8.png").scaled(ui->Button8->rect().size()));
  ui->Button9->setIcon(QPixmap("./maps/9.png").scaled(ui->Button9->rect().size()));
  ui->Button10->setIcon(QPixmap("./maps/10.png").scaled(ui->Button10->rect().size()));  
}
