#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "progress.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  // making view options checkable
  ui->action_List_View->setCheckable(true);
  ui->action_List_View->setChecked(true);
  ui->action_Tile_View->setCheckable(true);
  ui->action_Restore_Last_View->setCheckable(true);
  ui->listWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
  ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
  connect(ui->listWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_listWidget_customContextMenuRequested(QPoint)));
  // make tree view and scrol area spiltable
  sp = new QSplitter(Qt::Horizontal);
  sp->addWidget(ui->treeView);
  sp->addWidget(ui->listWidget);
  QList<int> lst;
  lst << 50<<this->width()-50-20;
  sp->setSizes(lst);

  // loading settings
  int sizeOfInt = sizeof(int);
  bool RLV[1];
  bool TV[1];
  bool MW[1];
  int sizeOfBool = sizeof(RLV);
  int W[1];
  int H[1];
  int X[1];
  int Y[1];
  int SW[1];
  char LD[MAX_PATH_LENGTH];
  int bufferSize = MAX_PATH_LENGTH;
  HKEY hKey;
  RegOpenKeyExA(
        HKEY_CURRENT_USER,
        "Software\\AmnPardaz\\SimpleFileManager\\AppState",
        NULL,
        KEY_READ,
        &hKey);
  RegQueryValueExA(
        hKey,
        "RLV",
        NULL,
        NULL,
        (LPBYTE)RLV,
        (LPDWORD)&sizeOfBool);
  RegQueryValueExA(
        hKey,
        "TV",
        NULL,
        NULL,
        (LPBYTE)TV,
        (LPDWORD)&sizeOfBool);
  RegQueryValueExA(
        hKey,
        "MW",
        NULL,
        NULL,
        (LPBYTE)MW,
        (LPDWORD)&sizeOfBool);
  RegQueryValueExA(
        hKey,
        "W",
        NULL,
        NULL,
        (LPBYTE)W,
        (LPDWORD)&sizeOfInt);
  RegQueryValueExA(
        hKey,
        "H",
        NULL,
        NULL,
        (LPBYTE)H,
        (LPDWORD)&sizeOfInt);
  RegQueryValueExA(
        hKey,
        "X",
        NULL,
        NULL,
        (LPBYTE)X,
        (LPDWORD)&sizeOfInt);
  RegQueryValueExA(
        hKey,
        "Y",
        NULL,
        NULL,
        (LPBYTE)Y,
        (LPDWORD)&sizeOfInt);
  RegQueryValueExA(
        hKey,
        "LD",
        NULL,
        NULL,
        (LPBYTE)LD,
        (LPDWORD)&bufferSize);
  RegQueryValueExA(
        hKey,
        "SW",
        NULL,
        NULL,
        (LPBYTE)SW,
        (LPDWORD)&sizeOfInt);
  RegCloseKey(hKey);

  // test status bar
//  ui->statusBar->showMessage("Test Message |      4,254 files");

  sp->setChildrenCollapsible(false);
  ui->horizontalLayout->addWidget(sp);
  // set tree view model
  model = new QFileSystemModel(this);
  model->setRootPath(QDir::rootPath());
  model->setFilter(QDir::Filter::AllDirs | QDir::NoDotAndDotDot);
  model->setNameFilterDisables(true);

  ui->treeView->setModel(model);
  ui->treeView->setAnimated(true);
  ui->treeView->setHeaderHidden(true);
  ui->treeView->setColumnHidden(1, true);
  ui->treeView->setColumnHidden(2, true);
  ui->treeView->setColumnHidden(3, true);
  ui->treeView->resizeColumnToContents(0);

  lastDir = QString("/");
  qDebug() << lastDir;
  QModelIndex a = model->index(QString(lastDir));
  ui->treeView->expand(a);
  ui->treeView->scrollTo(a);
  ui->treeView->setCurrentIndex(a);
  on_treeView_clicked(a);

  if(RLV[0]){
      this->setGeometry(X[0], Y[0], W[0], H[0]);
      this->resize(W[0], H[0]);
      QList<int> lst;
      lst << SW[0]<<this->width()-SW[0]-20;
      sp->setSizes(lst);
      if(strcmp(LD, "")!=0){
          lastDir = QString(LD);
          pathList.append(lastDir);
          a = model->index(QString(lastDir));
          ui->treeView->expand(a);
          ui->treeView->scrollTo(a);
          ui->treeView->setCurrentIndex(a);
          on_treeView_clicked(a);
        }
      ui->action_Restore_Last_View->setChecked(RLV[0]);
      if(ui->action_Tile_View->isChecked() != TV[0]){
          ui->action_Tile_View->toggle();
          on_action_Tile_View_triggered();
        }
      if(MW[0]){
          this->showMaximized();
        }
    }
}

MainWindow::~MainWindow()
{
  delete ui;
}

//-----------------------------------------------------------------------------

// slots
void MainWindow::on_action_Tile_View_triggered()
{
  ui->action_List_View->toggle();
}
void MainWindow::on_action_List_View_triggered()
{
  ui->action_Tile_View->toggle();
}
void MainWindow::on_action_Exit_triggered()
{
  this->close();
}
void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
  open(model->filePath(index));
}
void MainWindow::on_foreward_clicked()
{
  if(currentIdx>=pathList.size()-1){
      return;
    }
  currentIdx++;
  lastDir = pathList[currentIdx];
  open(lastDir);
}
void MainWindow::on_backward_clicked()
{
  if(currentIdx <= 0){
      return;
    }
  currentIdx--;
  lastDir = pathList[currentIdx];
  open(lastDir);
}
void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
  if(index.isValid()){
      return;
    }
}
void MainWindow::resizeEvent(QResizeEvent *e)
{
  //  ui->treeView->setMaximumWidth(0.2*(this->width()-20));
  //  ui->treeView->setMaximumWidth(0.8*(this->width()-20));
  if(!e){
      return;
    }
}
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
  QString b = item->text();
  open(b);
}
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
  int row = -1;
  for(int i=0; i<ui->listWidget->count(); i++){
      if(ui->listWidget->item(i) == item){
          row = i;
          break;
        }
    }
  if(row<0 || row>ui->listWidget->count()){
      return;
    }
}
void MainWindow::on_action_Refresh_triggered()
{
  open(lastDir);
}
void MainWindow::closeEvent(QCloseEvent *e)
{
  bool RLV[1] = {ui->action_Restore_Last_View->isChecked()};
  bool TV[1] = {ui->action_Tile_View->isChecked()};
  bool MW[1] = {this->isMaximized()};
  int W[1] = {this->width()};
  int H[1] = {this->height()};
  int X[1] = {this->x()};
  int Y[1] = {this->y()};
  int SW[1] = {ui->treeView->width()};
  char *LD = lastDir.toLocal8Bit().data();
  // open registry and save app state
  HKEY hKey;
  if(
     RegOpenKeyExA(
       HKEY_CURRENT_USER,
       "Software\\AmnPardaz\\SimpleFileManager",
       NULL,
       KEY_ALL_ACCESS,
       &hKey) != ERROR_SUCCESS){
      RegCreateKeyExA(HKEY_CURRENT_USER,
                      "Software\\AmnPardaz\\SimpleFileManager",
                      NULL,
                      NULL,
                      NULL,
                      KEY_ALL_ACCESS,
                      NULL,
                      &hKey,
                      NULL);
    }

  RegSetKeyValueA(
        hKey,
        "AppState",
        "RLV",
        REG_BINARY,
        (LPBYTE)RLV,
        sizeof(RLV));
  RegSetKeyValueA(
        hKey,
        "AppState",
        "TV",
        REG_BINARY,
        (LPBYTE)TV,
        sizeof(RLV));
  RegSetKeyValueA(
        hKey,
        "AppState",
        "MW",
        REG_BINARY,
        (LPBYTE)MW,
        sizeof(MW));
  RegSetKeyValueA(
        hKey,
        "AppState",
        "W",
        REG_DWORD,
        (LPBYTE)W,
        sizeof(W));
  RegSetKeyValueA(
        hKey,
        "AppState",
        "H",
        REG_DWORD,
        (LPBYTE)H,
        sizeof(H));
  RegSetKeyValueA(
        hKey,
        "AppState",
        "X",
        REG_DWORD,
        (LPBYTE)X,
        sizeof(X));
  RegSetKeyValueA(
        hKey,
        "AppState",
        "Y",
        REG_DWORD,
        (LPBYTE)Y,
        sizeof(Y));
  RegSetKeyValueA(
        hKey,
        "AppState",
        "SW",
        REG_DWORD,
        (LPBYTE)SW,
        sizeof(SW));
  RegSetKeyValueA(
        hKey,
        "AppState",
        "LD",
        REG_SZ,
        (LPBYTE)LD,
        MAX_PATH_LENGTH);
  RegCloseKey(hKey);
  e->accept();
}
void MainWindow::on_addressBar_returnPressed()
{
  if(QDir(ui->addressBar->text()).isReadable()){
      open(ui->addressBar->text());

    } else {
      QMessageBox::warning(this, "Path Error", "Invalid Path or Access denied!", QMessageBox::Ok);
    }
}
void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
  QList<QListWidgetItem*> selected = ui->listWidget->selectedItems();
  if(selected.size()==0)
    {
      return;
    }
  // for most widgets
  QPoint globalPos = ui->listWidget->mapToGlobal(pos);
  // for QAbstractScrollArea and derived classes you would use:
  // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

  QMenu myMenu;
  myMenu.addAction("Open", this, SLOT(open_triger()));
  myMenu.addAction("Copy");
  myMenu.addAction("Move");
  myMenu.addAction("Paste");
  // ...

  QAction* selectedItem = myMenu.exec(globalPos);
  if (selectedItem)
    {
      clipboard.clear();
      for(int i=0; i<selected.size(); i++){
          qDebug() << selected[i]->text();
          clipboard.append(selected[i]->text());
        }
    }
  else
    {
      qDebug() << "else";
    }
}




//-----------------------------------------------------------------------------
// other functions
bool recursive_copy(QDir &src, QDir &dst)
{
  QDirIterator itDst(dst.path());
  QDirIterator itSrc(src.path());
  if(QFile(dst.path()+itSrc.fileInfo().fileName()).exists()){
      return false;
    }
  qDebug() << itDst.path()+itSrc.fileInfo().fileName();;
  if (itSrc.fileInfo().isDir()) {
      QString path = itDst.path()+itSrc.fileInfo().fileName();
      QDir().mkdir(path);
      QDirIterator it(QDir(src.path()));
      while(it.hasNext())
        {
          QDir next(it.next());
          if(!recursive_copy(next, QDir(path)))
            {
              return false;
            }
        }
    }
  else if (itSrc.fileInfo().isFile())
    {
      return QFile::copy(itSrc.path(), itDst.path()+itSrc.fileInfo().fileName());
    }
  else {
      qDebug() << "IDKWFGO";
      return false;
    }
  return true;
}


void MainWindow::cleanScrollArea(){
}


void MainWindow::clearView()
{
  while(ui->listWidget->count()>0){
      ui->listWidget->takeItem(0);
    }
}

//-----------------------------------------------------------------------------
// other functions


void MainWindow::open(QString path){
  qDebug() << path << currentIdx;
  QDirIterator it(path);
  it.next();
  if(it.fileInfo().isDir()){
      while(pathList.size()>currentIdx+1){
          pathList.removeAt(currentIdx+1);
        }

      lastDir = path;
      QModelIndex index = model->index(QString(lastDir));

      clearView();
      currentIdx++;
      pathList.append(path);
      ui->addressBar->setText(lastDir);
      int files = 0;
      int folders = 0;
      while(it.hasNext()){
          QFile f(it.next());
          if(it.fileName() == "." || it.fileName() == ".."){
              continue;
            }
          if(it.fileInfo().isDir()){
              ui->listWidget->addItem(f.fileName());
              folders++;
            } else {
              ui->listWidget->addItem(f.fileName());
              files++;
            }
        }
      ui->treeView->expand(index);
      ui->treeView->scrollTo(index);
      qDebug() << lastDir;
      ui->addressBar->setText(lastDir);
    } else {
      QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
    }
}

void MainWindow::on_pushButton_clicked()
{
  Progress *a = new Progress(0, QString("Move"));
  a->show();
}


void MainWindow::paste(){
  QThread a;
}

void MainWindow::on_listWidget_itemPressed(QListWidgetItem *item)
{
  on_listWidget_itemClicked(item);
}
