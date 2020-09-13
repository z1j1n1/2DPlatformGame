#ifndef PLAYSELECTIONINTERFACE_H
#define PLAYSELECTIONINTERFACE_H

#include <QDialog>
#include <QPainter>
#include <QSignalMapper>

namespace Ui {
class PlaySelectionInterface;
}

//选择游玩地图界面
class PlaySelectionInterface : public QDialog
{
  Q_OBJECT
  
public:
  explicit PlaySelectionInterface(QWidget *parent = nullptr, QObject *manager=nullptr);
  ~PlaySelectionInterface();
  
public slots:
  void reloadImgs(); //重新加载按钮上的缩略图
  
protected:
  void paintEvent(QPaintEvent*);
  
private:
  Ui::PlaySelectionInterface *ui;
};

#endif // PLAYSELECTIONINTERFACE_H
