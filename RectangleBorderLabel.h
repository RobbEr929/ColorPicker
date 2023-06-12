#ifndef RECTANGLEBORDERLABEL_H
#define RECTANGLEBORDERLABEL_H

#include <QLabel>
class RectangleBorderLabel final
    : public QLabel
{
    Q_OBJECT

public:
    enum class Position
    {
        NONE,
        LEFT_TOP,
        RIGHT_TOP,
        LEFT_BOTTOM,
        RIGHT_BOTTOM,
        LEFT,
        TOP,
        RIGHT,
        BOTTOM,
        CENTER,
    };
    explicit RectangleBorderLabel(QWidget *parent = nullptr, Position position = Position::NONE);

    void SetPosition(Position position);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Position mPosition;
};



#endif // RECTANGLEBORDERLABEL_H


