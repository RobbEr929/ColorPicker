#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QSystemTrayIcon>

#include "AboutWidget.h"
#include "ColorDetailDialog.h"
#include "OptionWidget.h"
#include "qhotkey.h"
#include "ui_ColorPicker.h"

class ColorWidget final
    : public QWidget
{
    Q_OBJECT
public:
    explicit ColorWidget(QWidget *parent = nullptr);

Q_SIGNALS:
    void SignalFixedDialog();

    void SignalPixmapChanged(const QPixmap &pixmap);

    void SignalMeasureRectChanged(const QRect &rect);

public Q_SLOTS:
    void SlotReceivedAppMessage(const QString &msg);

protected:
    void paintEvent(QPaintEvent *event) override;

    void showEvent(QShowEvent *event) override;

private:
    void CheckMeasureType();

    void FindCursorBorder();

    bool mIsPicking;
    QTimer *mTimer;
    QMenu *mTrayMenu;
    QSystemTrayIcon *mTrayIcon;
    QHotkey *mEnterKey;
    QHotkey *mLeaveKey;
    QHotkey *mQuitKey;
    QHotkey *mMethodKey;
    QHotkey *mCopyKey;
    QHotkey *mFixedKey;
    QHotkey *mMeasureKey;
    QRect mRect;
    QPixmap mPixmap;

    AboutWidget mAboutWidget;
    OptionWidget mOptionWidget;
    ColorDetailDialog mColorDetailDialog;

    Ui::ColorPicker ui;

Q_SIGNALS:
    void SignalCursorPositionChanged();

private Q_SLOTS:
    void SlotShowDetailDialog();

    void SlotEnterPick();

    void SlotLeavePick();

    void SlotMeasureTypeChanged();

    void SlotRepresentationMethodChanged();

    void SlotTrayIconClicked(QSystemTrayIcon::ActivationReason reason);

    void SlotOpenOption();

    void SlotOpenAbout();
};

#endif // COLORWIDGET_H
