#pragma once
#include <QGraphicsDropShadowEffect>
#include <QWidget>

inline void AddShadowBox(QWidget* widget)
{
    auto *effect = new QGraphicsDropShadowEffect(widget);
    effect->setOffset(0, 0);
    effect->setBlurRadius(6);
    effect->setColor(QColor(0x56, 0x56, 0x56));
    widget->setGraphicsEffect(effect);
}