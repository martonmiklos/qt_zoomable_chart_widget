#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QT_CHARTS_USE_NAMESPACE
#endif

//![1]
class ZoomableChartView : public QChartView
        //![1]
{
public:
    enum ZoomMode {
        Pan,
        RectangleZoom,
        VerticalZoom,
        HorizontalZoom
    };

    ZoomableChartView(QWidget *parent = 0);
    ZoomableChartView(QChart *chart, QWidget *parent = nullptr);

    void zoomX(qreal factor, qreal xcenter);
    void zoomX(qreal factor);

    void zoomY(qreal factor, qreal ycenter);
    void zoomY(qreal factor);

    //![2]
    ZoomMode zoomMode() const;
    void setZoomMode(const ZoomMode &zoomMode);


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    //![2]

private:

    bool m_isTouching = false;
    QPointF m_lastMousePos;
    ZoomMode m_zoomMode = RectangleZoom;

    static bool isAxisTypeZoomableWithMouse(const QAbstractAxis::AxisType type);
    QPointF getSeriesCoordFromChartCoord(const QPointF & mousePos, QAbstractSeries *series) const;
    QPointF getChartCoordFromSeriesCoord(const QPointF & seriesPos, QAbstractSeries *series) const;
};

#endif
