#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "looper.h"
#include "simulationviewer.h"
#include "simulationfield.h"
#include "painttool.h"
#include "simulationfieldcontroller.h"
#include "renderengine.h"
#include "renderenginecontroller.h"

const int FIELD_WIDTH = 100;
const int FIELD_HEIGHT = 100;
const int VIEWER_WIDTH = 800;
const int VIEWER_HEIGHT = 800;
const int INIT_BRUSH_SIZE = 10;
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
    PaintTool* paintTool = new PaintTool(simField->getSmokeDensityGrid());
    paintTool->setHardness(INIT_BRUSH_HARDNESS);
    paintTool->setSize(INIT_BRUSH_SIZE);

    ui->simulationVisualisator->setSize(FIELD_WIDTH, FIELD_HEIGHT, VIEWER_WIDTH, VIEWER_HEIGHT);

    SimulationFieldController* simViewConnector = simField->getController();
    simViewConnector->connectToViewer(ui->simulationVisualisator);
    simViewConnector->connectToSimSettingsCheckboxes(ui->forwardAdvectionToggle, ui->reverseAdvectionToggle, ui->pressureToggle);
    simViewConnector->connectToResetButton(ui->resetButton);

    // connect model and GUI
    connect(ui->simulationVisualisator, &SimulationViewer::mouseLeftButtonClicked, this, &MainWindow::clicked);
    connect(ui->simulationVisualisator, &SimulationViewer::mouseLeftButtonMoved, this, &MainWindow::clicked);
    connect(ui->simulationVisualisator, &SimulationViewer::mouseLeftButtonClicked, paintTool, &PaintTool::drawPoint);
    connect(ui->simulationVisualisator, &SimulationViewer::mouseLeftButtonMoved, paintTool, &PaintTool::drawPoint);

    connect(ui->simulationVisualisator, &SimulationViewer::mouseRightButtonClicked, this, &MainWindow::rightClicked);
    connect(ui->simulationVisualisator, &SimulationViewer::mouseRightButtonMoved, this, &MainWindow::rightClicked);

    RenderEngine* renderEngine = new RenderEngine(VIEWER_WIDTH, VIEWER_HEIGHT, false);
    RenderEngineController* RenderEngineController = renderEngine->getController();
    RenderEngineController->connectView(ui->velocityToggle);

    // init and start the looper
    Looper* looper = new Looper(simField, renderEngine);
    connect(looper, &Looper::FieldUpdated, this, &MainWindow::updateSimulationVisualisation);
    looper->start();
}

void MainWindow::updateSimulationVisualisation(QImage* image)
{
    ui->simulationVisualisator->updateView(image);
    delete image;
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
    QString clickedPosText = QString("Left: (").append(QString::number(x)).append(", ").append(QString::number(y).append(")"));
    ui->clickedPosViewer->setText(clickedPosText);
}

void MainWindow::rightClicked(int x, int simY, int viewX, int viewY)
{
    QString clickedPosText = QString("Right: (").append(QString::number(x).append(", ").append(QString::number(simY)).append(")"));
    ui->rightClickedPosViewer->setText(clickedPosText);
}
