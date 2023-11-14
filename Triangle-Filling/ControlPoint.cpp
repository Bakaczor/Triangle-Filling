#include <QPainter>

#include "ControlPoint.h"

ControlPoint::ControlPoint(QPoint position, QVector3D coordinate, bool modifiable) : position(position), coordinate(coordinate), modifiable(modifiable), m_color(0, 0, 0) {}

void ControlPoint::paint(QSharedPointer<QImage> image) const
{
    QPainter painter(image.get());
    QPen pen(m_color);
    pen.setWidth(m_radius);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    painter.drawPoint(position);
}

bool ControlPoint::equals(const int &x, const int &y) const
{
    return qFabs(position.x() - x) < m_radius && qFabs(position.y() - y) < m_radius;
}

void ControlPoint::select()
{
    m_color = QColor(255, 0, 0);
}

void ControlPoint::unselect()
{
    m_color = QColor(0, 0, 0);
}


