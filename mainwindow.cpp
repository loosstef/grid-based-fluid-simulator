#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>

#include "looper.h"
#include "simulationviewer.h"
#include "simulationfield.h"
#include "painttool.h"
#include "simulationfieldcontroller.h"
#include "renderengine.h"
#include "renderenginecontroller.h"
#include "painttoolcontroller.h"
#include "io.h"

const int FIELD_WIDTH = 150;
const int FIELD_HEIGHT = 150;
const int VIEWER_WIDTH = 800;
const int VIEWER_HEIGHT = 800;
const int INIT_BRUSH_SIZE = 10;
const int INIT_BRUSH_HARDNESS = 1;
const double INIT_VELOCITY_SCALE = 10;

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
    PaintToolController* paintToolController = paintTool->getController();
    paintToolController->connectToBrushSettings(ui->brushSize, ui->brushHardness);
    ui->brushSize->setValue(INIT_BRUSH_SIZE);
    ui->brushHardness->setValue(INIT_BRUSH_HARDNESS);

    ui->simulationVisualisator->setSize(FIELD_WIDTH, FIELD_HEIGHT, VIEWER_WIDTH, VIEWER_HEIGHT);

    SimulationFieldController* simViewConnector = simField->getController();
    simViewConnector->connectToViewer(ui->simulationVisualisator);
    simViewConnector->connectToSimSettingsCheckboxes(ui->forwardAdvectionToggle, ui->reverseAdvectionToggle, ui->pressureToggle, ui->diffusionToggle);
    simViewConnector->connectToResetButton(ui->resetButton);
    simViewConnector->connectEdgeCaseSelector(ui->edgeCaseSelector);

    // connect model and GUI
    connect(ui->simulationVisualisator, &SimulationViewer::mouseLeftButtonClicked, this, &MainWindow::clicked);
    connect(ui->simulationVisualisator, &SimulationViewer::mouseLeftButtonMoved, this, &MainWindow::clicked);
    connect(ui->simulationVisualisator, &SimulationViewer::mouseLeftButtonClicked, paintTool, &PaintTool::drawPoint);
    connect(ui->simulationVisualisator, &SimulationViewer::mouseLeftButtonMoved, paintTool, &PaintTool::drawPoint);

    connect(ui->simulationVisualisator, &SimulationViewer::mouseRightButtonClicked, this, &MainWindow::rightClicked);
    connect(ui->simulationVisualisator, &SimulationViewer::mouseRightButtonMoved, this, &MainWindow::rightClicked);

    RenderEngine* renderEngine = new RenderEngine(VIEWER_WIDTH, VIEWER_HEIGHT, false);
    RenderEngineController* renderEngineController = renderEngine->getController();
    renderEngineController->connectView(ui->velocityToggle, ui->velocitySize);
    renderEngineController->connectRenderTypeSelector(ui->renderTypeSelector);

    ui->velocitySize->setValue(INIT_VELOCITY_SCALE);

    // init and start the looper
    Looper* looper = new Looper(simField, renderEngine);
    connect(looper, &Looper::FieldUpdated, this, &MainWindow::updateSimulationVisualisation);
    connect(ui->pauseButton, &QPushButton::released, looper, &Looper::toggleRunning);
    connect(ui->pauseButton, &QPushButton::released, this, &MainWindow::pauseClicked);

    connect(ui->saveImageButton, &QPushButton::released, this, &MainWindow::saveImageClicked);
    looper->start();
}

void MainWindow::updateSimulationVisualisation(QImage* image)
{
    QImage* lastImage = this->currentImage;
    this->currentImage = image;
    delete lastImage;
    ui->simulationVisualisator->updateView(image);
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

void MainWindow::pauseClicked()
{
    if(ui->pauseButton->text().compare(QString("Pause")) == 0) {
        ui->pauseButton->setText(QString("Play"));
    } else {
        ui->pauseButton->setText(QString("Pause"));
    }
}

void MainWindow::saveImageClicked()
{
    QString fileName = ui->fileNameInput->displayText();
    IO::saveImage(fileName, this->currentImage);
}
