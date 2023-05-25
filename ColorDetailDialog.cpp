#include "ColorDetailDialog.h"

#include <QClipboard>
#include <QKeyEvent>
#include <QScreen>
#include <QStringBuilder>
#include <QtMath>

#include "AboutWidget.h"

ColorDetailDialog::ColorDetailDialog(QWidget *parent)
    : QDialog(parent)
    , mRepresentationMethod(RepresentationMethod::RGB)
{
    ui.setupUi(this);

    setWindowFlag(Qt::Tool);
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlag(Qt::WindowStaysOnTopHint);
}

void ColorDetailDialog::SlotCursorPositionChanged()
{
    setGeometry(PositionCanShownCompleted(QCursor::pos()));

    const int x = QCursor::pos().x();
    const int y = QCursor::pos().y();

    QScreen *screen = qApp->primaryScreen();
    const QPixmap pixmap = screen->grabWindow(0, qMax(x - 5, 0), qMax(y - 3, 0), 11, 7);

    ui.thumbnailLabel->setPixmap(pixmap.scaled(154, 98));
    
    if (pixmap.isNull())
    {
        return;
    }

    const QImage image = pixmap.toImage();

    if (image.valid(5, 3)) 
    {
        const QColor color = image.pixel(5, 3);
        switch (mRepresentationMethod)
        {
        case RepresentationMethod::RGB:
        {
            ui.typeLabel->setText("RGB:");
            ui.colorEdit->setText(QString("(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue()));
            break;
        }
        case RepresentationMethod::HEX:
        {
            ui.typeLabel->setText("HEX:");
            ui.colorEdit->setText("#" % Rgb2Hex(color.red()) % Rgb2Hex(color.green()) % Rgb2Hex(color.blue()));
            break;
        }
        case RepresentationMethod::HSV:
        {
            ui.typeLabel->setText("HSV:");
            ui.colorEdit->setText(QString("(%1, %2%, %3%)").arg(color.hslHue()).arg(qCeil(color.hslSaturationF() * 100)).arg(qCeil(color.valueF() * 100)));
            break;
        }
        case RepresentationMethod::HSL:
        {
            ui.typeLabel->setText("HSL:");
            ui.colorEdit->setText(QString("(%1, %2%, %3%)").arg(color.hslHue()).arg(qCeil(color.hsvSaturationF() * 100)).arg(qCeil(color.lightnessF() * 100)));
            break;
        }
        case RepresentationMethod::CMYK:
        {
            ui.typeLabel->setText("CMYK:");
            ui.colorEdit->setText(QString("(%1%, %2%, %3%, %4%)").arg(qCeil(color.cyanF() * 100)).arg(qCeil(color.magentaF() * 100)).arg(qCeil(color.yellowF() * 100)).arg(qCeil(color.blackF() * 100)));
            break;
        }
        }
    }

    ui.pointEdit->setText(QString("x:%1  y:%2").arg(x).arg(y));
}

void ColorDetailDialog::SlotChangeMethod()
{
    switch (mRepresentationMethod)
    {
    case RepresentationMethod::RGB:
    {
        mRepresentationMethod = RepresentationMethod::HEX;
        break;
    }
    case RepresentationMethod::HEX:
    {
        mRepresentationMethod = RepresentationMethod::HSV;
        break;
    }
    case RepresentationMethod::HSV:
    {
        mRepresentationMethod = RepresentationMethod::HSL;
        break;
    }
    case RepresentationMethod::HSL:
    {
        mRepresentationMethod = RepresentationMethod::CMYK;
        break;
    }
    case RepresentationMethod::CMYK:
    {
        mRepresentationMethod = RepresentationMethod::RGB;
        break;
    }
    }
    SlotCursorPositionChanged();
}

void ColorDetailDialog::SlotCopyValue()
{
    qApp->clipboard()->setText(ui.colorEdit->text());
}

QRect ColorDetailDialog::PositionCanShownCompleted(const QPoint &mousePoint)
{
    const QRect screenRect = qApp->primaryScreen()->availableGeometry();

    bool canShownRight = false;
    bool canShownBottom = false;
    if (screenRect.right() - mousePoint.x() >= 154)
    {
        canShownRight = true;
    }
    if (screenRect.bottom() - mousePoint.y() >= 198)
    {
        canShownBottom = true;
    }
    return QRect(mousePoint + QPoint(canShownRight ? 10 : -164, canShownBottom ? 0 : -198), QSize(154, 198));
}
