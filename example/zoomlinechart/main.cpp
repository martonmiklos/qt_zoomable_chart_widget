// based on https://doc.qt.io/Qt-6/qtcharts-zoomlinechart-example.html 

#include <QApplication>
#include <QLineSeries>
#include <QMainWindow>
#include <QRandomGenerator>
#include <QtMath>
#include <QValueAxis>

#include <zoomablechart.h>
#include <zoomablechartview.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto series = new QLineSeries;
    for (int i = 0; i < 500; i++) {
        QPointF p((qreal) i, qSin(M_PI / 50 * i) * 100);
        p.ry() += QRandomGenerator::global()->bounded(20);
        *series << p;
    }

    auto chart = new ZoomableChart;
    chart->addSeries(series);
    chart->setTitle("Zoom in/out example");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->hide();
    chart->createDefaultAxes();


    QMainWindow window;
    window.resize(800, 600);
    window.grabGesture(Qt::PanGesture);
    window.grabGesture(Qt::PinchGesture);

    auto chartView = new ZoomableChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    window.setCentralWidget(chartView);

    window.show();

    return a.exec();
}
