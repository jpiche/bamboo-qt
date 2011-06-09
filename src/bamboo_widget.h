#ifndef GOBOARDWIDGET_H
#define GOBOARDWIDGET_H

#include <vector>

#include <QWidget>
#include <QMouseEvent>

#include "gomove.h"

class BambooWidget : public QWidget
{
public:
    BambooWidget(QWidget *parent = 0);
    ~BambooWidget();
    void setMatrix(const MOVEMATRIX&);

protected:
    void paintEvent(QPaintEvent *event);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);

private:
    MOVEMATRIX matrix;
    bool pressing;
    bool hovering;
    QPointF currentHover;
    QPoint *pressingPoint;
};

#endif
