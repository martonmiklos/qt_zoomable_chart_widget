# Zoomable chart widget for Qt

Wrapper widget around QChartView which provides basic zoom/pan/legend handling stuff

## Why?

Because I got rid of configuring the QtCharts widget in each of my applications

### How to use it?

Add this project to your Qt application as a submodule with:

```
git submodule add https://github.com/martonmiklos/qt_zoomable_chart_widget.git
```

Include it into your project by adding the follwing to your .pro file:

```
include(qt_zoomable_chart_widget/zoomable_chart_widget.pri)
```

Create you UI with QtDesigner (or Designer plugin in the QtCreator).
Add your chart as a QWidget and then promote to the ZoomableChartWidget:

![Promoting widget](https://raw.githubusercontent.com/martonmiklos/qt_zoomable_chart_widget/master/screenshots/promote_to.png "Promoting widget")

You can access the underlying QtChart object via 

```
ui->whateverIsMyWidgetName->chart()
```

You can add/remove series,axes do whatever else you do with a QtChart.

### Features

#### Zoom mode
There is a zoom mode selector combobox above the chart. I know this is a space wasting thing, I am planning to move this feature to a hamburger style button to the graph area's corner.

![Zoom mode selector](https://raw.githubusercontent.com/martonmiklos/qt_zoomable_chart_widget/master/screenshots/zoom_selector.png "Zoom mode selector")

You can select 4 zoom modes with it:

- Pan mode: you can drag the graph horizontally
- Rectangular zoom mode
- Vertical zoom mode
- Horizontal zoom mode

#### Legend show/hide
You can show/hide series by clicking on the Legend markers. Inspired by the [Legend markers Qt example)](https://doc.qt.io/qt-5/qtcharts-legendmarkers-example.html)

#### Legend hover highlight
When hovering the mouse over the legend markers the respective series line width will be doubled:
![Legend hover](https://raw.githubusercontent.com/martonmiklos/qt_zoomable_chart_widget/master/screenshots/hover.png "Legend hover")

#### Limiting scrolling/zooming of QValueAxis
You can limit the scroll/zooming ranges of QValueAxes by using RangeLimitedValueAxis. 

If you find this feature useful to be included in the Qt itself [please comment/vote on this issue.](https://codereview.qt-project.org/c/qt-creator/qt-creator/+/256985)
