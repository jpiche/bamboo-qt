#include <iostream>
#include <math.h>

#include <QRectF>
#include <QColor>
#include <QPainter>
#include <QLineF>
#include <QPen>
#include <QPointF>
#include <QtGui>

#include "bamboo_widget.h"
#include "bamboo_window.h"

BambooWidget::BambooWidget(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    pressing = false;
    hovering = false;
}

BambooWidget::~BambooWidget()
{
}

QSize BambooWidget::minimumSizeHint() const
{
    return QSize(150, 150);
}

QSize BambooWidget::sizeHint() const
{
    return QSize(450, 450);
}

void BambooWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    const unsigned int size = matrix.size();
    const unsigned int sideLen = width() < height() ? width() : height();
    const qreal sqLen = sideLen / (size * 1.0);

    // AA needs to be off for pen width to be exact
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.translate( (width() - sideLen) / 2, (height() - sideLen) / 2 );

    painter.fillRect(QRectF(0.0, 0.0, sideLen, sideLen), QColor(230, 186, 96));

    QPen pen(QColor(0, 0, 0));
    pen.setWidthF(1.0);
    painter.setPen(pen);
    painter.setBrush(Qt::black);

    // draw lines of board
    for (unsigned short y = 0; y < size; y+=1) {
        painter.drawLine(QLineF(sqLen / 2.0,
                                (sqLen / 2.0) + (sqLen * y),
                                sideLen - (sqLen / 2.0),
                                (sqLen / 2.0) + (sqLen * y)));
        for (unsigned short x = 0; x < size; x+=1) {
            painter.drawLine(QLineF((sqLen / 2) + (sqLen * x),
                                    sqLen / 2,
                                    (sqLen / 2) + (sqLen * x),
                                    sideLen - (sqLen / 2)));
        }
    }

    // turn AA back on for drawing stones
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(Qt::NoPen);

    const qreal rad = (sqLen / 2) * 0.85;

    QGradientStops stops;
    QPointF center, focal;
    for (unsigned short y = 0; y < size; y+=1) {
        for (unsigned short x = 0; x < size; x+=1) {
            center = QPointF( (x * sqLen) + (sqLen / 2), (y * sqLen) + (sqLen / 2) );

            if (matrix.at(y).at(x) == EMPTY) {
                if (hovering
                    && floor((currentHover.x() - ((width() - sideLen) / 2)) / sqLen) == x
                    && floor((currentHover.y() - ((height() - sideLen) / 2)) / sqLen) == y
                ) {
                    BambooWindow *p = dynamic_cast<BambooWindow *>(parentWidget());
                    if (p->getNextMove() == BLACK) {
                        painter.setBrush(QColor::fromRgbF(0.0, 0.0, 0.0, 0.2));
                    } else if (p->getNextMove() == WHITE) {
                        painter.setBrush(QColor::fromRgbF(1.0, 1.0, 1.0, 0.2));
                    }
                    painter.drawEllipse(center, rad, rad);
                }
                continue;
            }

            if (matrix.at(y).at(x) == WHITE) {
                focal = QPointF( (x * sqLen) + (sqLen / 3), (y * sqLen) + (sqLen / 3) );
                stops << QGradientStop(0.00, QColor::fromRgbF(1.0, 1.0, 1.0, 1.0));
                stops << QGradientStop(1.00, QColor::fromRgbF(0.85, 0.85, 0.85, 1.0));
            } else if (matrix.at(y).at(x) == BLACK) {
                focal = QPointF( (x * sqLen) + ((sqLen * 3) / 8), (y * sqLen) + ((sqLen * 3) / 8) );
                stops << QGradientStop(0.00, QColor::fromRgbF(0.3, 0.3, 0.3, 1.0));
                stops << QGradientStop(1.00, QColor::fromRgbF(0, 0, 0, 1.0));
            }

            Q_ASSERT(stops.size() > 0);

            QRadialGradient g(center, rad, focal);
            for (int i=0; i < stops.size(); ++i) {
                g.setColorAt(stops.at(i).first, stops.at(i).second);
            }
            g.setSpread(QGradient::PadSpread);

            painter.setBrush(QColor::fromRgbF(0, 0, 0, 0.3));
            painter.drawEllipse(QPointF(center.x() + (rad / 9), center.y() + (rad / 9)), rad, rad);

            painter.setBrush(g);
            painter.drawEllipse(center, rad, rad);
        }
    }
}

void BambooWidget::setMatrix(const MOVEMATRIX& m)
{
    matrix = m;
}

void BambooWidget::mousePressEvent(QMouseEvent *event)
{
    const QPointF p = event->posF();
    if (p.x() >= 0
        && p.y() >= 0
        && p.x() < width()
        && p.y() < height()
    ) {
        const unsigned int sideLen = width() < height() ? width() : height();
        const qreal sqLen = sideLen / (matrix.size() * 1.0);
        pressingPoint = new QPoint(floor((p.x() - ((width() - sideLen) / 2)) / sqLen),
                               floor((p.y() - ((height() - sideLen) / 2)) / sqLen));
        pressing = true;
    }
}

void BambooWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (!pressing) {
        return;
    }

    if (0 != pressingPoint) {
        const unsigned int sideLen = width() < height() ? width() : height();
        const qreal sqLen = sideLen / (matrix.size() * 1.0);
        const QPointF p = event->posF();
        QPoint *tmp = new QPoint(floor((p.x() - ((width() - sideLen) / 2)) / sqLen),
                               floor((p.y() - ((height() - sideLen) / 2)) / sqLen));
        if (*tmp == *pressingPoint) {
            dynamic_cast<BambooWindow *>(parentWidget())->addMove(tmp->x(), tmp->y());
            update();
        }

        delete tmp;
        delete pressingPoint;
    }

    pressing = false;
    hovering = false;
}

void BambooWidget::mouseMoveEvent(QMouseEvent *event)
{
    const QPointF p = event->posF();
    const QPointF prevHoverPoint = currentHover;
    const bool prevHover = hovering;

    if (p.x() < 0
        || p.y() < 0
        || p.x() > width()
        || p.y() > height()
    ) {
        // out of bounds
        hovering = false;
        return;
    }

    hovering = true;
    currentHover = p;

    if (hovering != prevHover
        || currentHover != prevHoverPoint
    ) {
        update();
    }
}

void BambooWidget::leaveEvent(QEvent *event)
{
    if (event->type() == QEvent::Leave) {
        hovering = false;
        pressing = false;
        update();
    }
}
