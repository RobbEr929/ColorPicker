#pragma once

#include "define.h"
#include "qhotkey.h"
#include "ui_OptionWidget.h"

class OptionWidget final
    : public QDialog 
{
    Q_OBJECT

public:
    explicit OptionWidget(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:

    Ui::OptionWidget ui;
};
