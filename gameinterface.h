#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include "gamecontrol.h"
#include "editleaderboarddialog.h"
class WindowManager;

#include <QDialog>
#include <QTimer>
#include <QDateTime>

namespace Ui {
class GameInterface;
}

//游戏界面
class GameInterface : public QDialog
{
  Q_OBJECT
  
public:
  explicit GameInterface(QWidget *parent = nullptr, WindowManager* manager=nullptr);
  ~GameInterface();
  void keyPressEvent(QKeyEvent*);
  void keyReleaseEvent(QKeyEvent*);

public slots:
  void switchback(); //退回到选择游玩地图界面
  void loadGame(); //重新加载地图
  void timeout();  //帧数计时器到时对应的槽
  void win(); //获得游戏胜利对应的槽
  
protected:
  void paintEvent(QPaintEvent*);  
  
private:
  Ui::GameInterface *ui;
  WindowManager* m_manager; //管理该窗口的窗口管理器的指针
  GControl* Controller;  //管理该窗口游戏的游戏控制器
  QElapsedTimer* Timer; //记录游戏耗时的计时器
  QTimer* RefreshTimer; //记录窗口重绘的计时器
  QTimer* FrameTimer; //记录计算下一帧的计时器
  QMediaPlayer* QM; //播放背景音乐的播放器
  qint64 FinalTime; //最终耗时
  double FinalScore; //最终得分
};

#endif // GAMEINTERFACE_H
