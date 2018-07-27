#include "address.h"

Address::Address(QObject *parent) : QObject(parent) {
}

Address::Address(QString _street, QString _district, QString _city, QString _country, QString _state,
                 QString _countryCode, QString _postalCode, QObject *parent) :
    street(_street), district(_district), city(_city), country(_country), state(_state),
    countryCode(_countryCode), postalCode(_postalCode), QObject(parent) { }

void Address::setStreet(QString newStreet) {
    if (street != newStreet)
        street = newStreet;
    else return;
}

void Address::setDistrict(QString newDistrict) {
    if (district != newDistrict)
        district = newDistrict;
    else return;
}

void Address::setCity(QString newCity) {
    if (city != newCity)
        city = newCity;
    else return;
}

void Address::setCountry(QString newCountry) {
    if (country != newCountry)
        country = newCountry;
    else return;
}

void Address::setState(QString newState) {
    if (state != newState)
        state = newState;
    else return;
}

void Address::setCountryCode(QString newCountryCode) {
    if (countryCode != newCountryCode)
        countryCode = newCountryCode;
    else return;
}

void Address::setPostalCode(QString newPostalCode) {
    if (postalCode != newPostalCode)
        postalCode = newPostalCode;
    else return;
}

void Address::setAddress(const Address *newAddress) {
            this->street = newAddress->getStreet();
            this->district = newAddress->getDistrict();
            this->city = newAddress->getCity();
            this->country = newAddress->getCountry();
            this->state = newAddress->getState();
            this->countryCode = newAddress->getCountryCode();
            this->postalCode = newAddress->getPostalCode();

}

Address &Address::operator =(const Address &newAddress) {
    if (&newAddress != this) {
        setAddress(&newAddress);
    } else {
        return *this;
    }
}
