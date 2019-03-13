#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "looper.h"
#include "simulationviewer.h"
#include "simulationfield.h"
#include "painttool.h"

const int FIELD_WIDTH = 2;
const int FIELD_HEIGHT = 2;
const int VIEWER_WIDTH = 400;
const int VIEWER_HEIGHT = 400;
const int INIT_BRUSH_SIZE = 1;
const int INIT_BRUSH_HARDNESS = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * Init model and gui
 * @brief MainWindow::init
 */
void MainWindow::init()
{
    // init model-objects
    SimulationField* simField = new SimulationField(FIELD_WIDTH, FIELD_HEIGHT);
    PaintTool* paintTool = new PaintTool(simField->getDensityGrid());
    paintTool->setHardness(INIT_BRUSH_HARDNESS);
    paintTool->setSize(INIT_BRUSH_SIZE);

    // init the looper
    Looper* looper = new Looper(simField);
    connect(looper, &Looper::FieldUpdated, this, &MainWindow::updateSimulationVisualisation);
    connect(looper, &Looper::FieldUpdated, ui->simulationVisualisator, &SimulationViewer::updateView);
    looper->start();

    // connect model and GUI
    ui->simulationVisualisator->setSize(VIEWER_WIDTH, VIEWER_HEIGHT);
    connect(ui->simulationVisualisator, &SimulationViewer::mouseLeftButtonClicked, this, &MainWindow::clicked);
    connect(ui->simulationVisualisator, &SimulationViewer::mouseLeftButtonMoved, this, &MainWindow::clicked);
    connect(ui->simulationVisualisator, &SimulationViewer::mouseLeftButtonClicked, paintTool, &PaintTool::drawPoint);
    connect(ui->simulationVisualisator, &SimulationViewer::mouseLeftButtonMoved, paintTool, &PaintTool::drawPoint);
}

void MainWindow::updateSimulationVisualisation(QImage* image)
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
