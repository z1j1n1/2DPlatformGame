#ifndef EDITSELECTIONINTERFACE_H
#define EDITSELECTIONINTERFACE_H

#include <QDialog>
#include <QPainter>
#include <QSignalMapper>

namespace Ui {
class EditSelectionInterface;
}

//挑选编辑地图界面
class EditSelectionInterface : public QDialog
{
  Q_OBJECT
  
public:
  explicit EditSelectionInterface(QWidget *parent = nullptr, QObject *manager=nullptr);
  ~EditSelectionInterface();
  
public slots:
  void reloadImgs();//重新加载按钮上的缩略图
  
protected:
  void paintEvent(QPaintEvent*);
  
private:
  Ui::EditSelectionInterface *ui;
};

#endif // EDITSELECTIONINTERFACE_H
