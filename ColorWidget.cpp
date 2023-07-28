#include "ColorWidget.h"

#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QMutex>
#include <QPainter>
#include <QScreen>
#include <QStyleOption>
#include <QTimer>
#include <QtSingleApplication>
#include <Windows.h>

#include "AboutWidget.h"

ColorWidget::ColorWidget(QWidget *parent)
    : QWidget(parent)
    , mIsPicking(false)
    , mTimer(new QTimer(this))
    , mTrayMenu(new QMenu(this))
    , mTrayIcon(new QSystemTrayIcon(QIcon(":/ColorWidget/icon.png"), this))
    , mEnterKey(new QHotkey(Qt::Key_C, Qt::AltModifier, false, this))
    , mLeaveKey(new QHotkey(Qt::Key_Escape, Qt::NoModifier, false, this))
    , mQuitKey(new QHotkey(Qt::Key_Q, Qt::ControlModifier | Qt::AltModifier, false, this))
    , mMethodKey(new QHotkey(Qt::Key_T, Qt::NoModifier, false, this))
    , mCopyKey(new QHotkey(Qt::Key_C, Qt::NoModifier, false, this))
    , mFixedKey(new QHotkey(Qt::Key_F, Qt::NoModifier, false, this))
    , mMeasureKey(new QHotkey(Qt::Key_S, Qt::NoModifier, false, this))
    , mAboutWidget(AboutWidget())
    , mOptionWidget(OptionWidget())
    , mColorDetailDialog(ColorDetailDialog())
{
    ui.setupUi(this);

    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    mTrayMenu->addAction(QStringLiteral("启动拾色"), this, &ColorWidget::SlotEnterPick);
    mTrayMenu->addAction(QStringLiteral("取消拾色"), this, &ColorWidget::SlotLeavePick);
    mTrayMenu->addSeparator();
    mTrayMenu->addAction(QStringLiteral("选项"), this, &ColorWidget::SlotOpenOption);
    mTrayMenu->addAction(QStringLiteral("关于"), this, &ColorWidget::SlotOpenAbout);
    mTrayMenu->addSeparator();
    mTrayMenu->addAction(QStringLiteral("退出"), qSingleApp, &QtSingleApplication::quit);
    mTrayIcon->setContextMenu(mTrayMenu);
    mTrayIcon->show();

    mTimer->start(10);
    mTrayIcon->setToolTip(QStringLiteral("拾色助手(ColorPicker)"));
    mTrayIcon->showMessage(QStringLiteral("欢迎使用拾色助手(ColorPicker)!"), QStringLiteral("单击托盘图标可开始拾色, 右键查看菜单"));
   
    connect(mTimer, &QTimer::timeout, this, &ColorWidget::SlotShowDetailDialog);
    connect(mTrayIcon, &QSystemTrayIcon::activated, this, &ColorWidget::SlotTrayIconClicked);
    connect(mEnterKey, &QHotkey::activated, this, &ColorWidget::SlotEnterPick);
    connect(mLeaveKey, &QHotkey::activated, this, &ColorWidget::SlotLeavePick);
    connect(mQuitKey, &QHotkey::activated, qSingleApp, &QtSingleApplication::quit);
    connect(mMethodKey, &QHotkey::activated, this, &ColorWidget::SlotRepresentationMethodChanged);
    connect(mCopyKey, &QHotkey::activated, &mColorDetailDialog, &ColorDetailDialog::SlotCopyValue);
    connect(mFixedKey, &QHotkey::activated, &mColorDetailDialog, &ColorDetailDialog::SlotFixedDialog);
    connect(mMeasureKey, &QHotkey::activated, this, &ColorWidget::SlotMeasureTypeChanged);
    connect(this, &ColorWidget::SignalCursorPositionChanged, &mColorDetailDialog, &ColorDetailDialog::SlotUpdateCursorPosition);
    connect(this, &ColorWidget::SignalPixmapChanged, &mColorDetailDialog, &ColorDetailDialog::SlotPixmapChanged);
    connect(this, &ColorWidget::SignalMeasureRectChanged, &mColorDetailDialog, &ColorDetailDialog::SlotMeasureRectChanged);

    if (!mEnterKey->setRegistered(true))
    {
        mTrayIcon->showMessage(QStringLiteral("热键注册失败!"), QStringLiteral("热键[启动拾色(Alt + C)]注册失败!功能可能受限!"));
    }
    if (!mQuitKey->setRegistered(true))
    {
        mTrayIcon->showMessage(QStringLiteral("热键注册失败!"), QStringLiteral("热键[开始拾色(Ctrl + Alt + Q)]注册失败!功能可能受限!"));
    }

    hide();
}

void ColorWidget::SlotReceivedAppMessage(const QString &msg)
{
    const QStringList msgList = msg.split("$$");

    mTrayIcon->showMessage(msgList.at(0), msgList.at(1));
}

void ColorWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(mColorDetailDialog.MeasureType() == ColorDetailDialog::MeasureType::COLOR)
    {
        painter.setPen(QColor(0, 0, 0, 0));
        painter.drawRect(rect());
        return;
    }
    painter.drawPixmap(rect(), mPixmap);

    painter.setPen(QPen(Qt::red, 1));
    painter.drawLine(QCursor::pos().x(), mRect.top(), QCursor::pos().x(), mRect.bottom());
    painter.drawLine(mRect.left(), QCursor::pos().y(), mRect.right(), QCursor::pos().y());

}

void ColorWidget::showEvent(QShowEvent *event)
{
    CheckMeasureType();
}

void ColorWidget::CheckMeasureType()
{
    if(mColorDetailDialog.MeasureType() == ColorDetailDialog::MeasureType::COLOR)
    {
        return;
    }

    mColorDetailDialog.hide();
    mPixmap = qSingleApp->primaryScreen()->grabWindow(0, 0, 0);
    emit SignalPixmapChanged(mPixmap);
    mColorDetailDialog.show();
}

void ColorWidget::FindCursorBorder()
{
    const int x = QCursor::pos().x();
    const int y = QCursor::pos().y();

    if (mPixmap.isNull())
    {
        return;
    }
    const QImage image = mPixmap.toImage();

    if (image.valid(x, y))
    {
        for (int curX = x - 1;; --curX)
        {
            if (!image.valid(curX, y) || image.pixel(x, y) != image.pixel(curX, y))
            {
                mRect.setLeft(curX + 1);
                break;
            }
        }
        for (int curX = x + 1;; ++curX)
        {
            if (!image.valid(curX, y) || image.pixel(x, y) != image.pixel(curX, y))
            {
                mRect.setRight(curX - 1);
                break;
            }
        }
        for (int curY = y - 1;; --curY)
        {
            if (!image.valid(x, curY) || image.pixel(x, y) != image.pixel(x, curY))
            {
                mRect.setTop(curY + 1);
                break;
            }
        }
        for (int curY = y + 1;; ++curY)
        {
            if (!image.valid(x, curY) || image.pixel(x, y) != image.pixel(x, curY))
            {
                mRect.setBottom(curY - 1);
                break;
            }
        }
    }
}


void ColorWidget::SlotShowDetailDialog()
{
    if (!mIsPicking) 
    {
        return;
    }

    if(mColorDetailDialog.MeasureType() == ColorDetailDialog::MeasureType::SIZE)
    {
        FindCursorBorder();
        emit SignalMeasureRectChanged(mRect);
    }

    update();
    mColorDetailDialog.SlotUpdateCursorPosition();
}

void ColorWidget::SlotEnterPick()
{
    if(mIsPicking)
    {
        return;
    }
    mIsPicking = true;
    setGeometry(qSingleApp->primaryScreen()->geometry());
    show();

    if (!mLeaveKey->setRegistered(true))
    {
        mTrayIcon->showMessage(QStringLiteral("热键注册失败!"), QStringLiteral("热键[取消拾色(Esc)]注册失败!功能可能受限!"));
    }
    if(!mMethodKey->setRegistered(true))
    {
        mTrayIcon->showMessage(QStringLiteral("热键注册失败!"), QStringLiteral("热键[切换颜色类型(T)]注册失败!功能可能受限!"), QSystemTrayIcon::Warning);
    }
    if(!mCopyKey->setRegistered(true))
    {
        mTrayIcon->showMessage(QStringLiteral("热键注册失败!"), QStringLiteral("热键[复制颜色(C)]注册失败!功能可能受限!"), QSystemTrayIcon::Warning);
    }
    if(!mFixedKey->setRegistered(true))
    {
        mTrayIcon->showMessage(QStringLiteral("热键注册失败!"), QStringLiteral("热键[固定(C)]注册失败!功能可能受限!"), QSystemTrayIcon::Warning);
    }
    if(!mMeasureKey->setRegistered(true))
    {
        mTrayIcon->showMessage(QStringLiteral("热键注册失败!"), QStringLiteral("热键[固定(S)]注册失败!功能可能受限!"), QSystemTrayIcon::Warning);
    }

    mColorDetailDialog.exec();
}

void ColorWidget::SlotLeavePick()
{
    if(!mIsPicking)
    {
        return;
    }

    mIsPicking = false;

    mLeaveKey->setRegistered(false);
    mMethodKey->setRegistered(false);
    mCopyKey->setRegistered(false);
    mFixedKey->setRegistered(false);
    mMeasureKey->setRegistered(false);

    hide();
    mColorDetailDialog.done(0);
}


void ColorWidget::SlotMeasureTypeChanged()
{
    mColorDetailDialog.ChangeMeasureType();

    CheckMeasureType();
}

void ColorWidget::SlotRepresentationMethodChanged()
{
    mColorDetailDialog.ChangeRepresentationMethod();
}

void ColorWidget::SlotTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Context:
    {
        mTrayMenu->exec(QCursor::pos());
        return;
    }
    case QSystemTrayIcon::Trigger:
    {
        SlotEnterPick();
        return;
    }
    default:
    {
        return;
    }
    }
}

void ColorWidget::SlotOpenOption()
{
    if(mOptionWidget.isVisible())
    {
        mOptionWidget.raise();
        return;
    }
    mOptionWidget.exec();
}

void ColorWidget::SlotOpenAbout()
{
    if (mAboutWidget.isVisible())
    {
        mAboutWidget.raise();
        return;
    }
    mAboutWidget.exec();
}
