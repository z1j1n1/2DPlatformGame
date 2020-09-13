#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "maininterface.h"
#include "mapstopng.h"
#include "editselectioninterface.h"
#include "editinterface.h"
#include "playselectioninterface.h"
#include "gameinterface.h"
#include "editplayinterface.h"
#include "leaderboard.h"

#include <QObject>
#include <vector>
using namespace std;

//窗口管理器
class WindowManager : public QObject
{
  Q_OBJECT
public:
  explicit WindowManager(QObject *parent = nullptr);
  ~ WindowManager();
  int getSelectedMap(); //获取当前选中的地图

private:
  vector <QWidget*> WindowList; //管理的窗口列表
  int Current, SelectedMap; //当前打开的窗口，当前选中的地图
  MapsToPng Converter; //将地图转换为缩略图的工具
  
signals:
  void switched(int); //窗口被切换
  void mapChanged(); //选中地图发生变化
  void reloadImgs(); //重新加载缩略图
  void startGame(); //开始游戏
  void startEditGame(); //开始编辑模式下的游戏
  void loadLeaderBoard(); //加载排行榜
  
public slots:
  void switchTo(int); //转到指定窗口
  void SelectMap(int);  //选择指定地图
};

#endif // WINDOWMANAGER_H
