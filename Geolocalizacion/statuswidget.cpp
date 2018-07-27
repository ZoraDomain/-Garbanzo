#include "statuswidget.h"
#include "coordinate.h"
#include "address.h"
#include "marker.h"
#include "bus.h"

#include <QtWidgets>

StatusWidget::StatusWidget(QWidget *parent) : QWidget(parent) {
    setMinimumWidth(360);
    setupStatusName();
    setupStatusCoordinateBox();
    setupStatusAddressBox();
    setThereAreSomeone(false);

    QObject::connect(this, SIGNAL(thereAreSomeoneChanged(bool)), this, SLOT(onAndOff(bool)));
    QObject::connect(idLineEdit, SIGNAL(editingFinished()), this, SLOT(setName()));
    QObject::connect(setCheckBox, SIGNAL(clicked(bool)), this, SLOT(setSet(bool)));
    //QObject::connect(setCheckBox, SIGNAL(clicked(bool)), this, SIGNAL(setStilMarker(index, bool)));

    QVBoxLayout *box = new QVBoxLayout(this);
    box->addLayout(idLayout);
    box->addWidget(coordinateBox);
    box->addWidget(addressBox);
}

void StatusWidget::setupStatusName() {
    idLabel = new QLabel(tr("Id:"));
    idLabel->setFont(QFont("Courier New", 12, 1));
    idLineEdit = new QLineEdit();
    idLineEdit->setFont(QFont("Courier New", 10, 1));
    idLineEdit->setDisabled(true);
    setCheckBox = new QCheckBox(tr("Set"));

    idLayout = new QHBoxLayout;
    idLayout->addWidget(idLabel);
    idLayout->addWidget(idLineEdit);
    idLayout->addWidget(setCheckBox);
}

void StatusWidget::setupStatusCoordinateBox() {
    coordinate = new Coordinate(this);

    coordinateBox = new QGroupBox(tr("Coordinate"), this);
    latitudeTitle = new QLabel(tr("Latitude:"));
    latitudeTitle->setFont(QFont("Courier New", 10, 1));
    longitudeTitle = new QLabel(tr("Longitude:"));
    longitudeTitle->setFont(QFont("Courier New", 10, 1));
    latitude = new QLabel(QString::number(0.00));
    latitude->setFont(QFont("Courier New", 10, 1));
    longitude = new QLabel(QString::number(0.00));
    longitude->setFont(QFont("Courier New", 10, 1));

    QHBoxLayout *latitudeLayout = new QHBoxLayout;
    latitudeLayout->addWidget(latitudeTitle);
    latitudeLayout->addWidget(latitude);

    QHBoxLayout *longitudeLayout = new QHBoxLayout;
    longitudeLayout->addWidget(longitudeTitle);
    longitudeLayout->addWidget(longitude);

    QVBoxLayout *coordinateLayout = new QVBoxLayout;
    coordinateLayout->addLayout(latitudeLayout);
    coordinateLayout->addLayout(longitudeLayout);

    coordinateBox->setLayout(coordinateLayout);
    coordinateBox->setFixedSize(QSize(200, 90));
    coordinateBox->setFont(QFont("Courier New", 12, 1));

}

void StatusWidget::setupStatusAddressBox() {
    address = new Address(this);

    addressBox = new QGroupBox(tr("Address"), this);
    streetTitle = new QLabel(tr("Street:"));
    streetTitle->setFont(QFont("Courier New", 10, 1));
    districtTitle = new QLabel(tr("District:"));
    districtTitle->setFont(QFont("Courier New", 10, 1));
    cityTitle = new QLabel(tr("City:"));
    cityTitle->setFont(QFont("Courier New", 10, 1));
    countryTitle = new QLabel(tr("Country:"));
    countryTitle->setFont(QFont("Courier New", 10, 1));
    stateTitle = new QLabel(tr("State:"));
    stateTitle->setFont(QFont("Courier New", 10, 1));
    countryCodeTitle = new QLabel(tr("Country Code:"));
    countryCodeTitle->setFont(QFont("Courier New", 10, 1));
    postalCodeTitle = new QLabel(tr("Postal Code:"));
    postalCodeTitle->setFont(QFont("Courier New", 10, 1));
    street = new QLabel("-");
    street->setFont(QFont("Courier New", 10, 1));
    district = new QLabel("-");
    district->setFont(QFont("Courier New", 10, 1));
    city = new QLabel("-");
    city->setFont(QFont("Courier New", 10, 1));
    country = new QLabel("-");
    country->setFont(QFont("Courier New", 10, 1));
    state = new QLabel("-");
    state->setFont(QFont("Courier New", 10, 1));
    countryCode = new QLabel("-");
    countryCode->setFont(QFont("Courier New", 10, 1));
    postalCode = new QLabel("-");
    postalCode->setFont(QFont("Courier New", 10, 1));

    QHBoxLayout *streetLayout = new QHBoxLayout;
    streetLayout->addWidget(streetTitle);
    streetLayout->addWidget(street);

    QHBoxLayout *districtLayout = new QHBoxLayout;
    districtLayout->addWidget(districtTitle);
    districtLayout->addWidget(district);

    QHBoxLayout *cityLayout = new QHBoxLayout;
    cityLayout->addWidget(cityTitle);
    cityLayout->addWidget(city);

    QHBoxLayout *countryLayout = new QHBoxLayout;
    countryLayout->addWidget(countryTitle);
    countryLayout->addWidget(country);

    QHBoxLayout *stateLayout = new QHBoxLayout;
    stateLayout->addWidget(stateTitle);
    stateLayout->addWidget(state);

    QHBoxLayout *countryCodeLayout = new QHBoxLayout;
    countryCodeLayout->addWidget(countryCodeTitle);
    countryCodeLayout->addWidget(countryCode);

    QHBoxLayout *postalCodeLayout = new QHBoxLayout;
    postalCodeLayout->addWidget(postalCodeTitle);
    postalCodeLayout->addWidget(postalCode);

    QVBoxLayout *addressLayout = new QVBoxLayout;
    addressLayout->addLayout(streetLayout);
    addressLayout->addLayout(districtLayout);
    addressLayout->addLayout(cityLayout);
    addressLayout->addLayout(countryLayout);
    addressLayout->addLayout(stateLayout);
    addressLayout->addLayout(countryCodeLayout);
    addressLayout->addLayout(postalCodeLayout);

    addressBox->setLayout(addressLayout);
    addressBox->setFixedSize(QSize(200, 200));
    addressBox->setFont(QFont("Courier New", 12, 1));
}

void StatusWidget::setCoordinate(Coordinate *newCoordinate) {
    coordinate->setLatitude(newCoordinate->getLatitude());
    coordinate->setLongitude(newCoordinate->getLongitude());
    coordinate->setAltitude(newCoordinate->getAltitude());
    update();
}

void StatusWidget::setCoordinate(double newAltitude, double newLatitude, double newLongitude) {
    coordinate->setLatitude(newLatitude);
    coordinate->setLongitude(newLongitude);
    coordinate->setAltitude(newAltitude);
    update();
}

void StatusWidget::setAddress(Address *newAddress) {
    address->setAddress(newAddress);
    update();
}

void StatusWidget::setIndex(int newIndex) {
    if (index != newIndex)
        index = newIndex;
    else
        return;
}
void StatusWidget::update() {
    updateLabel();
}

void StatusWidget::thankForMarker(Marker *marker) {
    setCoordinate(marker->getCoordinate());
    setAddress(marker->getAddress());
    setIndex(marker->getIndex());
    setThereAreSomeone(true);
    setName(marker->text());
    setCheckBox->setChecked(marker->getSet());
}

void StatusWidget::thankForBus(Bus *bus) {
    setCoordinate(bus->getCoordinate());
    setAddress(bus->getAddress());
    setIndex(bus->getIndex());
    setThereAreSomeone(true);
    setName(bus->text());
    setCheckBox->setChecked(bus->getSet());
}

void StatusWidget::checkRemoved(int index) {
    if (index == getIndexOfCurrentObject())
        setThereAreSomeone(false);
    else
        return;
}

void StatusWidget::lookForMarker(int index) {
    emit requestMarker(index);
}

void StatusWidget::lookForBus(int index) {
    emit requestBus(index);
}

void StatusWidget::updateInfo(int markerIndex, Marker *marker) {
    if (markerIndex == this->index) {
        setCoordinate(marker->getCoordinate());
        setAddress(marker->getAddress());
        setIndex(marker->getIndex());
        setCheckBox->setChecked(marker->getSet());
        update();
    } else {
        return;
    }
}

void StatusWidget::updateInfo(int busIndex, Bus *bus) {
    if (busIndex == this->index) {
        setCoordinate(bus->getCoordinate());
        setAddress(bus->getAddress());
        setIndex(bus->getIndex());
        setCheckBox->setChecked(bus->getSet());
        update();
    } else {
        return;
    }
}

StatusWidget *StatusWidget::instance() {
    return this;
}

void StatusWidget::setThereAreSomeone(const bool newThereAreSomeone) {
    if (newThereAreSomeone == true && thereAreSomeone == true)
        return;
    if (newThereAreSomeone == true && thereAreSomeone == false)
        thereAreSomeone = newThereAreSomeone; emit thereAreSomeoneChanged(thereAreSomeone);
    if (newThereAreSomeone == false && thereAreSomeone == true)
        thereAreSomeone = newThereAreSomeone; emit thereAreSomeoneChanged(thereAreSomeone);
    if (newThereAreSomeone == false && thereAreSomeone == false)
        return;
}

void StatusWidget::onAndOff(bool) {
    if (!isThereSomeone()) {
        idLineEdit->setDisabled(true);
        idLineEdit->setText(" ");
        latitude->setText(QString::number(0.00));
        longitude->setText(QString::number(0.00));
        street->setText("-");
        district->setText("-");
        city->setText("-");
        country->setText("-");
        state->setText("-");
        countryCode->setText("-");
        postalCode->setText("-");
    } else {
        idLineEdit->setDisabled(false);
    }
}

void StatusWidget::updateLabel() {
    /////cordinale label
    latitude->setText(QString::number(coordinate->getLatitude()));
    longitude->setText(QString::number(coordinate->getLongitude()));

    ////address label
    street->setText(address->getStreet());
    district->setText(address->getDistrict());
    city->setText(address->getCity());
    country->setText(address->getCountry());
    state->setText(address->getState());
    countryCode->setText(address->getCountryCode());
    postalCode->setText(address->getPostalCode());
}

void StatusWidget::setSet(bool newSet) {
    emit setFollowMe(getIndexOfCurrentObject(), newSet);
}

void StatusWidget::setName() {
    QString newName = idLineEdit->text();
    if (name == newName)
        return;
    else
        name = newName; emit statusNameChanged(getIndexOfCurrentObject(), getNameOfCurrentObject());
}

void StatusWidget::setName(const QString newName) {
    if (name == newName)
        return;
    else {
        name = newName;
        idLineEdit->setText(name);
    }
}
