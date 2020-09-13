#ifndef EDITPAINTBOARD_H
#define EDITPAINTBOARD_H

#include <QDialog>
#include <QCloseEvent>
#include <QSignalMapper>
namespace Ui {
class EditPaintBoard;
}

//修改模式下的绘版

class EditPaintBoard : public QDialog
{
  Q_OBJECT
  
public:
  explicit EditPaintBoard(QWidget *parent = nullptr);
  ~EditPaintBoard();
public slots:
  void checkVisibility(int);  //在切换窗口是确定该绘版的可见性
  void enableExcept(int); //将某个特定按钮置为不可用，其他按钮均可用
  
private:
  Ui::EditPaintBoard *ui;
};

#endif // EDITPAINTBOARD_H
