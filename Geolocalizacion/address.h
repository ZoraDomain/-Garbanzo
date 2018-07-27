#ifndef ADDRESS_H
#define ADDRESS_H

#include <QObject>

class Address : public QObject {
    Q_OBJECT
public:
    Address(QObject *parent = 0);
    Address(QString _street, QString _district, QString _city, QString _country, QString _state,
                     QString _countryCode, QString _postalCode, QObject *parent = 0);
    QString getStreet() const {return street;}
    QString getDistrict() const {return district;}
    QString getCity() const {return city;}
    QString getCountry() const {return country;}
    QString getState() const {return state;}
    QString getCountryCode() const {return countryCode;}
    QString getPostalCode() const {return postalCode;}

    void setStreet(QString newStreet);
    void setDistrict(QString newDistrict);
    void setCity(QString newCity);
    void setCountry(QString newCountry);
    void setState(QString newState);
    void setCountryCode(QString newCountryCode);
    void setPostalCode(QString newPostalCode);
    void setAddress(const Address *);

    Address &operator =(const Address &);
private:
    QString street;
    QString district;
    QString city;
    QString country;
    QString state;
    QString countryCode;
    QString postalCode;

public slots:
    /*void addAddress(QString street, QString district, QString city,
                    QString county, QString state, QString countryCode,
                    QString country,QString postalCode, QString latitude,
                    QString longitude);*/
};

#endif // ADDRESS_H
