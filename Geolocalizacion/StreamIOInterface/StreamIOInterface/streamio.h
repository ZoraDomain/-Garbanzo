#ifndef STREAMIO_H
#define STREAMIO_H

#include <QQuickItem>

class StreamIO : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(StreamIO)

    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
    StreamIO(QObject *parent = 0);
    ~StreamIO();

    Q_INVOKABLE void read();
    Q_INVOKABLE void write();

    void setSource(QUrl &newSource);
    void setText(QString &newText);

private:
    QUrl source;
    QString text;

signals:
    void sourceChanged();
    void textChanged();

};

#endif // STREAMIO_H
