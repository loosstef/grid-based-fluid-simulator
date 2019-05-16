#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QDateTime>
#include "painttool.h"

const int INIT_SMOKE_BRUSH_SIZE = 10;
const double INIT_SMOKE_BRUSH_HARDNESS = 1;
const int INIT_WALL_BRUSH_SIZE = 2;
const double INIT_WALL_BRUSH_HARDNESS = 10;
const int INIT_TEMP_BRUSH_SIZE = 5;
const int INIT_TEMP_BRUSH_HARDNESS = 5;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void init();
    Ui::MainWindow *ui;
    int counter = 0;
    qint64 timer = QDateTime::currentMSecsSinceEpoch();
    QImage* currentImage = nullptr;
public slots:
    void updateSimulationVisualisation(QImage* image);
    void clicked(int x, int y);
    void rightClicked(int x, int simY, int viewX, int viewY);  //TODO: remove this function
    void pauseClicked();
    void saveImageClicked();
private slots:
    void updateTotalMass(double totalMass);
    void updateAvgTemp(double avgTemp);
    void brushTypeChanged(int index);
};

#endif // MAINWINDOW_H
