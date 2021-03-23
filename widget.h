#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>
#include <QList>
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    struct figure;
    void clear_f();
    void paint_f();
    void clear_game();
    bool check_field();
    void set_score();

private:
    Ui::Widget *ui;
    int SZ;
    int blocks[20][10];
    bool start;
    int score, best, speed, prev_score, border;
    QTimer *timer;

public slots:
    void field_update();

signals:
    void get_score(QString);
    void get_best(QString);
};

#endif // WIDGET_H
