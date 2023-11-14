#include <QtConcurrent/QtConcurrent>
#include <QPainter>
#include <iterator>

#include "Maths.h"
#include "Triangle.h"

Triangle::Triangle(QList<QPoint> positions, QSize size) : positions(positions), m_color(0, 0, 0)
{
    const float width = size.width();
    const float height = size.height();
    coordinates = {
        QVector2D(positions.at(0).x() / width, positions.at(0).y() / height),
        QVector2D(positions.at(1).x() / width, positions.at(1).y() / height),
        QVector2D(positions.at(2).x() / width, positions.at(2).y() / height)
    };
    normalVersors = QList<QVector3D>(3);
}

void Triangle::calculateNormalVersors(const QList<ControlPoint>& controlPoints)
{
    QtConcurrent::map(coordinates, [this, &controlPoints](const QVector2D& cor) {
        std::ptrdiff_t k = std::distance(&coordinates.at(0), &cor);
        float du = 0.0f;
        float dv = 0.0f;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                du += controlPoints.at(ij2k(i, j)).coordinate.z() * dB(i, cor.x()) * B(j, cor.y());
                dv += controlPoints.at(ij2k(i, j)).coordinate.z() * B(i, cor.x()) * dB(j, cor.y());
            }
        }
        normalVersors[k] = QVector3D(-du, -dv, 1).normalized();
    });
}

void Triangle::paint(QSharedPointer<QImage> image) const
{
    QPainter painter(image.get());
    QPen pen(m_color);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawPolygon(positions);
}

void Triangle::fill(QSharedPointer<QImage> image, const Lambert& params) const
{
    // scan linia
}

QVector3D Triangle::approxN(const QVector2D& coordinate) const
{
    const QVector2D vec0 = QVector2D(coordinates.at(0)) - coordinate;
    const QVector2D vec1 = QVector2D(coordinates.at(1)) - coordinate;
    const QVector2D vec2 = QVector2D(coordinates.at(2)) - coordinate;
    const float P0 = triangleArea(vec1.x(), vec1.y(), vec2.x(), vec2.y());
    const float P1 = triangleArea(vec0.x(), vec0.y(), vec2.x(), vec2.y());
    const float P2 = triangleArea(vec0.x(), vec0.y(), vec1.x(), vec1.y());
    return QVector3D(P0 * normalVersors.at(0) + P1 * normalVersors.at(1) + P2 * normalVersors.at(2));
}
