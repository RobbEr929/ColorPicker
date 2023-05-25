#include <QDateTime>
#include <QMessageBox>
#include <QtSingleApplication>

#include "AboutWidget.h"
#include "ColorWidget.h"

int main(int argc, char **argv)
{
    QtSingleApplication a("ColorWidget", argc, argv);

    if(a.isRunning())
    {
        QMessageBox::warning(nullptr, QStringLiteral("程序已经在运行中!"), QStringLiteral("程序已经在运行中!请检查您的任务栏图标!"));
        a.sendMessage(QStringLiteral("程序运行中!$$当前时间为%1").arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")));
        return EXIT_SUCCESS;
    }

    const ColorWidget colorWidget;
    QObject::connect(&a, &QtSingleApplication::messageReceived, &colorWidget, &ColorWidget::SlotReceivedAppMessage);

    return QtSingleApplication::exec();
}
