#ifndef THUMBNAILLABEL_H
#define THUMBNAILLABEL_H

#include "ui_ThumbnailLabel.h"

class ThumbnailLabel final
    : public QLabel
{
    Q_OBJECT

public:
    explicit ThumbnailLabel(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::ThumbnailLabelClass ui;
};

#endif // THUMBNAILLABEL_H
