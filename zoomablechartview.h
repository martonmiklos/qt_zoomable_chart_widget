#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>

QT_CHARTS_USE_NAMESPACE

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

    void zoomX(qreal factor, qreal xcenter);
    void zoomX(qreal factor);

    void zoomY(qreal factor, qreal ycenter);
    void zoomY(qreal factor);

    //![2]
    ZoomMode zoomMode() const;
    void setZoomMode(const ZoomMode &zoomMode);


protected:
    bool viewportEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    //![2]

private:

    bool m_isTouching = false;
    QPoint m_lastMousePos;
    ZoomMode m_zoomMode = RectangleZoom;

    static bool isAxisTypeZoomableWithMouse(const QAbstractAxis::AxisType type);
    QPointF getSeriesCoordFromChartCoord(const QPointF & mousePos, QAbstractSeries *series) const;
    QPointF getChartCoordFromSeriesCoord(const QPointF & seriesPos, QAbstractSeries *series) const;
};

#endif
