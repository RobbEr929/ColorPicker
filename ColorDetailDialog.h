#pragma once

#include "ui_ColorDetailDialog.h"

class ColorDetailDialog final
    : public QDialog
{
    Q_OBJECT

public:
    enum class MeasureType : qint8
    {
        COLOR,          // 测量颜色
        SIZE,           // 测量大小, 自动测量
    };
    explicit ColorDetailDialog(QWidget *parent = nullptr);

    MeasureType MeasureType() const;

    void ChangeMeasureType();

    void ChangeRepresentationMethod();

public slots:
    
    void SlotCopyValue();

    void SlotFixedDialog();

    void SlotUpdateCursorPosition();

    void SlotPixmapChanged(const QPixmap &pixmap);

    void SlotMeasureRectChanged(QRect rect);

protected:
    void showEvent(QShowEvent *event) override;

private:
    QPoint PositionCanShownCompleted(const QPoint &mousePoint);

    QString Rgb2Hex(const int rgb) const
    {
        return QString("%1").arg(rgb, 2, 16, QLatin1Char('0')).toUpper();
    }

    bool mDialogFixed;
    QPixmap mPixmap;

    Ui::ColorDetailDialog ui;
};
