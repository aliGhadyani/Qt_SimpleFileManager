#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFileSystemModel>
#include <QList>
#include <QListView>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScrollArea>
#include <QSettings>
#include <QSplitter>
#include <QThread>
#include <QUrl>
#include <QVector>

#include <QDirModel>

#include <windows.h>
#include <winreg.h>
#include <stdlib.h>
#include <cstring>
#include <filesystem>

#define MAX_PATH_LENGTH 260

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

public:
  void clearView();
  void open(QString path);
  void copy();
  void move();
  void paste();
  void del();

private slots:
  void on_action_Tile_View_triggered();
  void on_action_List_View_triggered();
  void on_action_Exit_triggered();
  void on_treeView_clicked(const QModelIndex &index);
  void cleanScrollArea();
  void on_foreward_clicked();
  void on_listWidget_clicked(const QModelIndex &index);
  void on_backward_clicked();
  void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
  void on_listWidget_itemClicked(QListWidgetItem *item);
  void on_action_Refresh_triggered();
  void on_addressBar_returnPressed();

  void on_pushButton_clicked();

  void on_listWidget_customContextMenuRequested(const QPoint &pos);

  void on_listWidget_itemPressed(QListWidgetItem *item);

public slots:
  void MainWindow::resizeEvent(QResizeEvent *e);
  void MainWindow::closeEvent(QCloseEvent *e);


private:
  Ui::MainWindow *ui;
  QLabel *statusBar;
  QFileSystemModel *model;
  QFileSystemModel *modelV;
  QSplitter *sp;
  QString lastDir;
  QList<QString> pathList;
  QList<QString> clipboard;
  QVector<QThread> threads;
  int currentIdx = 0;
};

#endif // MAINWINDOW_H
