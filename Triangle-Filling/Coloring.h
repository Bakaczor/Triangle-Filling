#include <QImage>
#include <QSharedPointer>
#include <QVector3D>

struct Lambert
{
    uint ks;
    uint kd;
    uint m;
    QVector3D lightCoordiante;
    QVector3D lightColor;
    QVector3D backColor;
};

void colorPixel(QSharedPointer<QImage> image, const QPoint& position, const QVector2D& coordinate, const QVector3D& N, const Lambert& params);
