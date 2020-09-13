#include "editpaintboard.h"
#include "ui_editpaintboard.h"

EditPaintBoard::EditPaintBoard(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::EditPaintBoard)
{
  ui->setupUi(this);
  //连接保存与运行
  QObject::connect(ui->pushButtonSave,SIGNAL(clicked()),parent,SLOT(save()));
  QObject::connect(ui->pushButtonRun,SIGNAL(clicked()),parent,SLOT(run()));
  
  //连接修改类型变更
  QObject::connect(ui->radioButtonDot,SIGNAL(toggled(bool)),parent,SLOT(setEditTypeDot(bool)));
  QObject::connect(ui->radioButtonRect,SIGNAL(toggled(bool)),parent,SLOT(setEditTypeRect(bool)));
  
  QSignalMapper* BanButton =new QSignalMapper(this);
  //连接选中时的不可选效果
  QObject::connect(BanButton,SIGNAL(mapped(int)),this,SLOT(enableExcept(int)));
  //连接修改方块种类设置
  QObject::connect(BanButton,SIGNAL(mapped(int)),parent,SLOT(setBrush(int)));
  QObject::connect(ui->pushButton0,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton0,0);
  QObject::connect(ui->pushButton1,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton1,1);
  QObject::connect(ui->pushButton2,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton2,2);
  QObject::connect(ui->pushButton3,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton3,3);
  QObject::connect(ui->pushButton4,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton4,4);
  QObject::connect(ui->pushButton5,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton5,5);
  QObject::connect(ui->pushButton6,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton6,6);
  QObject::connect(ui->pushButton7,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton7,7);
  QObject::connect(ui->pushButton8,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton8,8);
  QObject::connect(ui->pushButton9,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton9,9);
  QObject::connect(ui->pushButton10,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton10,10);
  QObject::connect(ui->pushButton11,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton11,11);
  QObject::connect(ui->pushButton12,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton12,12);
  QObject::connect(ui->pushButton13,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton13,13);
  QObject::connect(ui->pushButton14,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton14,14);
  QObject::connect(ui->pushButton15,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton15,15);
  QObject::connect(ui->pushButton16,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton16,16);
  QObject::connect(ui->pushButton17,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton17,17);
  QObject::connect(ui->pushButton18,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton18,18);
  QObject::connect(ui->pushButton19,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton19,19);
  QObject::connect(ui->pushButton20,SIGNAL(clicked()),BanButton,SLOT(map()));
  BanButton->setMapping(ui->pushButton20,20);
}

EditPaintBoard::~EditPaintBoard()
{
  delete ui;
}

//在切换窗口是确定该绘版的可见性
void EditPaintBoard::checkVisibility(int input){
  if (input==2) this->show();
  else this->close();
}

//将某个特定按钮置为不可用，其他按钮均可用
void EditPaintBoard::enableExcept(int number){
  ui->pushButton0->setEnabled(true);
  ui->pushButton1->setEnabled(true);
  ui->pushButton2->setEnabled(true);
  ui->pushButton3->setEnabled(true);
  ui->pushButton4->setEnabled(true);
  ui->pushButton5->setEnabled(true);
  ui->pushButton6->setEnabled(true);
  ui->pushButton7->setEnabled(true);
  ui->pushButton8->setEnabled(true);
  ui->pushButton9->setEnabled(true);
  ui->pushButton10->setEnabled(true);
  ui->pushButton11->setEnabled(true);
  ui->pushButton12->setEnabled(true);
  ui->pushButton13->setEnabled(true);
  ui->pushButton14->setEnabled(true);
  ui->pushButton15->setEnabled(true);
  ui->pushButton16->setEnabled(true);
  ui->pushButton17->setEnabled(true);
  ui->pushButton18->setEnabled(true);  
  ui->pushButton19->setEnabled(true);  
  ui->pushButton20->setEnabled(true);
  switch(number){
  case 0:
    ui->pushButton0->setEnabled(false);
    break;
  case 1:
    ui->pushButton1->setEnabled(false);
    break; 
  case 2:
    ui->pushButton2->setEnabled(false);
    break;
  case 3:
    ui->pushButton3->setEnabled(false);
    break;
  case 4:
    ui->pushButton4->setEnabled(false);
    break;
  case 5:
    ui->pushButton5->setEnabled(false);
    break;
  case 6:
    ui->pushButton6->setEnabled(false);
    break;
  case 7:
    ui->pushButton7->setEnabled(false);
    break;
  case 8:
    ui->pushButton8->setEnabled(false);
    break;
  case 9:
    ui->pushButton9->setEnabled(false);
    break;
  case 10:
    ui->pushButton10->setEnabled(false);
    break;
  case 11:
    ui->pushButton11->setEnabled(false);
    break;
  case 12:
    ui->pushButton12->setEnabled(false);
    break;
  case 13:
    ui->pushButton13->setEnabled(false);
    break;
  case 14:
    ui->pushButton14->setEnabled(false);
    break;
  case 15:
    ui->pushButton15->setEnabled(false);
    break;
  case 16:
    ui->pushButton16->setEnabled(false);
    break;
  case 17:
    ui->pushButton17->setEnabled(false);
    break;
  case 18:
    ui->pushButton18->setEnabled(false);
    break;
  case 19:
    ui->pushButton19->setEnabled(false);
    break;
  case 20:
    ui->pushButton20->setEnabled(false);
    break;
      
  }
}
