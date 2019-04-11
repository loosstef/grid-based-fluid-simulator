#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QDateTime>
#include "painttool.h"

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
};

#endif // MAINWINDOW_H
