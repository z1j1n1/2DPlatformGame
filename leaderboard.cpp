#include "leaderboard.h"
#include "ui_leaderboard.h"
#include "windowmanager.h"

LeaderBoard::LeaderBoard(QWidget *parent, WindowManager* manager) :
  QDialog(parent),
  ui(new Ui::LeaderBoard)
{
  ui->setupUi(this);
  m_manager=manager;
  
  QSignalMapper* WindowConversion =new QSignalMapper(this);
  QObject::connect(ui->pushButton,SIGNAL(clicked()),WindowConversion,SLOT(map()));
  WindowConversion->setMapping(ui->pushButton,0);
  QObject::connect(WindowConversion,SIGNAL(mapped(int)),manager,SLOT(switchTo(int)));
  
  QObject::connect(manager,SIGNAL(loadLeaderBoard()),this,SLOT(loadLeaderBoard()));
}

LeaderBoard::~LeaderBoard()
{
  delete ui;
}

void LeaderBoard::paintEvent(QPaintEvent* ev){
  QPainter painter(this);
  painter.drawPixmap(rect(),QPixmap(":/universal/mario-images/background.png"));  
}

//载入每张地图的积分榜
void LeaderBoard::loadLeaderBoard(){
  loadScore(ui->textBrowser_1,QString::fromStdString("./maps/1leaderboard.txt"));
  loadScore(ui->textBrowser_2,QString::fromStdString("./maps/2leaderboard.txt"));
  loadScore(ui->textBrowser_3,QString::fromStdString("./maps/3leaderboard.txt"));
  loadScore(ui->textBrowser_4,QString::fromStdString("./maps/4leaderboard.txt"));
  loadScore(ui->textBrowser_5,QString::fromStdString("./maps/5leaderboard.txt"));
  loadScore(ui->textBrowser_6,QString::fromStdString("./maps/6leaderboard.txt"));
  loadScore(ui->textBrowser_7,QString::fromStdString("./maps/7leaderboard.txt"));
  loadScore(ui->textBrowser_8,QString::fromStdString("./maps/8leaderboard.txt"));
  loadScore(ui->textBrowser_9,QString::fromStdString("./maps/9leaderboard.txt"));
  loadScore(ui->textBrowser_10,QString::fromStdString("./maps/10leaderboard.txt"));
}

//将某张地图的得分记录按照得分排序并显示
void LeaderBoard::loadScore(QTextBrowser* Browser, QString FileName){
  Browser->clear();
  QFile* file=new QFile(FileName);
  file->open(QIODevice::ReadOnly);
  QTextStream in(file);
  
  vector< pair<double, pair <QString,QString> > > ScoreList;
  
  //读取所有记录
  while (!in.atEnd()) {
    double Score=in.readLine().toDouble();
    QString Name=in.readLine();
    QString Time=in.readLine();
    ScoreList.push_back(make_pair(Score,make_pair(Name,Time)));
  }
  
  //排序
  sort(ScoreList.begin(),ScoreList.end());
  
  //添加记录到Browser
  for (int i=ScoreList.size()-1;i>=0;i--)
    Browser->append("第"+QString::number(ScoreList.size()-i)+"名:"+ScoreList[i].second.first+"在"+ScoreList[i].second.second+"得到"+QString::number(ScoreList[i].first,'f',2)+"分");
  
  file->close();
}
