#include "analogclock.h"
#include "./ui_analogclock.h"
#include <QPainter>
#include <QTime>
#include <QTimer>

AnalogClock::AnalogClock(QWidget *parent)
    : QWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&AnalogClock::update));
    timer->start(1000);
    setWindowTitle(tr("Стрілковий годинник"));
    resize(200, 200);
}
AnalogClock::~AnalogClock()
{
    delete ui;
}
void AnalogClock::paintEvent(QPaintEvent *)
{
    static const QPoint hourHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -40)
    };
    static const QPoint minuteHand[3] = {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -70)
    };
    QColor hourColor(127, 0, 127);
    QColor minuteColor(0, 127, 127, 191);
    int side = qMin(width(), height());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);
    painter.setPen(Qt::NoPen);
    painter.setBrush(hourColor);
    QTime time = QTime::currentTime();
    painter.save();

    int hourAngle = (360 / 24) * time.hour();
    painter.rotate(hourAngle);
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore();
    painter.setPen(hourColor);
    for (int i = 0; i < 24; ++i) {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(15.0);
    }
    painter.setPen(Qt::NoPen);
    painter.setBrush(minuteColor);
    painter.save();

    int minuteAngle = (360 / 60) * ((time.minute() + time.second() / 60.0));
    painter.rotate(minuteAngle);
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();
    painter.setPen(minuteColor);
    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }

    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.rotate(-90.0); // Зсув для початку малювання цифр
    for (int k = 0; k < 24; ++k) {
        int hour = (k + 18) % 24 + 1;
        painter.drawText(-6, -85, QString::number(hour));
        painter.rotate(15.0);
    }
}
