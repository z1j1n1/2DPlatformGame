#ifndef MAPSTOPNG_H
#define MAPSTOPNG_H

#include <QObject>
#include <QDir>
#include <QRgb>
#include <QImage>
#include <QDebug>
#include <string>

//将地图转换为缩略图的工具
class MapsToPng : public QObject
{
  Q_OBJECT
public:
  explicit MapsToPng(QObject *parent = nullptr);
  void work(); //转换所有地图
  ~MapsToPng();
private:
  void convert(const QString&,const QString&); //转换某个地图
  QRgb* colors; //对象种类对应的缩略图中的颜色
signals:
  
};

#endif // MAPSTOPNG_H
