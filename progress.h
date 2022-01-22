#ifndef PROGRESS_H
#define PROGRESS_H
#include "progressthread.h"

#include <QDialog>
#include <QThread>
#include <QString>
#include <QDebug>

namespace Ui {
  class Progress;
}

class Progress : public QDialog
{
  Q_OBJECT

public:
  explicit Progress(QWidget *parent = 0, QString &title = QString("Progress"));
  ~Progress();
  void setupProgress(int action, QStringList src, QString dst);
  QString compactSize(int bytes);
public slots:
  void progressionIncrease(int bytes);
  void itemNameReceive(QString name);
  void threadFinished();
  void closeEvent(QCloseEvent *event);

signals:
  void terminateThread();

private:
  Ui::Progress *ui;
  QThread *thread;
  WorkerQt *worker;
};

#endif // PROGRESS_H
