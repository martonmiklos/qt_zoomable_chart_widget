#include "zoomablechartwidget.h"
#include "ui_zoomablechartwidget.h"

#include <QtCharts/QLegendMarker>
#include <QDebug>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QT_CHARTS_USE_NAMESPACE
#endif



ZoomableChartWidget::ZoomableChartWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZoomableChartWidget)
{
    ui->setupUi(this);

    QIcon panIcon;
    panIcon.addFile(QStringLiteral(":/icons/pan.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->comboBoxZoomMode->addItem(panIcon, QString(), ZoomableChartView::Pan);

    QIcon rectZoomIcon;
    rectZoomIcon.addFile(QStringLiteral(":/icons/rectangle_zoom.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->comboBoxZoomMode->addItem(rectZoomIcon, QString(), ZoomableChartView::RectangleZoom);

    QIcon vZoomIcon;
    vZoomIcon.addFile(QStringLiteral(":/icons/vertical_zoom.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->comboBoxZoomMode->addItem(vZoomIcon, QString(), ZoomableChartView::VerticalZoom);

    QIcon hZoomIcon;
    hZoomIcon.addFile(QStringLiteral(":/icons/horizontal_zoom.png"), QSize(), QIcon::Normal, QIcon::Off);
    ui->comboBoxZoomMode->addItem(hZoomIcon, QString(), ZoomableChartView::HorizontalZoom);

    ui->chartView->setZoomMode(ZoomableChartView::Pan);

    m_chart = new ZoomableChart();
    ui->chartView->setChart(m_chart);
    connect(m_chart, &ZoomableChart::seriesAdded, this, &ZoomableChartWidget::seriesAdded);
    connect(m_chart, &ZoomableChart::seriesRemoved, this, &ZoomableChartWidget::seriesRemoved);
}

ZoomableChartWidget::~ZoomableChartWidget()
{
    delete m_chart;
    delete ui;
}

void ZoomableChartWidget::on_comboBoxZoomMode_activated(int index)
{
    ui->chartView->setZoomMode(static_cast<ZoomableChartView::ZoomMode>(ui->comboBoxZoomMode->itemData(index).toInt()));
}

void ZoomableChartWidget::legendMarkerClicked()
{
    static QLegendMarker *m_lastClickedMarker = nullptr;
    auto* marker = qobject_cast<QLegendMarker*>(sender());
    Q_ASSERT(marker);

    // Toggle visibility of series
    setSeriesVisible(marker->series(), !marker->series()->isVisible());
    if (m_lastClickedMarker
            && (m_lastClickedMarker->series()->isVisible() == marker->series()->isVisible())
            && (QApplication::keyboardModifiers() & Qt::ShiftModifier)) {
        auto markers = m_chart->legend()->markers();
        int startIndex = markers.indexOf(marker);
        int lastIndex = markers.indexOf(m_lastClickedMarker);
        if (startIndex != -1 && lastIndex != -1) {
            if (startIndex > lastIndex) {
                auto tmp = lastIndex;
                lastIndex = startIndex;
                startIndex = tmp;
            }

            for (int i = (startIndex + 1); i<lastIndex; i++) {
                setSeriesVisible(markers[i]->series(), !markers[i]->series()->isVisible());
            }
        }
    }
    m_lastClickedMarker = marker;
    connect(m_lastClickedMarker, &QLegendMarker::destroyed, this, [=]() {m_lastClickedMarker = nullptr;});
}

void ZoomableChartWidget::legendMarkerHovered(bool hover)
{
    auto* marker = qobject_cast<QLegendMarker*>(sender());
    Q_ASSERT(marker);

    QFont font = marker->font();
    font.setBold(hover);
    marker->setFont(font);

    if (marker->series()->type() == QAbstractSeries::SeriesTypeLine) {
        auto series = qobject_cast<QLineSeries*>(marker->series());
        auto pen = series->pen();
        pen.setWidth(hover ? (pen.width() * 2) : (pen.width() / 2));
        series->setPen(pen);
    }
}

ZoomableChart *ZoomableChartWidget::chart() const
{
    return m_chart;
}

ZoomableChartView *ZoomableChartWidget::chartView() const
{
    return ui->chartView;
}

void ZoomableChartWidget::addToolWidget(QWidget *widget)
{
    ui->horizontalLayoutToolbar->insertWidget(1, widget);
}

void ZoomableChartWidget::setSeriesVisible(QAbstractSeries *series, bool visible)
{
    series->setVisible(visible);
    const auto markers = m_chart->legend()->markers(series);
    for (auto *marker : markers) {
        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);

        // Dim the marker, if series is not visible
        qreal alpha = visible ? 1.0 : 0.5;
        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);
    }

    for (QAbstractAxis *axis : m_chart->axes(Qt::Vertical)) {
        bool hideAxis = true;
        for (auto *series : m_chart->series()) {
            for (const auto *attachedAxis : series->attachedAxes()) {
                if (series->isVisible() && attachedAxis == axis) {
                    hideAxis = false;
                    break;
                }
            }
            if (!hideAxis)
                break;
        }
        axis->setVisible(!hideAxis);
    }
}

void ZoomableChartWidget::seriesAdded(QAbstractSeries *series)
{
    // Connect all markers to handler
    const auto markers = m_chart->legend()->markers(series);
    for (auto marker : markers) {
        QObject::connect(marker, &QLegendMarker::clicked,
                         this, &ZoomableChartWidget::legendMarkerClicked);
        QObject::connect(marker, &QLegendMarker::hovered,
                         this, &ZoomableChartWidget::legendMarkerHovered);
    }
}

void ZoomableChartWidget::seriesRemoved(QAbstractSeries *series)
{
    // Connect all markers to handler
    const auto markers = m_chart->legend()->markers(series);
    for (auto marker : markers) {
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &ZoomableChartWidget::legendMarkerClicked);
        QObject::disconnect(marker, &QLegendMarker::hovered,
                            this, &ZoomableChartWidget::legendMarkerHovered);
    }
}


void ZoomableChartWidget::on_toolButtonFitInView_clicked()
{
    QHash<const QAbstractAxis*, qreal> xmin, xmax, ymin, ymax;
    // loop on all axes (both vetival and horizontal)
    for (const QAbstractAxis *axis : m_chart->axes()) {
        // only QValueAxis supported at the moment
        if (axis->type() != QAbstractAxis::AxisTypeValue)
            continue;

        // loop on all series attached to the axes and look for each min max point
        for (auto *series : m_chart->series()) {
            // at the moment only QLineSeries is supported for zoom to fit
            if (series->type() != QAbstractSeries::SeriesTypeLine)
                continue;

            if (!xmin.contains(axis))
                xmin[axis] = std::numeric_limits<qreal>::max();
            if (!xmax.contains(axis))
                xmax[axis] = -std::numeric_limits<qreal>::max();
            if (!ymin.contains(axis))
                ymin[axis] = std::numeric_limits<qreal>::max();
            if (!ymax.contains(axis))
                ymax[axis] = -std::numeric_limits<qreal>::max();

            for (const auto *attachedAxis : series->attachedAxes()) {
                if (series->isVisible() && attachedAxis == axis) {
                    auto lineSeries = static_cast<const QLineSeries*>(series);
                    for (const auto pt : lineSeries->points()) {
                        if (pt.x() < xmin[axis])
                            xmin[axis] = pt.x();
                        if (pt.x() > xmax[axis])
                            xmax[axis] = pt.x();
                        if (pt.y() < ymin[axis])
                            ymin[axis] = pt.y();
                        if (pt.y() > ymax[axis])
                            ymax[axis] = pt.y();
                    }
                    break;
                }
            }
        }
    }

    for (QAbstractAxis *axis : m_chart->axes()) {
        if (axis->type() != QAbstractAxis::AxisTypeValue)
            continue;
        if (axis->alignment() == Qt::Horizontal) {
            if (xmin.contains(axis))
                axis->setMin(xmin[axis]);

            if (xmax.contains(axis))
                axis->setMax(xmax[axis]);
        } else if (axis->alignment() == Qt::Vertical) {
            if (ymin.contains(axis))
                axis->setMin(ymin[axis]);

            if (ymax.contains(axis))
                axis->setMax(ymax[axis]);
        }
    }
}
