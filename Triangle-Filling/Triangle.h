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
        QList<QVector3D> coordinates;
        QList<QVector3D> normalVersors;

        Triangle(QList<QPoint> positions, QSize size);
        void calculateNormalVersorsAndZ(const QList<ControlPoint>& controlPoints);
        void paint(QSharedPointer<QImage> image) const;
        void fill(QSharedPointer<QImage> image, const Lambert& params) const;

    private:
        QColor m_color;
        float m_width;
        float m_height;

        QPair<QVector3D, QVector3D> approxN_Z(const QVector2D& coordinate) const;
};
