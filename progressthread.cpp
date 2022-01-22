#include "progressthread.h"

WorkerQt::WorkerQt(QStringList s, QString d):
  src(s), dst(d)
{
  qDebug() << "worker";
}

void WorkerQt::threadTermination()
{

}

void WorkerQt::run()
{
  qDebug() << "copy ";
  for(int i=0; i<src.size(); i++)
    {
      qDebug() << "copy " << i;
      copyRecursive(src[i], dst);
    }
}

void WorkerQt::copyRecursive(QString s, QString d)
{
  std::string argv = s.toUtf8();
  WIN32_FIND_DATA FindFileData;
  HANDLE hFind;
  hFind = FindFirstFile((LPCWSTR)argv.c_str(), &FindFileData);
  if (hFind == INVALID_HANDLE_VALUE)
    {
      printf("FindFirstFile failed %s", GetLastError());
      //emit finish();
      return;
    }
  else
    {
      printf("The first file found is %s" ,FindFileData.cFileName);
      FindClose(hFind);
    }
 // emit finish();

}
