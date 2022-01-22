#ifndef PROGRESSTHREAD_H
#define PROGRESSTHREAD_H

#include <QThread>
#include <QStringList>
#include <QDebug>
#include <Windows.h>
#include <tchar.h>

enum {OPEN, COPY, MOVE, PASTE, RENAME};

class WorkerQt: public QThread
{
public:
  explicit WorkerQt(QStringList s, QString d);
  void run() override;
  void copyRecursive(QString s, QString d);

public slots:
  void threadTermination();

signals:
  void progressIncrease(int Bytes);
  void itemName(QString name);
  void finish();

private:
  QStringList src;
  QString dst;
};

#endif // PROGRESSTHREAD_H

