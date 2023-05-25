#include "ColorWidget.h"

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
    : QMainWindow(parent)
    , mIsPicking(false)
    , mIsFixed(false)
    , mTimer(new QTimer(this))
    , mTrayMenu(new QMenu(this))
    , mTrayIcon(new QSystemTrayIcon(QIcon(":/ColorWidget/icon.png"), this))
    , mEnterKey(new QHotkey(Qt::Key_C, Qt::AltModifier, false, this))
    , mLeaveKey(new QHotkey(Qt::Key_Escape, Qt::NoModifier, false, this))
    , mQuitKey(new QHotkey(Qt::Key_Q, Qt::ControlModifier | Qt::AltModifier, false, this))
    , mMethodKey(new QHotkey(Qt::Key_T, Qt::NoModifier, false, this))
    , mCopyKey(new QHotkey(Qt::Key_C, Qt::NoModifier, false, this))
    , mFixedKey(new QHotkey(Qt::Key_F, Qt::NoModifier, false, this))
    , mAboutWidget(AboutWidget())
    , mOptionWidget(OptionWidget())
    , mColorDetailDialog(ColorDetailDialog())
{
    ui.setupUi(this);

    setWindowFlag(Qt::Tool);
    setWindowFlag(Qt::FramelessWindowHint);
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
   
    connect(mTimer, &QTimer::timeout, this, &ColorWidget::SlotShowColorValue);
    connect(mTrayIcon, &QSystemTrayIcon::activated, this, &ColorWidget::SlotTrayIconClicked);
    connect(mEnterKey, &QHotkey::activated, this, &ColorWidget::SlotEnterPick);
    connect(mLeaveKey, &QHotkey::activated, this, &ColorWidget::SlotLeavePick);
    connect(mQuitKey, &QHotkey::activated, qSingleApp, &QtSingleApplication::quit);
    connect(mMethodKey, &QHotkey::activated, &mColorDetailDialog, &ColorDetailDialog::SlotChangeMethod);
    connect(mCopyKey, &QHotkey::activated, &mColorDetailDialog, &ColorDetailDialog::SlotCopyValue);
    connect(mFixedKey, &QHotkey::activated, this, &ColorWidget::SlotFixedDetail);
    connect(this, &ColorWidget::SignalCursorPositionChanged, &mColorDetailDialog, &ColorDetailDialog::SlotCursorPositionChanged);

    if (!mEnterKey->setRegistered(true))
    {
        mTrayIcon->showMessage(QStringLiteral("热键注册失败!"), QStringLiteral("热键[启动拾色(Alt + C)]注册失败!功能可能受限!"));
        mEnterKey->blockSignals(true);
    }
    if (!mLeaveKey->setRegistered(true))
    {
        mTrayIcon->showMessage(QStringLiteral("热键注册失败!"), QStringLiteral("热键[取消拾色(Esc)]注册失败!功能可能受限!"));
        mLeaveKey->blockSignals(true);
    }
    if (!mQuitKey->setRegistered(true))
    {
        mTrayIcon->showMessage(QStringLiteral("热键注册失败!"), QStringLiteral("热键[开始拾色(Ctrl + Alt + Q)]注册失败!功能可能受限!"));
        mQuitKey->blockSignals(true);
    }

    hide();
}

void ColorWidget::SlotReceivedAppMessage(const QString &msg)
{
    const QStringList msgList = msg.split("$$");

    mTrayIcon->showMessage(msgList.at(0), msgList.at(1));
}

void ColorWidget::SlotShowColorValue()
{
    if (!mIsPicking) 
    {
        return;
    }

    emit SignalCursorPositionChanged();
}

void ColorWidget::SlotEnterPick()
{
    if(mIsPicking)
    {
        return;
    }
    mIsPicking = true;

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
    mColorDetailDialog.exec();
}

void ColorWidget::SlotLeavePick()
{
    if(!mIsPicking)
    {
        return;
    }

    mMethodKey->setRegistered(false);
    mCopyKey->setRegistered(false);
    mFixedKey->setRegistered(false);
    mIsPicking = false;
    mIsFixed = false;
    hide();
    mColorDetailDialog.done(0);
}

void ColorWidget::SlotFixedDetail()
{
    mIsFixed = !mIsFixed;
    blockSignals(mIsFixed);
}

void ColorWidget::SlotTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Context:
        mTrayMenu->exec(QCursor::pos());
        return;
    case QSystemTrayIcon::Trigger:
        SlotEnterPick();
        return;
    default: 
        return;
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
