#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>

#include "looper.h"
#include "simulationviewer.h"
#include "simulationfield.h"
#include "simulationfieldcontroller.h"
#include "renderengine.h"
#include "renderenginecontroller.h"
#include "io.h"
#include "painthandler.h"

const int FIELD_WIDTH = 120;
const int FIELD_HEIGHT = 120;
const int VIEWER_WIDTH = 800;
const int VIEWER_HEIGHT = 800;
const double INIT_VELOCITY_SCALE = 10;
const int INIT_VELOCITY_SPARSENESS = 1;
const int INIT_SLEEP_PER_LOOP = 0;

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
    PaintHandler* paintHandler = new PaintHandler(simField);
    paintHandler->connectToSettings(ui->paintTypeSelector, ui->smokeBrushSize, ui->smokeBrushHardness,
                                    ui->wallBrushSize, ui->wallBrushHardness, ui->tempBrushSize, ui->tempBrushHardness);
    paintHandler->connectToSimulationViewer(ui->simulationVisualisator);
    ui->smokeBrushSize->setValue(INIT_SMOKE_BRUSH_SIZE);
    ui->smokeBrushHardness->setValue(INIT_SMOKE_BRUSH_HARDNESS);
    ui->wallBrushSize->setValue(INIT_WALL_BRUSH_SIZE);
    ui->wallBrushHardness->setValue(INIT_WALL_BRUSH_HARDNESS);
    ui->tempBrushSize->setValue(INIT_TEMP_BRUSH_SIZE);
    ui->tempBrushHardness->setValue(INIT_TEMP_BRUSH_HARDNESS);

    ui->simulationVisualisator->setSize(FIELD_WIDTH, FIELD_HEIGHT, VIEWER_WIDTH, VIEWER_HEIGHT);

    SimulationFieldController* simViewConnector = simField->getController();
    simViewConnector->connectToSimSettingsCheckboxes(ui->forwardAdvectionToggle, ui->reverseAdvectionToggle, ui->pressureToggle, ui->diffusionToggle);
    simViewConnector->connectToResetButton(ui->resetButton);
    simViewConnector->connectEdgeCaseSelector(ui->edgeCaseSelector);

    // connect model and GUI
    connect(ui->simulationVisualisator, &SimulationViewer::mouseLeftButtonClicked, this, &MainWindow::clicked);
    connect(ui->simulationVisualisator, &SimulationViewer::mouseLeftButtonMoved, this, &MainWindow::clicked);
    connect(ui->simulationVisualisator, &SimulationViewer::mouseRightButtonClicked, this, &MainWindow::rightClicked);
    connect(ui->simulationVisualisator, &SimulationViewer::mouseRightButtonMoved, this, &MainWindow::rightClicked);

    //remove unnecesary brush-settings
    ui->wallBrushForm->hide();
    ui->tempBrushForm->hide();

    connect(ui->paintTypeSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::brushTypeChanged);

    // connect debug data
    connect(simField, &SimulationField::totalMassCalculated, this, &MainWindow::updateTotalMass);
    connect(simField, &SimulationField::avgTempCalculated, this, &MainWindow::updateAvgTemp);

    RenderEngine* renderEngine = new RenderEngine(VIEWER_WIDTH, VIEWER_HEIGHT, false);
    RenderEngineController* renderEngineController = renderEngine->getController();
    renderEngineController->connectView(ui->velocityToggle, ui->velocitySize, ui->sparsnessInput);
    renderEngineController->connectRenderTypeSelector(ui->renderTypeSelector);

    ui->velocitySize->setValue(INIT_VELOCITY_SCALE);
    ui->sparsnessInput->setValue(INIT_VELOCITY_SPARSENESS);

    // init and start the looper
    Looper* looper = new Looper(simField, renderEngine);
    connect(looper, &Looper::FieldUpdated, this, &MainWindow::updateSimulationVisualisation);
    connect(ui->pauseButton, &QPushButton::released, looper, &Looper::toggleRunning);
    connect(ui->pauseButton, &QPushButton::released, this, &MainWindow::pauseClicked);

    connect(ui->saveImageButton, &QPushButton::released, this, &MainWindow::saveImageClicked);
    connect(ui->sleepPerLoopInput, qOverload<int>(&QSpinBox::valueChanged), looper, &Looper::setSleepPerLoop);
    ui->sleepPerLoopInput->setValue(INIT_SLEEP_PER_LOOP);

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
        ui->topInfo->setText(QString("fps: ").append(QString::number(((double)counter)/diffTime*1000)));
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

void MainWindow::updateTotalMass(double totalMass)
{
    ui->totalMassLabel->setText(QString::number(totalMass));
}

void MainWindow::updateAvgTemp(double avgTemp)
{
    ui->avgTempLabel->setText(QString::number(avgTemp));
}

void MainWindow::brushTypeChanged(int index)
{
    if(index == 0) {
        ui->wallBrushForm->hide();
        ui->tempBrushForm->hide();
        ui->smokeBrushForm->show();
    }
    else if(index == 1) {
        ui->smokeBrushForm->hide();
        ui->tempBrushForm->hide();
        ui->wallBrushForm->show();
    }
    else if(index == 2) {
        ui->smokeBrushForm->hide();
        ui->wallBrushForm->hide();
        ui->tempBrushForm->show();
    }
}
