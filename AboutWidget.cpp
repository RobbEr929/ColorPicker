#include "AboutWidget.h"

#include <QMouseEvent>

#include "define.h"

AboutWidget::AboutWidget(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    ui.titleLabel->installEventFilter(this);

    setWindowFlag(Qt::Tool);
    setWindowFlag(Qt::FramelessWindowHint);

    setAttribute(Qt::WA_TranslucentBackground);

    AddShadowBox(ui.centreWidget);

    connect(ui.closeButton, &QPushButton::clicked, this, &QDialog::close);
}

bool AboutWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui.titleLabel)
    {
        static QPoint mousePoint;
        static bool mousePress = false;
        switch (event->type())
        {
        case QEvent::MouseButtonPress:
        {
            const QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                mousePress = true;
                mousePoint = mouseEvent->globalPos() - this->pos();
            }
            return true;
        }
        case QEvent::MouseButtonRelease:
        {
            const QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                mousePress = false;
            }
            return true;
        }
        case QEvent::MouseMove:
        {
            const QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mousePress)
            {
                this->move(mouseEvent->globalPos() - mousePoint);
            }
            return true;
        }
        default:
        {
            break;
        }
        }

    }
    return QDialog::eventFilter(watched, event);
}
