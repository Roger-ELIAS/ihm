#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>

#include "editionwindow.h"
#include "ui_mainwindow.h"

#include "filepropertieswindow.h"

#include "pathvisit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setStatusBar();
    bool updateCurrentPath(QString);

private slots:
    void on_elementListView_doubleClicked(const QModelIndex &index);
    void on_dirTreeView_clicked(const QModelIndex &index);
    void openEditor();
    void openDirectory();
    void addToAlbum();
    void informations();
    void eraseItem();
    bool removeDirectory(QString dirPath = "");
    void on_lePath_returnPressed();
    void showContextMenu(const QPoint&);
    void on_pushButton_2_clicked(); //test fleche gauche
    void on_pushButton_clicked(); //test fleche droite

private:
    Ui::MainWindow *ui;
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
    QString actualFile;
    PathVisit *pathVisit;
};
#endif // MAINWINDOW_H
