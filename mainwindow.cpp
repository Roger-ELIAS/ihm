#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>

#include <QStandardPaths>

#include <database.h>
#include <QLabel>
#include <QPushButton>

#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString mainPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);


    /*******************/
    QSplitter *splitter = new QSplitter(); //CQStandardPaths::writableLocation(QStandardPaths::PicturesLocation)réation d'un splitter, permettant de redimensionner l'espace occupé par les widgets enfants
    splitter->setOrientation(Qt::Vertical); //Mettre l'orientation verticale
    ui->leftLayout->addWidget(splitter);
    QWidget *w1 = new QWidget();
    ui->lAlbums->setParent(nullptr);
    w1->setLayout(ui->lAlbums);
    QWidget *w2 = new QWidget();
    ui->lTree->setParent(nullptr);
    w2->setLayout(ui->lTree);
    splitter->addWidget(w2);
    splitter->addWidget(w1);

    ui->lAlbums->setAlignment(Qt::AlignTop);
    /*******************/


    dirModel = new QFileSystemModel(this);
    dirModel->setRootPath(mainPath);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    ui->dirTreeView->setModel(dirModel);
//    ui->treeView->setStyleSheet("color:white;");
    ui->dirTreeView->setHeaderHidden(true);
    for (int i = 1; i < dirModel->columnCount(); ++i)
        ui->dirTreeView->hideColumn(i);


    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs);
    QStringList sDriveFilters;
    sDriveFilters << "*.png" << "*.jpeg" << "*.jpg" << "*.gif" << "*.bmp" << "*.jpe" << "*.jfif" << "*.rle" << "*.tga" << "*.tif" << "*.tiff" ;
    fileModel->setNameFilters(sDriveFilters);
    fileModel->setNameFilterDisables(false);
    fileModel->setRootPath(mainPath);
    ui->elementListView->setModel(fileModel);

    ui->lePath->setText(mainPath);



    /*****
     * Initialisation de la liste en bas à gauche de la main windows
     * Ceci est codé en dur pour le moment (pas de bdd)
     * Il n'y a pas de limite à l'expension du layout (en hauteur)
     * ça va prendre une place énorme si on ajoute trop d'élément.
     * Il va faloir réfléchir au scroll et à la position du bouton x...
     */
    for(int i=0; i<5; i++) {
        // Création du sous-layout horizontal => label + bouton
        QLayout *layoutTest = new QHBoxLayout();

        // Création du label
        QLabel *testLabel = new QLabel();
        testLabel->setText("Test " + QString::number(i));
        // Ajout du label au sous-layout
        layoutTest->addWidget(testLabel);

        // Création du bouton
        QPushButton *test = new QPushButton();
        test->setText("x");
        test->setMaximumSize(20,20);
        test->setStyleSheet("color:red;");
        //todo : ajouter l'intération
        // Ajout du bouton au sous-layout
        layoutTest->addWidget(test);

        // Ajout du sous-layout au layout vertical de l'UI
        ui->vlAlbums->addLayout(layoutTest);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_elementListView_doubleClicked(const QModelIndex &index)
{
    QString dirPath = dirModel->fileInfo(index).absoluteFilePath();

    ui->elementListView->setRootIndex(fileModel->setRootPath(dirPath));
    ui->dirTreeView->setExpanded(dirModel->setRootPath(dirPath), true);
    //ui->dirTreeView->collapse(dirModel->setRootPath(dirPath));
    ui->dirTreeView->setCurrentIndex(dirModel->setRootPath(dirPath));

    QDir pathDir(dirPath);
    if (pathDir.exists()) {
        qDebug() << "DIR " + dirPath;
        ui->lePath->setText(dirPath);//condition si dossier ou si fichier
    } else {
        qDebug() << "FILE " + dirPath;

        EditionWindow w;
        w.setImage(dirPath);
        w.createContents();
        w.show();

        QEventLoop eventLoop;
        eventLoop.exec();

    }
}

void MainWindow::on_dirTreeView_clicked(const QModelIndex &index)
{
    QString dirPath = dirModel->fileInfo(index).absoluteFilePath();
    ui->elementListView->setRootIndex(fileModel->setRootPath(dirPath));
    ui->dirTreeView->setExpanded(dirModel->setRootPath(dirPath), true);
    //ui->dirTreeView->collapse(dirModel->setRootPath(dirPath));
    ui->dirTreeView->setCurrentIndex(dirModel->setRootPath(dirPath));
    ui->lePath->setText(dirPath);
}

void MainWindow::on_lePath_returnPressed()
{
    QString dirPath = ui->lePath->text();
    QDir pathDir(dirPath);

    if (pathDir.exists()) {
        qDebug() << "DIR " + dirPath;

        ui->elementListView->setRootIndex(fileModel->setRootPath(dirPath));
        ui->dirTreeView->setExpanded(dirModel->setRootPath(dirPath), true);
        ui->dirTreeView->setCurrentIndex(dirModel->setRootPath(dirPath));
    }
}

void MainWindow::setStatusBar() {
    QFrame *statusFrame = new QFrame();
    statusFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    statusFrame->setStyleSheet("background-color: rgb(0, 0, 0);"
                               "border: 0px;"
                               "margin: 0px;"
                               "padding: 0px;");

    QHBoxLayout *layout = new QHBoxLayout(statusFrame);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QLabel *statusMessage = new QLabel("0 élement selectionné", statusFrame);
    statusMessage->setStyleSheet("color: white");

    QFrame *frame = new QFrame(statusFrame);
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    frame->setStyleSheet("background-color: rgb(0, 0, 0);"
                         "border: 0px;"
                         "margin: 0px;"
                         "padding: 0px;");

    QPushButton *liste = new QPushButton("Liste", statusFrame);
    liste->setStyleSheet("background-color: rgb(51, 51, 51);"
                         "color: rgb(140, 140, 140);"
                         "border: 0px;"
                         "margin: 0px;"
                         "padding: 0px;");
    QPushButton *icone = new QPushButton("Icone", statusFrame);
    icone->setStyleSheet("background-color: rgb(51, 51, 51);"
                         "color: rgb(140, 140, 140);"
                         "border: 0px;"
                         "margin: 0px;"
                         "padding: 0px;");

    layout->addWidget(statusMessage);
    layout->addWidget(frame);
    layout->addWidget(liste);
    layout->addWidget(icone);

    ui->statusbar->addWidget(statusFrame, 1);
    ui->statusbar->setStyleSheet("background-color: rgb(0,0,0);");
}
