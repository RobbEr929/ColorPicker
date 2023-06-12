#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

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

#endif // ABOUTWIDGET_H

