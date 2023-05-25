#pragma once

#include <QDialog>
#include "ui_ColorDetailDialog.h"

class ColorDetailDialog final
    : public QDialog
{
    Q_OBJECT

public:
    explicit ColorDetailDialog(QWidget *parent = nullptr);

public slots:
    void SlotCursorPositionChanged();

    void SlotChangeMethod();

    void SlotCopyValue();

private:
    enum class RepresentationMethod : qint8
    {
        RGB,
        HEX,
        HSV,
        HSL,
        CMYK,
    } mRepresentationMethod;

    QRect PositionCanShownCompleted(const QPoint &mousePoint);

    QString Rgb2Hex(const int rgb) const
    {
        return QString("%1").arg(rgb, 2, 16, QLatin1Char('0')).toUpper();
    }

    Ui::ColorDetailDialog ui;
};
