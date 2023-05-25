#pragma once

#include <QDialog>
#include "ui_AboutWidget.h"

class AboutWidget final
    : public QDialog
{
    Q_OBJECT

public:
   explicit AboutWidget(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::AboutWidget ui;
};
