#include <QtWidgets>

#include "main_window.h"
#include <QValueAxis>
#include <QAction>
#include <iostream>
#include <functional>
#include <string>
#include "lms/logger.h"

MainWindow::MainWindow(){
    this->collectData = true;
    //The central Widget for the window
    QWidget *central = new QWidget(this);
    //create the layout for the central widget
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QGridLayout *layout = new QGridLayout;

    QPushButton *reset = new QPushButton();
    reset->setText(tr("reset"));
    QPushButton *stop = new QPushButton();
    stop->setText(tr("stop"));
    // Connect buttons
    connect(reset, &QPushButton::clicked, [this](){
        this->positionSeries->clear();
        this->velocitySeries->clear();
        this->steeringFrontSeries->clear();
        this->steeringRearSeries->clear();
        this->carRectAngleSeries->clear();
    });
    connect(stop, &QPushButton::clicked, [this](){
        this->collectData = !collectData;
    });
    gridGroupBox = new QGroupBox(tr("Overview"));
    //position
    positionChart = new QtCharts::QChart();
    positionSeries = new QtCharts::QSplineSeries();
    carRectAngleSeries = new QtCharts::QSplineSeries();
    positionChart->legend()->hide();
    positionChart->addSeries(positionSeries);
    positionChart->addSeries(carRectAngleSeries);
    positionChart->setTitle("position in meter");
    positionChart->createDefaultAxes();
    QtCharts::QChartView *chartView = new QtCharts::QChartView(positionChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView,0,0,1,2);
    //steering
    steeringChart = new QtCharts::QChart();
    steeringFrontSeries = new QtCharts::QSplineSeries();
    steeringRearSeries = new QtCharts::QSplineSeries();
    steeringChart->legend()->setVisible(true);
    //    m_chart->legend()->setVisible(true);
    //m_chart->legend()->setAlignment(Qt::AlignBottom);
    steeringFrontSeries->setName("front");
    steeringRearSeries->setName("rear");
    steeringChart->addSeries(steeringFrontSeries);
    steeringChart->addSeries(steeringRearSeries);
    steeringChart->setTitle("steering in degree");
    steeringChart->createDefaultAxes();
    chartView = new QtCharts::QChartView(steeringChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView,1,0,1,1);
    //velocity
    velocityChart = new QtCharts::QChart();
    velocitySeries = new QtCharts::QSplineSeries();
    velocitySeries->setName("current velocity");
    targetVelocitySeries = new QtCharts::QSplineSeries();
    targetVelocitySeries->setName("target velocity");
    //velocityChart->legend()->hide();
    velocityChart->addSeries(velocitySeries);
    velocityChart->addSeries(targetVelocitySeries);
    velocityChart->setTitle("velocity in m/s");
    velocityChart->createDefaultAxes();
    chartView = new QtCharts::QChartView(velocityChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView,1,1,1,1);


    gridGroupBox->setLayout(layout);
    mainLayout->addWidget(stop);
    mainLayout->addWidget(reset);
    mainLayout->addWidget(gridGroupBox);
    //set the central widget
    central->setLayout(mainLayout);
    setCentralWidget(central);

    setWindowTitle(tr("Car Tracker"));
}


void MainWindow::addPoint(const float x,const float y, const float theta){
    if(!collectData)
        return;
    positionSeries->append(x,y);
    carRectAngleSeries->clear();
    float lineLength = 1;
    carRectAngleSeries->append(x-cos(theta)*lineLength,y-sin(theta)*lineLength);
    carRectAngleSeries->append(x+cos(theta)*lineLength,y+sin(theta)*lineLength);

    scaleChart(positionChart,positionSeries,true);
}


void MainWindow::addVelocity(const float velocity, const float targetVelocity){
    if(!collectData)
        return;
    velocitySeries->append(velocitySeries->count(),velocity);
    targetVelocitySeries->append(targetVelocitySeries->count(),targetVelocity);
    std::vector<QtCharts::QSplineSeries*> series_;
    series_.push_back(velocitySeries);
    series_.push_back(targetVelocitySeries);
    scaleChart(velocityChart,series_);
}

void MainWindow::addSteering(const float front, const float rear){
    if(!collectData)
        return;
    steeringFrontSeries->append(steeringFrontSeries->count(),front);
    steeringRearSeries->append(steeringRearSeries->count(),rear);
    std::vector<QtCharts::QSplineSeries*> series_;
    series_.push_back(steeringFrontSeries);
    series_.push_back(steeringRearSeries);
    scaleChart(steeringChart,series_); //TODO use both


}

void MainWindow::scaleChart(QtCharts::QChart *chart, std::vector<QtCharts::QSplineSeries*> series_,const bool sameAxes){
    float xMin = std::numeric_limits<float>::max(); // everything is <= this
    float xMax = std::numeric_limits<float>::min(); // everything is >= this
    float yMin = std::numeric_limits<float>::max();
    float yMax = std::numeric_limits<float>::min();
    for(QtCharts::QSplineSeries* series:series_){
        for (QPointF &p: series->points()) {
            xMin = qMin<float>(xMin, p.x());
            xMax = qMax<float>(xMax, p.x());
            yMin = qMin<float>(yMin, p.y());
            yMax = qMax<float>(yMax, p.y());
        }
    }
    if(sameAxes){
        xMin = qMin<float>(xMin, yMin);
        xMax = qMin<float>(xMax, yMax);
        yMin = xMin;
        yMax = xMax;
    }
    chart->axisX()->setRange(xMin-1,xMax+1);
    chart->axisY()->setRange(yMin-1,yMax+1);

}

void MainWindow::scaleChart(QtCharts::QChart *chart, QtCharts::QSplineSeries *series, const bool sameAxes){
    float xMin = std::numeric_limits<float>::max(); // everything is <= this
    float xMax = std::numeric_limits<float>::min(); // everything is >= this
    float yMin = std::numeric_limits<float>::max();
    float yMax = std::numeric_limits<float>::min();
    for (QPointF &p: series->points()) {
        xMin = qMin<float>(xMin, p.x());
        xMax = qMax<float>(xMax, p.x());
        yMin = qMin<float>(yMin, p.y());
        yMax = qMax<float>(yMax, p.y());
    }
    if(sameAxes){
        xMin = qMin<float>(xMin, yMin);
        xMax = qMin<float>(xMax, yMax);
        yMin = xMin;
        yMax = xMax;
    }
    chart->axisX()->setRange(xMin-1,xMax+1);
    chart->axisY()->setRange(yMin-1,yMax+1);
}
