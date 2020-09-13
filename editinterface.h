#ifndef EDITINTERFACE_H
#define EDITINTERFACE_H

#include "editpaintboard.h"
#include "windowmanager.h"
class WindowManager;

#include <QDialog>
#include <QPaintEvent>
#include <string>
#include <QDesktopWidget>
#include <QDebug>
#include <iostream>
#include <QMessageBox>

namespace Ui {
class EditInterface;
}

//修改界面
class EditInterface : public QDialog
{
  Q_OBJECT
  
public:
  explicit EditInterface(QWidget *parent = nullptr, WindowManager* manager=nullptr);
  ~EditInterface();  
  void mousePressEvent(QMouseEvent*);
  void mouseMoveEvent(QMouseEvent*);
  void mouseReleaseEvent(QMouseEvent*);
private:
  Ui::EditInterface *ui;
  EditPaintBoard* PaintBoard; //修改界面控制的油漆板界面
  int BrushType=0; //当前修改的目标方块种类
  int EditType=1; //修改的模式，1为框选，0为点选
  std::string StringMap; //地图信息
  WindowManager* m_manager; //管理当前界面的窗口管理器的指针
  int BeginX,BeginY,NowX,NowY; //鼠标按下时与当前的坐标
  bool Pressed=false; //框选模式鼠标是否按下
public slots:
  void setBrush(int); //修改目标方块种类
  void save(); //保存地图
  void run(); //保存并运行地图
  void loadMap(); //重新加载地图
  void setEditTypeDot(bool); //设置修改的模式为点选
  void setEditTypeRect(bool); //设置修改的模式为框选
protected:
  void paintEvent(QPaintEvent*);
};

#endif // EDITINTERFACE_H
