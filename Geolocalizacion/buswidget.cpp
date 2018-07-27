#include "buswidget.h"
#include "bus.h"
#include "coordinate.h"
#include "address.h"
#include "signalmapper.h"

#include <QtWidgets>

BusWidget::BusWidget (QWidget *parent) : QWidget(parent) {
    busArea = new QScrollArea(this);
    busArea->setWidgetResizable(true);
    scrollAreaContens = new QWidget();
    this->setMinimumHeight(100);

    listLayout = new QGridLayout(scrollAreaContens);
    listLayout->setMargin(1); listLayout->setSpacing(1);
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    listLayout->addItem(verticalSpacer, 1, 0);

    busArea->setWidget(scrollAreaContens);
    busArea->setStyleSheet("background-color:transparent;");
    busArea->setFrameShape(QFrame::NoFrame);

    signalMapper = new SignalMapper(this);
    QObject::connect(signalMapper, SIGNAL(mapped(int)), this, SIGNAL(clicked(int)));

}

void BusWidget::addBus(Bus *newBus) {
    listLayout->addWidget(newBus, BusWidget::row, 0);
    listLayout->removeItem(verticalSpacer);
    listLayout->addItem(verticalSpacer, BusWidget::row + 1, 0);
    BusWidget::row++;

    QObject::connect(newBus, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(newBus, newBus->getIndex());

    emit clicked(newBus->getIndex());
}

BusWidget *BusWidget::instance() {
    return this;
}

void BusWidget::resizeEvent(QResizeEvent *event) {
    busArea->setFixedSize(event->size());
}

int BusWidget::row = 0;
