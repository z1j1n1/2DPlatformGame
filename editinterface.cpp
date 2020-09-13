#include "editinterface.h"
#include "ui_editinterface.h"

EditInterface::EditInterface(QWidget *parent, WindowManager* manager) :
  QDialog(parent),
  ui(new Ui::EditInterface)
{
  ui->setupUi(this);
  m_manager=manager;
  PaintBoard=new EditPaintBoard(this);
  PaintBoard->move(-25,0);
  //在窗口切换时确定绘版是否可见
  QObject::connect(manager,SIGNAL(switched(int)),PaintBoard,SLOT(checkVisibility(int)));
  //在选中地图变化时重新加载地图
  QObject::connect(manager,SIGNAL(mapChanged()),this,SLOT(loadMap()));
}

EditInterface::~EditInterface()
{
  delete ui;
}

//修改目标方块种类
void EditInterface::setBrush(int input){
  BrushType=input;
}

void EditInterface::paintEvent(QPaintEvent *ev){
  QPainter painter(this);
  painter.setPen(Qt::black);
  //加载背景
  painter.drawPixmap(rect(),QPixmap(":/universal/mario-images/background.png")); 
  //绘图方格
  for (int i=1;i<10;i++)
    painter.drawLine(0,90*i,1350,90*i);
  for (int i=1;i<15;i++)
    painter.drawLine(90*i,0,90*i,900);
  int CurrentPos=0;
  //绘制地图上的方块
  if (StringMap.size()==300)
  for (int i=0;i<10;i++)
    for (int j=0;j<15;j++){
      int BlockType=10*(StringMap[CurrentPos]-'0')+StringMap[CurrentPos+1]-'0';
      CurrentPos+=2;
      QRect Rect(j*90,i*90,90,90);
      switch(BlockType){
      case 1:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/mario_stop.png"));
        break;
      case 2:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/peach_sprite.png"));
        break;
      case 3:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/rail_right.png"));
        break;
      case 4:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/rail_down.png"));
        break;
      case 5:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/rail_up.png"));
        break;
      case 6:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/rail_left.png"));
        break;
      case 7:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/floor_right.jpg"));
        break;
      case 8:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/floor_down.jpg"));
        break;
      case 9:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/floor_up.jpg"));
        break;
      case 10:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/floor_left.jpg"));
        break;
      case 11:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/dark_eater.png"));
        break;
      case 12:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/dark_eater_die.png"));
        break;
      case 13:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/fantom_sprite.png"));
        break;
      case 14:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/mysticTree.png"));
        break;
      case 15:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/fire.png"));
        break;
      case 16:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/champ_gris.png"));
        break;
      case 17:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/flower.png"));
        break;
      case 18:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/floor_uni.png"));
        break;
      case 19:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/floor_breakable.png"));
        break;
      case 20:
        painter.drawPixmap(Rect,QPixmap(":/edit/EditMode/spring.png"));
        break;
      }
    }
  
  //在框选模式时绘画选中方格
  if (Pressed){
    painter.setPen(Qt::green);    
    painter.drawLine(min(BeginX,NowX),BeginY,max(BeginX,NowX),BeginY);
    painter.drawLine(min(BeginX,NowX),NowY,max(BeginX,NowX),NowY);
    painter.drawLine(BeginX,min(BeginY,NowY),BeginX,max(BeginY,NowY));
    painter.drawLine(NowX,min(BeginY,NowY),NowX,max(BeginY,NowY));
  }
}

//重新加载地图
void EditInterface::loadMap(){
  QString Temp="./maps/";
  int SM=m_manager->getSelectedMap();
  if (SM<10) Temp+=QChar(SM+'0');else Temp+="10";
  Temp+=".txt";
  QFile* file=new QFile(Temp);
  file->open(QIODevice::ReadWrite);
  StringMap=QString(file->readAll()).toStdString();
  file->close();
}

//设置修改的模式为点选
void EditInterface::setEditTypeDot(bool flag){
  if (flag) EditType=0;
}

//设置修改的模式为框选
void EditInterface::setEditTypeRect(bool flag){
  if (flag) EditType=1;
}

//鼠标按下事件
void EditInterface::mousePressEvent(QMouseEvent* ev){
  if (ev->button()==Qt::LeftButton){
    if (EditType==0){//修改模式为点选，直接修改
      int PosX=int(ev->x()/90),PosY=int(ev->y()/90);
      StringMap[2*(PosY*15+PosX)]=BrushType/10+'0';
      StringMap[2*(PosY*15+PosX)+1]=BrushType%10+'0';
      update();
    }else{//修改模式为框选，记录开始位置
      BeginX=ev->x();
      BeginY=ev->y();     
      Pressed=true;
    }
  }
}

//鼠标移动事件
void EditInterface::mouseMoveEvent(QMouseEvent* ev){
  if (ev->buttons()&Qt::LeftButton){
    if (EditType==0){//修改模式为点选，直接修改
      int PosX=int(ev->x()/90),PosY=int(ev->y()/90);
      StringMap[2*(PosY*15+PosX)]=BrushType/10+'0';
      StringMap[2*(PosY*15+PosX)+1]=BrushType%10+'0';
      update();
    }else{//修改模式为框选，记录当前位置，绘画选中方框
      NowX=ev->x();
      NowY=ev->y();
      update();
    }
  }
}

//鼠标释放事件
void EditInterface::mouseReleaseEvent(QMouseEvent* ev){
  if (ev->button()==Qt::LeftButton){
    if (EditType==0){//修改模式为点选，直接修改
      int PosX=int(ev->x()/90),PosY=int(ev->y()/90);
      StringMap[2*(PosY*15+PosX)]=BrushType/10+'0';
      StringMap[2*(PosY*15+PosX)+1]=BrushType%10+'0';
      update();
    }else{//修改模式为框选，记录当前位置，在选中方框内修改
      NowX=ev->x();
      NowY=ev->y();
      Pressed=false;
      int minX=min(BeginX,NowX),maxX=max(BeginX,NowX),minY=min(BeginY,NowY),maxY=max(BeginY,NowY);
      for (int PosX=max(minX/90,0);PosX<min((maxX+89)/90,15);PosX++)
        for (int PosY=max(minY/90,0);PosY<min((maxY+89)/90,10);PosY++){
          StringMap[2*(PosY*15+PosX)]=BrushType/10+'0';
          StringMap[2*(PosY*15+PosX)+1]=BrushType%10+'0';
        }
      update();
    }
  }
}

//保存地图
void EditInterface::save(){
  int CountHero=0,CountTarget=0,CurrentPos=0;
  //统计地图中的主角与目标数量
  for (int i=0;i<10;i++)
    for (int j=0;j<15;j++){
      int BlockType=10*(StringMap[CurrentPos]-'0')+StringMap[CurrentPos+1]-'0';
      if (BlockType==1) 
        CountHero++;
      if (BlockType==2)
        CountTarget++;
      CurrentPos+=2;
    }
  if (CountHero!=1){//主角数量不为1，发出警告
    int ret=QMessageBox::warning(this, "警告", "地图上应当有且仅有一个主角", QMessageBox::Ok);
  }else
  if (CountTarget!=1){//目标数量不为1,发出警告
    int ret=QMessageBox::warning(this, "警告", "地图上应当有且仅有一个目标", QMessageBox::Ok);
  }else{
    //保存地图
    QString Temp="./maps/";
    int SM=m_manager->getSelectedMap();
    if (SM<10) Temp+=QChar(SM+'0');else Temp+="10";
    Temp+=".txt";
    QFile* file=new QFile(Temp);
    file->open(QIODevice::ReadWrite);
    QTextStream out(file);
    out << QString::fromStdString(StringMap);
    file->close();
    
    //清空积分榜
    Temp="./maps/";
    if (SM<10) Temp+=QChar(SM+'0');else Temp+="10";
    Temp+="leaderboard.txt";   
    QFile* ScoreInfo=new QFile(Temp);
    ScoreInfo->open(QIODevice::WriteOnly);
    QTextStream clear(ScoreInfo);
    clear << QString::fromStdString("");    
    ScoreInfo->close();
    
    //切换至挑选编辑地图界面
    m_manager->switchTo(1);
  }
}

//保存并运行地图
void EditInterface::run(){
  int CountHero=0,CountTarget=0,CurrentPos=0;
  //统计地图中的主角与目标数量  
  for (int i=0;i<10;i++)
    for (int j=0;j<15;j++){
      int BlockType=10*(StringMap[CurrentPos]-'0')+StringMap[CurrentPos+1]-'0';
      if (BlockType==1) 
        CountHero++;
      if (BlockType==2)
        CountTarget++;
      CurrentPos+=2;
    }
  if (CountHero!=1){//主角数量不为1，发出警告
    int ret=QMessageBox::warning(this, "警告", "地图上应当有且仅有一个主角", QMessageBox::Ok);
  }else
  if (CountTarget!=1){//目标数量不为1,发出警告
    int ret=QMessageBox::warning(this, "警告", "地图上应当有且仅有一个目标", QMessageBox::Ok);
  }else{
    //保存地图    
    QString Temp="./maps/";
    int SM=m_manager->getSelectedMap();
    if (SM<10) Temp+=QChar(SM+'0');else Temp+="10";
    Temp+=".txt";
    QFile* file=new QFile(Temp);
    file->open(QIODevice::ReadWrite);
    QTextStream out(file);
    out << QString::fromStdString(StringMap);
    file->close();
    
    //切换至在编辑模式下启动的游玩界面
    m_manager->switchTo(5);
    
    //清空积分榜    
    Temp="./maps/";
    if (SM<10) Temp+=QChar(SM+'0');else Temp+="10";
    Temp+="leaderboard.txt";   
    file=new QFile(Temp);
    file->open(QIODevice::WriteOnly);
    QTextStream clear(file);
    clear << QString::fromStdString("");    
    file->close();
  }
}
