#ifndef SIGNALMAPPER_H
#define SIGNALMAPPER_H

#include <QObject>
#include <QHash>

class Coordinate;
class SignalMapper : public QObject {
    Q_OBJECT
public:
    explicit SignalMapper(QObject *parent = nullptr);
    ~SignalMapper();

    QObject *mapping(int) const;
    void setMapping(QObject *sender, int index);
    void removeMapping(QObject *sender);

signals:
    void mapped(int);

public slots:
    void map();
    void map(QObject *sender);
    
private:
    QHash<QObject *, int> indexHash;
};

#endif // SIGNALMAPPER_H
