#include "windowmanager.h"

//创建所有需要的窗口
WindowManager::WindowManager(QObject *parent) : QObject(parent){
  WindowList.push_back(new MainInterface(nullptr,this));
  WindowList.push_back(new EditSelectionInterface(nullptr,this));
  WindowList.push_back(new EditInterface(nullptr,this));
  WindowList.push_back(new PlaySelectionInterface(nullptr,this));
  WindowList.push_back(new GameInterface(nullptr,this));
  WindowList.push_back(new EditPlayInterface(nullptr,this));
  WindowList.push_back(new LeaderBoard(nullptr,this));
  
  WindowList[0]->show();Current=0;
}

WindowManager::~WindowManager(){
  //删除窗口
  for (int i=0;i<(int)WindowList.size();i++)
    delete WindowList[i];
}

//切换窗口
void WindowManager::switchTo(int pos){
  emit switched(pos);
  WindowList[Current]->close();
  Current=pos;
  Converter.work();
  emit reloadImgs();
  WindowList[Current]->show();
  if (pos==4)
    emit startGame();
  if (pos==5)
    emit startEditGame();
  if (pos==6)
    emit loadLeaderBoard();
}

//选择地图
void WindowManager::SelectMap(int input){
  SelectedMap=input;  
  emit mapChanged();
}

//获得选中的地图
int WindowManager::getSelectedMap(){
  return(SelectedMap);
}
