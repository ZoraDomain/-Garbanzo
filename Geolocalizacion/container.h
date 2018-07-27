#ifndef CONTAINER_H
#define CONTAINER_H

#include <QWidget>

class QVBoxLayout;
class Container : public QWidget {
    Q_OBJECT
public:
    explicit Container(QWidget *parent = nullptr);
    //Container(QWidget *_body = nullptr, QWidget *parent = nullptr);
    void setBody(QWidget *);
    //QWidget *body() {return body;}
    Container *instance();

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    QWidget *body;
    QVBoxLayout *vLayout;

signals:
    void resizeMap(int w, int h);
};


#endif // CONTAINER_H
