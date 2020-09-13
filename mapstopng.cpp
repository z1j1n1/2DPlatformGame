#include "mapstopng.h"


MapsToPng::MapsToPng(QObject *parent) : QObject(parent)
{
  //记录每种图案对应缩略图的颜色
  colors=new QRgb[30]{
      qRgb(255,255,255),
      qRgb(190,35,41),
      qRgb(108,225,132),
      qRgb(82,174,214),
      qRgb(241,73,144),
      qRgb(233,187,241),
      qRgb(166,179,235),
      qRgb(135,60,219),
      qRgb(153,62,12),
      qRgb(13,94,36),
      qRgb(183,6,28),
      qRgb(179,222,71),
      qRgb(200,77,18),
      qRgb(139,187,67),
      qRgb(3,31,166),
      qRgb(9,125,90),
      qRgb(31,37,56),
      qRgb(203,212,93),
      qRgb(245,150,252),
      qRgb(19,59,69),
      qRgb(10,137,13),
      qRgb(174,219,28),
      qRgb(154,32,50),
      qRgb(64,238,80),
      qRgb(253,54,120),
      qRgb(50,73,18),
      qRgb(125,158,246),
      qRgb(173,220,73),
      qRgb(242,20,79),
      qRgb(102,64,68)};
}

//将一张地图转回为一张缩略图
void MapsToPng::convert(const QString& src, const QString& tar){
  //读取地图
  QFile* file=new QFile(src);
  file->open(QIODevice::ReadOnly);
  std::string StringMap=QString(file->readAll()).toStdString();
  file->close();
  
  //绘图
  QImage Img(60,40,QImage::Format_RGB32);
  int CurrentPosition=0;
  for (int _y=1;_y<=10;_y++)
    for (int  _x=1;_x<=15;_x++){
      for (int offsetx=0;offsetx<4;offsetx++)
        for (int offsety=0;offsety<4;offsety++)
          Img.setPixel(4*(_x-1)+offsetx,4*(_y-1)+offsety,colors[10*((StringMap[CurrentPosition])-'0')+StringMap[CurrentPosition+1]-'0']);
      CurrentPosition+=2;
    }
  
  //保存
  Img.save(tar,"PNG",100);
}

void MapsToPng::work(){
  convert(QString("./maps/1.txt"),QString("./maps/1.png"));
  convert(QString("./maps/2.txt"),QString("./maps/2.png"));
  convert(QString("./maps/3.txt"),QString("./maps/3.png"));
  convert(QString("./maps/4.txt"),QString("./maps/4.png"));
  convert(QString("./maps/5.txt"),QString("./maps/5.png"));
  convert(QString("./maps/6.txt"),QString("./maps/6.png"));
  convert(QString("./maps/7.txt"),QString("./maps/7.png"));
  convert(QString("./maps/8.txt"),QString("./maps/8.png"));
  convert(QString("./maps/9.txt"),QString("./maps/9.png"));
  convert(QString("./maps/10.txt"),QString("./maps/10.png"));
}

MapsToPng::~MapsToPng(){
  delete[] colors;
}
