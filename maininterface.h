#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include "windowmanager.h"
#include "GameClasses.h"

#include <QDialog>
#include <QPainter>
#include <QElapsedTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QSignalMapper>

QT_BEGIN_NAMESPACE
namespace Ui { class MainInterface; }
QT_END_NAMESPACE

//主界面
class MainInterface : public QDialog
{
  Q_OBJECT
  
public:
  MainInterface(QWidget *parent = nullptr, QObject *Manager = nullptr);
  ~MainInterface();
  
private:
  Ui::MainInterface *ui;
  
protected:
  void paintEvent(QPaintEvent*);
};
#endif // MAININTERFACE_H
