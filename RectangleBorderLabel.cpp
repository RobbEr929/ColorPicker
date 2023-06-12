#include "RectangleBorderLabel.h"

#include <QPainter>
#include <QResizeEvent>

RectangleBorderLabel::RectangleBorderLabel(QWidget *parent, Position position)
    : QLabel(parent)
    , mPosition(position)
{
}

void RectangleBorderLabel::SetPosition(Position position)
{
    mPosition = position;
}

void RectangleBorderLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (pixmap())
    {
        painter.drawPixmap(rect(), *pixmap());
    }

    painter.setPen(QPen(QBrush(QColor(0x56, 0x56, 0x56)), 1));
    painter.drawRect(rect().adjusted(0, 0, -1, -1));

    painter.setPen(QPen(QBrush(QColor(0, 255, 255)), 5));

    switch (mPosition)
    {
    case Position::LEFT_TOP:
    {
        painter.drawLine(rect().center(), QPoint(width() * 0.5, height() * 0.75));
        painter.drawLine(rect().center(), QPoint(width() * 0.75, height() * 0.5));
        break;
    }
    case Position::RIGHT_TOP:
    {
        painter.drawLine(rect().center(), QPoint(width() * 0.5, height() * 0.75));
        painter.drawLine(rect().center(), QPoint(width() * 0.25, height() * 0.5));
        break;
    }
    case Position::LEFT_BOTTOM:
    {
        painter.drawLine(rect().center(), QPoint(width() * 0.5, height() * 0.25));
        painter.drawLine(rect().center(), QPoint(width() * 0.75, height() * 0.5));
        break;
    }
    case Position::RIGHT_BOTTOM:
    {
        painter.drawLine(rect().center(), QPoint(width() * 0.5, height() * 0.25));
        painter.drawLine(rect().center(), QPoint(width() * 0.25, height() * 0.5));
        break;
    }
    case Position::LEFT:
    case Position::RIGHT:
    {
        painter.drawLine(QPoint(width() * 0.5, height() * 0.25), QPoint(width() * 0.5, height() * 0.75));
        break;
    }
    case Position::TOP:
    case Position::BOTTOM:
    {
        painter.drawLine(QPoint(width() * 0.25, height() * 0.5), QPoint(width() * 0.75, height() * 0.5));
        break;
    }
    case Position::CENTER:
    {
        painter.drawPoint(rect().center());
    }
    }
}
