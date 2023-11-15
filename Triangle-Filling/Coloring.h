#include <QImage>
#include <QSharedPointer>
#include <QVector3D>

#include <Circle.h>

struct Lambert
{
    const float* ks;
    const float* kd;
    const uint* m;
    const QVector3D* lightColor;
    const QVector3D* backColor;
    QVector3D lightCoordinate;
};

void colorPixel(QSharedPointer<QImage> image, const QPoint& position, const QVector3D& coordinate, const QVector3D& N, const Lambert& params);
