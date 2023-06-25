#include "ColorDetailDialog.h"

#include <QClipboard>
#include <QKeyEvent>
#include <QScreen>
#include <QStringBuilder>
#include <QtSingleApplication>
#include <QDebug>

#include "AboutWidget.h"

ColorDetailDialog::ColorDetailDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    setWindowFlag(Qt::Tool);
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlag(Qt::WindowStaysOnTopHint);
    setFixedSize(154, 208);

    ui.ltLabel->SetPosition(RectangleBorderLabel::Position::LEFT_TOP);
    ui.rtLabel->SetPosition(RectangleBorderLabel::Position::RIGHT_TOP);
    ui.lbLabel->SetPosition(RectangleBorderLabel::Position::LEFT_BOTTOM);
    ui.rbLabel->SetPosition(RectangleBorderLabel::Position::RIGHT_BOTTOM);
    ui.lLabel->SetPosition(RectangleBorderLabel::Position::LEFT);
    ui.tLabel->SetPosition(RectangleBorderLabel::Position::TOP);
    ui.rLabel->SetPosition(RectangleBorderLabel::Position::RIGHT);
    ui.bLabel->SetPosition(RectangleBorderLabel::Position::BOTTOM);
    ui.cLabel->SetPosition(RectangleBorderLabel::Position::CENTER);
}

enum ColorDetailDialog::MeasureType ColorDetailDialog::MeasureType() const
{
    if (ui.contentWidget->currentWidget() == ui.colorPage)
    {
        return MeasureType::COLOR;
    }

    return MeasureType::SIZE;
}

void ColorDetailDialog::ChangeMeasureType()
{
    if (ui.contentWidget->currentWidget() == ui.colorPage)
    {
        ui.contentWidget->setCurrentWidget(ui.rulerPage);
        setFixedSize(165, 202);
    }
    else
    {
        ui.contentWidget->setCurrentWidget(ui.colorPage);
        setFixedSize(154, 208);
    }
}

void ColorDetailDialog::ChangeRepresentationMethod()
{
    if (ui.colorPage->isHidden())
    {
        return;
    }

    int index = ui.colorStackedWidget->currentIndex();
    if (++index >= ui.colorStackedWidget->count())
    {
        index = 0;
    }
    ui.colorStackedWidget->setCurrentIndex(index);
    SlotUpdateCursorPosition();
}

void ColorDetailDialog::SlotCopyValue()
{
    if(ui.contentWidget->currentWidget() != ui.colorPage)
    {
        return;
    }

    qApp->clipboard()->setText(ui.colorStackedWidget->currentWidget()->findChild<QLineEdit *>()->text());
}

void ColorDetailDialog::SlotFixedDialog()
{
    mDialogFixed = !mDialogFixed;
}

void ColorDetailDialog::SlotUpdateCursorPosition()
{
    if (mDialogFixed)
    {
        return;
    }

    move(PositionCanShownCompleted(QCursor::pos()));

    const int x = QCursor::pos().x();
    const int y = QCursor::pos().y();

    if (ui.contentWidget->currentWidget() == ui.colorPage)
    {
        ui.pointEdit->setText(QString("x:%1  y:%2").arg(x).arg(y));

        const QPixmap pixmap = qApp->primaryScreen()->grabWindow(0, qMax(x - 5, 0), qMax(y - 3, 0), 11, 7);

        ui.thumbnailLabel->setPixmap(pixmap.scaled(154, 98));

        if (pixmap.isNull())
        {
            return;
        }

        const QImage image = pixmap.toImage();

        ui.rgbEdit->clear();
        ui.hexEdit->clear();
        ui.hsvEdit->clear();
        ui.hslEdit->clear();
        ui.cmykEdit->clear();

        if (image.valid(5, 3))
        {
            const QColor color = image.pixel(5, 3);
            ui.rgbEdit->setText(QString("(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue()));
            ui.hexEdit->setText("#" % Rgb2Hex(color.red()) % Rgb2Hex(color.green()) % Rgb2Hex(color.blue()));
            ui.hsvEdit->setText(QString("(%1, %2%, %3%)").arg(qMin(360, qMax(0, color.hsvHue()))).arg(qRound(color.hsvSaturationF() * 100)).arg(qRound(color.valueF() * 100)));
            ui.hslEdit->setText(QString("(%1, %2%, %3%)").arg(qMin(360, qMax(0, color.hslHue()))).arg(qRound(color.hslSaturationF() * 100)).arg(qRound(color.lightnessF() * 100)));
            ui.cmykEdit->setText(QString("(%1%, %2%, %3%, %4%)").arg(qRound(color.cyanF() * 100)).arg(qRound(color.magentaF() * 100)).arg(qRound(color.yellowF() * 100)).arg(qRound(color.blackF() * 100)));
        }
    }
    else
    {
    }
}

void ColorDetailDialog::SlotPixmapChanged(const QPixmap &pixmap)
{
    mPixmap = pixmap;
}

void ColorDetailDialog::SlotMeasureRectChanged(QRect rect)
{
    qDebug() << rect;
    ui.distanceEdit->setText(QString("%1 x %2").arg(rect.width()).arg(rect.height()));
    ui.ltLabel->setPixmap(mPixmap.copy(qMax(rect.left() - 5, 0), qMax(rect.top() - 3, 0), 11, 7).scaled(55, 35));
    ui.rtLabel->setPixmap(mPixmap.copy(qMax(rect.right() - 5, 0), qMax(rect.top() - 3, 0), 11, 7).scaled(55, 35));
    ui.lbLabel->setPixmap(mPixmap.copy(qMax(rect.left() - 5, 0), qMax(rect.bottom() - 3, 0), 11, 7).scaled(55, 35));
    ui.rbLabel->setPixmap(mPixmap.copy(qMax(rect.right() - 5, 0), qMax(rect.bottom() - 3, 0), 11, 7).scaled(55, 35));
    ui.lLabel->setPixmap(mPixmap.copy(qMax(rect.left() - 5, 0), qMax(QCursor::pos().y() - 3, 0), 11, 7).scaled(55, 35));
    ui.tLabel->setPixmap(mPixmap.copy(qMax(QCursor::pos().x() - 5, 0), qMax(rect.top() - 3, 0), 11, 7).scaled(55, 35));
    ui.rLabel->setPixmap(mPixmap.copy(qMax(rect.right() - 5, 0), qMax(QCursor::pos().y() - 3, 0), 11, 7).scaled(55, 35));
    ui.bLabel->setPixmap(mPixmap.copy(qMax(QCursor::pos().x() - 5, 0), qMax(rect.bottom() - 3, 0), 11, 7).scaled(55, 35));
    ui.cLabel->setPixmap(mPixmap.copy(qMax(QCursor::pos().x() - 5, 0), qMax(QCursor::pos().y() - 3, 0), 11, 7).scaled(55, 35));
}

void ColorDetailDialog::showEvent(QShowEvent *event)
{
    mDialogFixed = false;
}

QPoint ColorDetailDialog::PositionCanShownCompleted(const QPoint &mousePoint)
{
    const QRect screenRect = qApp->primaryScreen()->availableGeometry();

    bool canShownRight = false;
    bool canShownBottom = false;
    if (screenRect.right() - mousePoint.x() >= width() + 10)
    {
        canShownRight = true;
    }
    if (screenRect.bottom() - mousePoint.y() >= height() + 10)
    {
        canShownBottom = true;
    }
    return { mousePoint + QPoint(canShownRight ? 10 : -(width() + 10), canShownBottom ? 10 : -(height() + 10))};
}
