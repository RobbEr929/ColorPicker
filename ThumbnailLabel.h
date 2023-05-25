#pragma once

#include <QLabel>
#include "ui_ThumbnailLabel.h"

class ThumbnailLabel : public QLabel
{
    Q_OBJECT

public:
    ThumbnailLabel(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::ThumbnailLabelClass ui;
};
