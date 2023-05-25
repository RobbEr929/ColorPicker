#pragma once
#include <QGraphicsDropShadowEffect>
#include <QString>

#include "ColorDetailDialog.h"

enum class Operation : qint8
{
    ENTER,
    LEAVE,
    QUIT,
    CHANGE_METHOD,
    COPY,
};

inline QString ModifiersToStr(Qt::KeyboardModifiers modifiers)
{
    QString str;
    if (modifiers & Qt::ControlModifier)
    {
        str.append("Ctrl + ");
    }
    if (modifiers & Qt::ShiftModifier)
    {
        str.append("Shift + ");
    }
    if (modifiers & Qt::AltModifier)
    {
        str.append("Alt + ");
    }
    return str;
}

inline void AddShadowBox(QWidget* widget)
{
    auto *effect = new QGraphicsDropShadowEffect(widget);
    effect->setOffset(0, 0);
    effect->setBlurRadius(6);
    effect->setColor(QColor(0x56, 0x56, 0x56));
    widget->setGraphicsEffect(effect);
}