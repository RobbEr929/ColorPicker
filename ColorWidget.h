#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QMainWindow>
#include <QSystemTrayIcon>

#include "AboutWidget.h"
#include "ColorDetailDialog.h"
#include "OptionWidget.h"
#include "qhotkey.h"
#include "ui_ColorWidget.h"

class ColorWidget final
    : public QMainWindow
{
    Q_OBJECT
public:
    explicit ColorWidget(QWidget *parent = nullptr);

public Q_SLOTS:
    void SlotReceivedAppMessage(const QString &msg);
    
private:
    bool mIsPicking;
    bool mIsFixed;
    QTimer *mTimer;
    QMenu *mTrayMenu;
    QSystemTrayIcon *mTrayIcon;
    QHotkey *mEnterKey;
    QHotkey *mLeaveKey;
    QHotkey *mQuitKey;
    QHotkey *mMethodKey;
    QHotkey *mCopyKey;
    QHotkey *mFixedKey;

    AboutWidget mAboutWidget;
    OptionWidget mOptionWidget;
    ColorDetailDialog mColorDetailDialog;

    Ui::ColorWidget ui;

Q_SIGNALS:
    void SignalCursorPositionChanged();

private Q_SLOTS:
    void SlotShowColorValue();

    void SlotEnterPick();

    void SlotLeavePick();

    void SlotFixedDetail();

    void SlotTrayIconClicked(QSystemTrayIcon::ActivationReason reason);

    void SlotOpenOption();

    void SlotOpenAbout();
};

#endif // COLORWIDGET_H
