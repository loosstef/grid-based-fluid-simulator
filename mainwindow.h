#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QPixmap"
#include <QDateTime>

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
    Ui::MainWindow *ui;
    int counter = 0;
    qint64 timer = QDateTime::currentMSecsSinceEpoch();
public slots:
    void updateSimulationVisualisation(QPixmap* pixmap);
    void clicked(int x, int y);
};

#endif // MAINWINDOW_H
