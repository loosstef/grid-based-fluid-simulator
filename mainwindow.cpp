#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "looper.h"
#include "simulationviewer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Init looper
    Looper* looper = new Looper();
    connect(looper, &Looper::FieldUpdated, this, &MainWindow::updateSimulationVisualisation);
    connect(looper, &Looper::FieldUpdated, ui->simulationVisualisator, &SimulationViewer::updateView);
    looper->start();

    ui->simulationVisualisator->setSize(400, 400);
    connect(ui->simulationVisualisator, &SimulationViewer::mouseLeftButtonDown, this, &MainWindow::clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSimulationVisualisation(QPixmap* pixmap)
{
    ++this->counter;
    qint64 diffTime = QDateTime::currentMSecsSinceEpoch() - this->timer;
    if(diffTime > 500) {
        ui->topInfo->setText(QString("fps: ").append(QString::number(((float)counter)/diffTime*1000)));
        this->timer = QDateTime::currentMSecsSinceEpoch();
        counter = 0;
    }
}

void MainWindow::clicked(int x, int y)
{
    ui->clickedPosViewer->setText(QString("Last clicked: (").append(QString::number(x)).append(QString(", ")).append(QString::number(y)).append(QString(')')));
}
