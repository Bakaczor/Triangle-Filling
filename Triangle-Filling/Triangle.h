#include <QPoint>
#include <QList>
#include <QVector3D>
#include <QImage>
#include <QSharedPointer>

#include "ControlPoint.h"
#include "Coloring.h"

class Triangle
{
    public:
        QList<QPoint> positions;
        QList<QVector2D> coordinates;
        QList<QVector3D> normalVersors;

        Triangle(QList<QPoint> positions, QSize size);
        void calculateNormalVersors(const QList<ControlPoint>& controlPoints);
        void paint(QSharedPointer<QImage> image) const;
        void fill(QSharedPointer<QImage> image, const Lambert& params) const;

    private:
        QColor m_color;

        QVector3D approxN(const QVector2D& coordinate) const;
};
