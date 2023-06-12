#include "ThumbnailLabel.h"

#include <QPainter>
#include <QResizeEvent>

ThumbnailLabel::ThumbnailLabel(QWidget *parent)
    : QLabel(parent)
{
    ui.setupUi(this);
}

void ThumbnailLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(QColor(0, 255, 255, 128), 14));

    painter.drawLine(QPointF(width() / 2.0 + 1, pos().y()), QPointF( width() / 2.0 + 1,  height()));
    painter.drawLine(QPointF(pos().x(), height() / 2.0), QPointF(width(),  height() / 2.0));
}