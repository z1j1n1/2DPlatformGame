#include "editleaderboarddialog.h"
#include "ui_editleaderboarddialog.h"

EditLeaderBoardDialog::EditLeaderBoardDialog(double Score,QWidget *parent) :
  QDialog(parent),
  ui(new Ui::EditLeaderBoardDialog)
{
  ui->setupUi(this);
  ui->label->setText("你赢了并获得得分"+QString::number(Score,'f',2)+",请留下姓名");
}

EditLeaderBoardDialog::~EditLeaderBoardDialog()
{
  delete ui;
}

//获取输入的姓名
QString EditLeaderBoardDialog::getName(){
  return(ui->lineEdit->text());
}
