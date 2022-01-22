#include "progress.h"
#include "ui_progress.h"

#include <filesystem>


Progress::Progress(QWidget *parent, QString &title) :
  QDialog(parent, Qt::WindowCloseButtonHint),
  ui(new Ui::Progress)
{
  ui->setupUi(this);
  this->setWindowTitle(title);
  QStringList lst;
  lst << "C:\\Users\\ghtayani\\Desktop\\a";
  worker = new WorkerQt(lst, "C:\\Users\\ghadyani\\Desktop\\a");
  worker->moveToThread(thread);
  connect(worker, SIGNAL(finish()), this, SLOT(threadFinished()));
}

Progress::~Progress()
{
  delete ui;
}

QString Progress::compactSize(int bytes){
  float size = bytes;
  QString dim("B");
  if(size>=1024){
      size /= 1024;
      dim = QString("KB");
    }
  if(size>=1024){
      size /= 1024;
      dim = QString("MB");
    }
  if(size>=1024){
      size /= 1024;
      dim = QString("GB");
    }
  if(size>=1024){
      size /= 1024;
      dim = QString("TB");
    }
  return QString("%1 "+dim).arg(size);
}

void Progress::progressionIncrease(int bytes)
{
  ui->progressBar->setValue(ui->progressBar->value() + bytes);
}

void Progress::itemNameReceive(QString name)
{
  ui->itemName->setText("Item name: "+name);
}

void Progress::setupProgress(int action, QStringList src, QString dst)
{
  worker = new WorkerQt(src, dst);
  connect(this, SIGNAL(terminateThread()), worker, SLOT(threadTermination()));
  worker->moveToThread(thread);
  if(action == COPY)
    {
    } else if(action == MOVE)
      {
      } else if(action == RENAME)
      {
      }
}

void Progress::threadFinished()
{
  qDebug() << "Ok done";
}

void Progress::closeEvent(QCloseEvent *event)
{
  terminateThread();
}
