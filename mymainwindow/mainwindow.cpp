#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 初始化文件为未保存状态
    isUntitled = true;
    // 初始化文件名为"未命名.txt"
    curFile = tr("unnamed.txt");
    // 初始化窗口标题为文件名
    setWindowTitle(curFile);
/*
    //创建新动作
    QAction *openAction = new QAction(tr("&Open"),this);
    //添加图标
    QIcon icon(":/myimages/images/open.png");
    openAction->setIcon(icon);
    //设置快捷键
    openAction->setShortcut(QKeySequence(tr("Ctrl+O")));
    //文件菜单中添加 open动作
    ui->menu_N->addAction(openAction);

    //创建新动作
    QAction *zoomAction = new QAction(tr("&Full"),this);
    //添加图标
    QIcon iconZoom(":/myimages/images/save_archive.png");
    zoomAction->setIcon(iconZoom);
    //设置快捷键
    zoomAction->setShortcut(QKeySequence(tr("Ctrl+F")));
    //文件菜单中添加 zoom动作
    ui->menu_N->addAction(zoomAction);

    //创建新动作
    QAction *checkAction = new QAction(tr("&Check"),this);
    //添加图标
    QIcon iconCheck(":/myimages/images/save_archive.png");
    zoomAction->setIcon(iconZoom);
    //设置快捷键
    checkAction->setShortcut(QKeySequence(tr("Ctrl+C")));
    //文件菜单中添加 check动作
    ui->menu_N->addAction(checkAction);

    //创建新动作
    QAction *closeAction = new QAction(tr("&Close"),this);
    //添加图标
    QIcon iconClose(":/myimages/images/close.png");
    closeAction->setIcon(iconClose);
    //设置快捷键
    closeAction->setShortcut(QKeySequence(tr("Ctrl+X")));
    //文件菜单中添加 close动作
    ui->menu_N->addAction(closeAction);

    //添加新菜单
    QMenu *menu_E = ui->menuBar->addMenu(tr("Edit(&E)"));
    QAction *findAction = new QAction(tr("&Find"),this);
    QIcon iconFind(":/myimages/images/find.png");
    findAction->setIcon(iconFind);
    findAction->setShortcut(QKeySequence(tr("Ctrl+F")));
    menu_E->addAction(findAction);

    connect(zoomAction,&QAction::triggered,this,&MainWindow::showMaximized);
    connect(checkAction,&QAction::triggered,this,&MainWindow::check);
    connect(closeAction,&QAction::triggered,this,&MainWindow::close);
    */
}

void MainWindow::check()
{
    QMessageBox::warning(this,tr("warn"),tr("check for demo"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 新建操作
void MainWindow::newFile()
{
    if(maybeSave()){
        isUntitled = true;
        curFile = tr("unnamed.txt");
        setWindowTitle(curFile);
        ui->textEdit->clear();
        ui->textEdit->setVisible(true);
    }
}

// 判断是否需要保存
bool MainWindow::maybeSave()
{
    //如果文档更改了
    if(ui->textEdit->document()->isModified()){
        QMessageBox box;
        box.setWindowTitle(tr("waring"));
        box.setIcon(QMessageBox::Warning);
        box.setText(curFile+tr(" unsaved,save ?"));
        QPushButton* yes = box.addButton(tr("yes(&Y)"),QMessageBox::YesRole);
        box.addButton(tr("no(&N)"),QMessageBox::NoRole);
        QPushButton* cancel = box.addButton(tr("cancel(&C)"),QMessageBox::RejectRole);
        box.exec();
        if(box.clickedButton() == yes){
            return save();
        }else if(box.clickedButton() == cancel){
            return false;
        }
    }
    //如果文档没有更改
    return true;
}

// 保存操作
bool MainWindow::save()
{
    if(isUntitled){
        return saveAs();
    }else{
        return saveFile(curFile);
    }
}

// 另存为操作
bool MainWindow::saveAs()
{
    QString file = QFileDialog::getSaveFileName(this,tr("saveas"),curFile);
    if(file.isEmpty()){
        return false;
    }

    return saveFile(file);
}

// 保存文件
bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);

    if(!file.open(QFile::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,tr("text editor"),tr("can't save file %1 /n %2").arg(fileName).arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << ui->textEdit->toPlainText();
    isUntitled =false;
    QApplication::restoreOverrideCursor();
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}

// 加载文件
bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly|QFile::Text)){
        QMessageBox::warning(this,tr("text editor"),tr("can't read file %1 /n %2").arg(fileName).arg(file.errorString()));
        return false;
    }
    //新建文本流对象
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //读取文件的全部内容并设置到编辑器中
    ui->textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    //设置当前文件名
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}

void MainWindow::on_action_New_triggered()
{
    newFile();
}

void MainWindow::on_action_Save_triggered()
{
    save();
}

void MainWindow::on_action_SaveAs_triggered()
{
    saveAs();
}

void MainWindow::on_action_Open_triggered()
{
    if(maybeSave()){
        QString fileName = QFileDialog::getOpenFileName(this);
        if(!fileName.isEmpty()){
            loadFile(fileName);
            ui->textEdit->setVisible(true);
        }
    }
}

void MainWindow::on_action_Close_triggered()
{
    if(maybeSave()){
        ui->textEdit->setVisible(false);
    }
}

//退出动作
void MainWindow::on_action_Exit_triggered()
{
    //先关闭文件，在退出程序
    on_action_Close_triggered();
    qApp->quit();
}


void MainWindow::on_action_Undo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_action_Cut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_action_Copy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_action_Paste_triggered()
{
    ui->textEdit->paste();
}

// 关闭事件
void MainWindow::closeEvent(QCloseEvent *event)
{
    //关闭之前执行修改保存操作，如已完成则退出
    if(maybeSave()){
        event->accept();
    }else{
        event->ignore();
    }
}
