#include "zoomablechartview.h"
#include <QtGui/QMouseEvent>

ZoomableChartView::ZoomableChartView(QWidget *parent) :
    QChartView(parent)
{
    setRubberBand(QChartView::RectangleRubberBand);
}

bool ZoomableChartView::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::TouchBegin) {
        // By default touch events are converted to mouse events. So
        // after this event we will get a mouse event also but we want
        // to handle touch events as gestures only. So we need this safeguard
        // to block mouse events that are actually generated from touch.
        m_isTouching = true;
    }
    return QChartView::viewportEvent(event);
}

void ZoomableChartView::mousePressEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;
    QChartView::mousePressEvent(event);
}

void ZoomableChartView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;

    if (dragMode() == ScrollHandDrag) {
        if (event->buttons() & Qt::LeftButton) {
            qreal dx = -(event->pos().x() - m_lastMousePos.x());
            chart()->scroll(dx, 0);
        }
        m_lastMousePos = event->pos();
    }

    QChartView::mouseMoveEvent(event);
}

void ZoomableChartView::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0) {
        zoomX(2, event->pos().x() - chart()->plotArea().x());
    } else if (event->angleDelta().y() < 0) {
        zoomX(0.5, event->pos().x() - chart()->plotArea().x());
    }
}

ZoomableChartView::ZoomMode ZoomableChartView::zoomMode() const
{
    return m_zoomMode;
}

void ZoomableChartView::setZoomMode(const ZoomMode &zoomMode)
{
    if (m_zoomMode != zoomMode) {
        m_zoomMode = zoomMode;
        switch (zoomMode) {
        case Pan:
            setRubberBand(QChartView::NoRubberBand);
            setDragMode(QChartView::ScrollHandDrag);
            break;
        case RectangleZoom:
            setRubberBand(QChartView::RectangleRubberBand);
            setDragMode(QChartView::NoDrag);
            break;
        case HorizontalZoom:
            setRubberBand(QChartView::HorizontalRubberBand);
            setDragMode(QChartView::NoDrag);
            break;
        case VerticalZoom:
            setRubberBand(QChartView::VerticalRubberBand);
            setDragMode(QChartView::NoDrag);
            break;
        }
    }
}

void ZoomableChartView::zoomX(qreal factor, qreal xcenter)
{
    QRectF rect = chart()->plotArea();
    qreal widthOriginal = rect.width();
    rect.setWidth(widthOriginal / factor);
    qreal centerScale = (xcenter / widthOriginal);

    qreal leftOffset = (xcenter - (rect.width() * centerScale) );

    rect.moveLeft(rect.x() + leftOffset);
    chart()->zoomIn(rect);
}

void ZoomableChartView::zoomX(qreal factor)
{
    QRectF rect = chart()->plotArea();
    qreal widthOriginal = rect.width();
    QPointF center_orig = rect.center();

    rect.setWidth(widthOriginal / factor);

    rect.moveCenter(center_orig);

    chart()->zoomIn(rect);
}


void ZoomableChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isTouching)
        m_isTouching = false;

    QChartView::mouseReleaseEvent(event);
}

//![1]
void ZoomableChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chart()->zoomOut();
        break;
        //![1]
    case Qt::Key_Left:
        chart()->scroll(-10, 0);
        break;
    case Qt::Key_Right:
        chart()->scroll(10, 0);
        break;
    case Qt::Key_Up:
        chart()->scroll(0, 10);
        break;
    case Qt::Key_Down:
        chart()->scroll(0, -10);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}
