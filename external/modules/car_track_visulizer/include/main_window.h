#ifndef QT_VISULATION_MAIN_WINDOW_H
#define QT_VISULATION_MAIN_WINDOW_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QSplineSeries>
#include "iostream"

class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    void scaleChart(QtCharts::QChart *chart, QtCharts::QSplineSeries *series,const bool sameAxes = false);
    void scaleChart(QtCharts::QChart *chart, std::vector<QtCharts::QSplineSeries*> series_,const bool sameAxes = false);
public:
    MainWindow();
    QtCharts::QSplineSeries *positionSeries,*carRectAngleSeries;
    QtCharts::QChart *positionChart;
    QtCharts::QSplineSeries *velocitySeries;
    QtCharts::QSplineSeries *targetVelocitySeries;
    QtCharts::QChart *velocityChart;
    QtCharts::QSplineSeries *steeringFrontSeries,*steeringRearSeries;
    QtCharts::QChart *steeringChart;

    QGroupBox *gridGroupBox;
    void addPoint(const float x,const float y,const float theta);
    void addVelocity(const float velocity,const float targetVelocity);
    void addSteering(const float front, const float rear);
    bool collectData;

};

#endif // QT_VISULATION_MAIN_WINDOW_H
