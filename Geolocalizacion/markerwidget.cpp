#include "markerwidget.h"
#include "marker.h"
#include "coordinate.h"
#include "address.h"
#include "signalmapper.h"

#include <QtWidgets>

MarkerWidget::MarkerWidget (QWidget *parent) : QWidget(parent) {
    markerArea = new QScrollArea(this);
    markerArea->setWidgetResizable(true);
    scrollAreaContens = new QWidget();
    this->setMinimumHeight(100);

    listLayout = new QGridLayout(scrollAreaContens);
    listLayout->setMargin(1); listLayout->setSpacing(1);
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    listLayout->addItem(verticalSpacer, 1, 0);

    markerArea->setWidget(scrollAreaContens);
    markerArea->setStyleSheet("background-color:transparent;");
    markerArea->setFrameShape(QFrame::NoFrame);

    signalMapper = new SignalMapper(this);
    QObject::connect(signalMapper, SIGNAL(mapped(int)), this, SIGNAL(clicked(int)));

}

void MarkerWidget::addMarker(Marker *newMarker) {
    listLayout->addWidget(newMarker, MarkerWidget::row, 0);
    listLayout->removeItem(verticalSpacer);
    listLayout->addItem(verticalSpacer, MarkerWidget::row + 1, 0);
    MarkerWidget::row++;

    QObject::connect(newMarker, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(newMarker, newMarker->getIndex());

    emit clicked(newMarker->getIndex());
}
MarkerWidget *MarkerWidget::instance() {
    return this;
}

void MarkerWidget::resizeEvent(QResizeEvent *event) {
    markerArea->setFixedSize(event->size());
}

int MarkerWidget::row = 0;
