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

    painter.setPen(QPen(QColor(0, 255, 255, 127), 14));

    painter.drawLine(QPointF(pos().x() + width() / 2 + 1, pos().y()), QPointF(pos().x() + width() / 2 + 1, pos().y() + height()));
    painter.drawLine(QPointF(pos().x(), pos().y() + height() / 2), QPointF(pos().x() + width(), pos().y() + height() / 2));
}