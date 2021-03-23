#include "widget.h"
#include <QApplication>
#include <QTimer>
#include <QGlobal.h>
#include <QTime>
#include <QLayout>
#include <QPushButton>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget *w = new Widget;
    w->setFixedSize(800, 800);

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QFont f = w->font();
    w->setStyleSheet("Widget {background-color: #ffffe6}");
    f.setFamily("Courier");
    QFont f1 = f, f2 = f;
    f1.setPixelSize(15);
    f2.setPixelSize(20);

    QGridLayout *l = new QGridLayout;

    QGridLayout *lay1 = new QGridLayout;
    lay1->setGeometry(QRect(60, 0, 480, 180));

    lay1->setVerticalSpacing(50);
    lay1->setHorizontalSpacing(8);

    QPushButton *b1 = new QPushButton("^");
    QPushButton *b2 = new QPushButton("v");
    QPushButton *b3 = new QPushButton(">");
    QPushButton *b4 = new QPushButton("<");
    QPushButton *b5 = new QPushButton("space");

    QLabel *l1 = new QLabel("turn");
    QLabel *l2 = new QLabel("swap color");
    QLabel *l3 = new QLabel("right");
    QLabel *l4 = new QLabel("left");
    QLabel *l5 = new QLabel("down");

    b1->setFixedSize(50, 50);
    b2->setFixedSize(50, 50);
    b3->setFixedSize(50, 50);
    b4->setFixedSize(50, 50);
    b5->setFixedSize(70, 50);

    b1->setFont(f1);
    b2->setFont(f1);
    b3->setFont(f1);
    b4->setFont(f1);
    b5->setFont(f1);

    b1->setDisabled(1);
    b2->setDisabled(1);
    b3->setDisabled(1);
    b4->setDisabled(1);
    b5->setDisabled(1);

    l1->setFont(f);
    l2->setFont(f);
    l3->setFont(f);
    l4->setFont(f);
    l5->setFont(f);

    lay1->addWidget(b1, 0, 0);
    lay1->addWidget(b2, 1, 0);
    lay1->addWidget(b3, 2, 0);
    lay1->addWidget(b4, 3, 0);
    lay1->addWidget(b5, 4, 0);

    lay1->addWidget(l1, 0, 1);
    lay1->addWidget(l2, 1, 1);
    lay1->addWidget(l3, 2, 1);
    lay1->addWidget(l4, 3, 1);
    lay1->addWidget(l5, 4, 1);

    QGridLayout *lay2 = new QGridLayout;
    lay2->setGeometry(QRect(60, 420, 480, 180));
    lay2->setHorizontalSpacing(10);
    lay2->setVerticalSpacing(70);

    QLabel *start = new QLabel("start");
    QLabel *stop = new QLabel("stop");
    QLabel *score1 = new QLabel("score:");
    QLabel *best1 = new QLabel("best:");
    QLabel *score = new QLabel("0");
    QLabel *best = new QLabel("0");

    QPushButton *s = new QPushButton("Enter");
    QPushButton *fn = new QPushButton("Esc");

    start->setFixedSize(70, 50);
    stop->setFixedSize(50, 50);
    score1->setFixedSize(70, 50);
    best1->setFixedSize(70, 50);
    score->setFixedSize(70, 50);
    best->setFixedSize(70, 50);

    s->setFixedSize(70, 50);
    fn->setFixedSize(50, 50);

    start->setFont(f1);
    stop->setFont(f1);
    s->setFont(f1);
    fn->setFont(f1);
    score1->setFont(f1);
    best1->setFont(f1);
    score->setFont(f1);
    best->setFont(f1);

    s->setDisabled(1);
    fn->setDisabled(1);

    lay2->addWidget(score1, 0, 0);
    lay2->addWidget(best1, 1, 0);
    lay2->addWidget(score, 0, 1);
    lay2->addWidget(best, 1, 1);
    lay2->addWidget(s, 2, 0);
    lay2->addWidget(fn, 3, 0);
    lay2->addWidget(start, 2, 1);
    lay2->addWidget(stop, 3, 1);

    /*QTimer *timer = new QTimer;
    timer->start(500);

    QObject::connect(timer, SIGNAL(timeout()), w, SLOT(field_update()));*/
    QObject::connect(w, SIGNAL(get_score(QString)), score, SLOT(setText(QString)));
    QObject::connect(w, SIGNAL(get_best(QString)), best, SLOT(setText(QString)));

    l->setHorizontalSpacing(250);
    l->addLayout(lay1, 0, 0);
    l->addLayout(lay2, 0, 1);
    w->setLayout(l);
    w->show();

    return a.exec();
}
