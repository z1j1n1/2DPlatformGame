#ifndef EDITLEADERBOARDDIALOG_H
#define EDITLEADERBOARDDIALOG_H

#include <QDialog>

namespace Ui {
class EditLeaderBoardDialog;
}

//游戏胜利时弹出的对话框
class EditLeaderBoardDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit EditLeaderBoardDialog(double Score,QWidget *parent = nullptr);
  ~EditLeaderBoardDialog();
  QString getName();//获取输入的姓名
  
private:
  Ui::EditLeaderBoardDialog *ui;
};

#endif // EDITLEADERBOARDDIALOG_H
