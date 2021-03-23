#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <iostream>
#include <QGlobal.h>
#include <QTime>

void swap(int &a, int &b) {
    int c = b;
    b = a;
    a = c;
}

bool check_down(int blocks[20][10], int i1, int j1, int i2, int j2, int i3, int j3) {
    if (j1 == j2) {
        return i3 < 19 && !blocks[i3 + 1][j3];
    }
    return i3 < 19 && !blocks[i3 + 1][j3] && !blocks[i2 + 1][j2] && !blocks[i1 + 1][j1];
}

bool check_right(int blocks[20][10], int i1, int j1, int i2, int j2, int i3, int j3) {
    if (j1 == j3) {
        return j1 < 9 && !blocks[i3][j3 + 1] && !blocks[i2][j2 + 1] && !blocks[i1][j1 + 1];
    }
    return j3 < 9 && !blocks[i3][j3 + 1];
}

bool check_left(int blocks[20][10], int i1, int j1, int i2, int j2, int i3, int j3) {
    if (j1 == j3) {
        return j1 > 0 && !blocks[i1][j1 - 1] && !blocks[i2][j2 - 1] && !blocks[i3][j3 - 1];
    }
    return j1 > 0 && !blocks[i1][j1 - 1];
}

struct Widget::figure {
    int i1, i2, i3, j1, j2, j3, c1, c2, c3;
    bool go;
    figure(int a1 = 0, int a2 = 5, int b1 = 1, int b2 = 5, int d1 = 2, int d2 = 5) {
        i1 = a1, j1 = a2, i2 = b1, j2 = b2, i3 = d1, j3 = d2;
        c1 = qrand() % 5 + 1;
        c2 = qrand() % 5 + 1;
        c3 = qrand() % 5 + 1;
        go = 1;
    }
    void down(int b[20][10]) {
        if (!go) {
            return;
        }
        if (check_down(b, i1, j1, i2, j2, i3, j3)) {
            i1++, i2++, i3++;
        }
        else {
            //std::cerr << "here\n";
            go = 0;
        }
    }
    void right(int b[20][10]) {
        if (go && check_right(b, i1, j1, i2, j2, i3, j3)) {
            j1++, j2++, j3++;
        }
    }
    void left(int b[20][10]) {
        if (go && check_left(b, i1, j1, i2, j2, i3, j3)) {
            j1--, j2--, j3--;
        }
    }
    void color_swap() {
        if (go) {
            swap(c1, c2); // c2 c1 c3
            swap(c2, c3); // c2 c3 c1
        }
    }
    void coor_swap() {
        if (!go) {
            return;
        }
        if (j1 == j2) {
            i1 = i3 = i2;
            if (j2 == 0) {
                j1 = 0, j2 = 1, j3 = 2;
            }
            else if (j2 == 9) {
                j3 = 9, j2 = 8, j3 = 7;
            }
            else {
                j1 = j2 - 1, j3 = j2 + 1;
            }
        }
        else {
            j1 = j3 = j2;
            if (i2 == 0) {
                i1 = 0, i2 = 1, i3 = 2;
            }
            else if (i2 == 19) {
                i3 = 19, i2 = 18, i1 = 17;
            }
            else {
                i1 = i2 - 1, i3 = i2 + 1;
            }
        }
    }
};

void Widget::clear_game() {
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 10; ++j) {
            blocks[i][j] = 0;
        }
    }
}

Widget::figure f;

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    SZ = 800;
    start = 0;
    score = 0;
    best = 0;
    speed = 500;
    prev_score = 0;
    border = 10;
    clear_game();
    timer = new QTimer;

    timer->start(speed);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(field_update()));
}


Widget::~Widget()
{
    delete ui;
}

QColor get_color(int a) {
    switch (a) {
    case 0:
        return QColor(255, 255, 205, 255);
    case 1:
        return QColor(136, 255, 77, 255);
    case 2:
        return QColor(77, 255, 255, 255);
    case 3:
        return QColor(204, 102, 255, 255);
    case 4:
        return QColor(255, 209, 26, 255);
    default:
        return QColor(255, 102, 102, 255);
    }
}

void Widget::paint_f() {
    blocks[f.i1][f.j1] = f.c1;
    blocks[f.i2][f.j2] = f.c2;
    blocks[f.i3][f.j3] = f.c3;
}

void Widget::clear_f() {
    blocks[f.i1][f.j1] = 0;
    blocks[f.i2][f.j2] = 0;
    blocks[f.i3][f.j3] = 0;
}

void clear_used(bool used[20][10]) {
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            used[i][j] = 0;
        }
    }
}

void clear_comp(int comp[20][10]) {
    int c = 201;
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            comp[i][j] = c;
            ++c;
        }
    }
}

bool check_block(int i, int j, int b[20][10]) {
    if (i >= 2 && b[i - 2][j] == b[i][j] && b[i - 1][j] == b[i][j]) {
        return 1;
    }
    if (i <= 17 && b[i + 2][j] == b[i][j] && b[i + 1][j] == b[i][j]) {
        return 1;
    }
    if (j >= 2 && b[i][j - 2] == b[i][j] && b[i][j - 1] == b[i][j]) {
        return 1;
    }
    if (j <= 7 && b[i][j + 2] == b[i][j] && b[i][j + 1] == b[i][j]) {
        return 1;
    }
    if (i >= 1 && i <= 18 && b[i - 1][j] == b[i][j] && b[i + 1][j] == b[i][j]) {
        return 1;
    }
    if (j >= 1 && j <= 8 && b[i][j - 1] == b[i][j] && b[i][j + 1] == b[i][j]) {
        return 1;
    }
    return 0;
}

void dfs(int cmp, int i, int j, bool used[20][10], int comp[20][10], int b[20][10]) {
    used[i][j] = 1;
    if (check_block(i, j, b)) {
        comp[i][j] = cmp;
    }
    if (i + 1 <= 19 && !used[i + 1][j] && b[i + 1][j] == b[i][j]) {
        dfs(cmp, i + 1, j, used, comp, b);
    }
    if (j + 1 <= 9 && !used[i][j + 1] && b[i][j + 1] == b[i][j]) {
        dfs(cmp, i, j + 1, used, comp, b);
    }
    if (i - 1 >= 0 && !used[i - 1][j] && b[i - 1][j] == b[i][j]) {
        dfs(cmp, i - 1, j, used, comp, b);
    }
    if (j - 1 >= 0 && !used[i][j - 1] && b[i][j - 1] == b[i][j]) {
        dfs(cmp, i, j - 1, used, comp, b);
    }
}

bool Widget::check_field() {
    bool used[20][10];
    int comp[20][10];
    clear_used(used);
    clear_comp(comp);
    int num = 0;
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (!used[i][j] && blocks[i][j]) {
                dfs(num, i, j, used, comp, blocks);
                ++num;
            }
        }
    }
    clear_used(used);
    bool ch = 0;
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (check_block(i, j, comp)) {
                used[i][j] = 1;
                ch = 1;
            }
        }
     }
    if (!ch) {
        return 0;
    }
    for (int j = 0; j < 10; ++j) {
        QVector <int> tmp(20, 0);
        int ind = 19;
        for (int i = 19; i >= 0; --i) {
            if (!used[i][j]) {
                tmp[ind] = blocks[i][j];
                --ind;
            }
            else {
                ++score;
            }
        }
        for (int i = 0; i < 20; ++i) {
            blocks[i][j] = tmp[i];
        }
    }
    return 1;
}

void Widget::set_score() {
    if (speed > 100 && score != prev_score && score >= border) {
        speed -= 80;
        border += 10;
        timer->start(speed);
    }
    prev_score = score;
    emit get_score(QString::number(score));
    if (score > best) {
        best = score;
    }
    emit get_best(QString::number(best));
}

void Widget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    if (!start) {
        clear_game();
        f = figure();
    }
    else if (!f.go && f.i1 == 0) {
        clear_game();
        start = 0;
        f = figure();
    }
    else {
        paint_f();
        if (!f.go) {
            bool ch = check_field();
            int y = 0;
            while (ch) {
                ch = check_field();
                ++y;
            }
            f = figure();
            paint_f();
        }
    }

    set_score();

    int h = SZ / 10, w = SZ / 10 * 3, sz = (SZ - 2 * h) / 20, i_cnt = -1, j_cnt = -1;
    for (int i = h; i < SZ - h; i += sz) {
        ++i_cnt;
        j_cnt = -1;
        for (int j = w; j < SZ - w; j += sz) {
            ++j_cnt;
            painter.setBrush(QBrush(get_color(blocks[i_cnt][j_cnt])));
            painter.drawRect(j, i, sz, sz);
        }
    }
}

void Widget::field_update() {
    clear_f();
    if (start) {
       f.down(blocks);
    }
    repaint();
}

int get_back(int i, int j, int blocks[20][10]) {
    while (i + 1 <= 19 && !blocks[i + 1][j]) {
        ++i;
    }
    return i;
}

void Widget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == 16777220 && !start) {
        start = 1;
        score = 0;
    }
    if (event->key() == Qt::Key_Escape) {
        start = 0;
        set_score();
        clear_game();
    }
    if (!start) {
        return;
    }
    if (event->key() == Qt::Key_Right) {
        clear_f();
        f.right(blocks);
        repaint();
    }
    if (event->key() == Qt::Key_Left) {
        clear_f();
        f.left(blocks);
        repaint();
    }
    if (event->key() == Qt::Key_Down) {
        f.color_swap();
        repaint();
    }
    if (event->key() == Qt::Key_Up) {
        clear_f();
        f.coor_swap();
        repaint();
    }
    if (event->key() == Qt::Key_Space) {
        clear_f();
        if (f.j1 == f.j2) {
            int tmp = get_back(f.i3, f.j3, blocks);
            if (tmp > 2) {
                f.i3 = tmp, f.i2 = tmp - 1, f.i1 = tmp - 2;
            }
            else {
                f.i1 = 0, f.i2 = 1, f.i3 = 2, f.go = 0;
            }
        }
        else {
            int tmp = qMin(qMin(get_back(f.i3, f.j3, blocks), get_back(f.i2, f.j2, blocks)), get_back(f.i1, f.j1, blocks));
            if (tmp > 0) {
                f.i3 = f.i2 = f.i1 = tmp;
            }
            else {
                f.i3 = f.i2 = f.i1 = 0;
                f.go = 0;
            }
        }
        repaint();
    }
}

