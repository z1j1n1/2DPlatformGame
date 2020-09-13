#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QDialog>
#include <QPainter>
#include <QTextBrowser>
class WindowManager;

namespace Ui {
class LeaderBoard;
}

//排行榜界面
class LeaderBoard : public QDialog
{
  Q_OBJECT
  
public:
  explicit LeaderBoard(QWidget *parent = nullptr, WindowManager* manager=nullptr);
  ~LeaderBoard();

public slots:
  void loadLeaderBoard(); //加载排行榜
  
protected:
  void paintEvent(QPaintEvent* ev);
  
private:
  Ui::LeaderBoard *ui;
  WindowManager* m_manager; //管理该窗口的窗口管理器
  void loadScore(QTextBrowser* Browser, QString FileName); //在Browser中加载成绩
};

#endif // LEADERBOARD_H
