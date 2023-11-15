#include <QtMath>

#include "Coloring.h"
#include "Maths.h"

void colorPixel(QSharedPointer<QImage> image, const QPoint& position, const QVector3D& coordinate, const QVector3D& N, const Lambert& params)
{
    const QVector3D L = (params.lightCoordinate - coordinate).normalized();
    float cosNL = dotProduct(N, L);

    const QVector3D V = { 0, 0, 1 };
    const QVector3D R = 2 * cosNL * N - L;
    float cosVR = dotProduct(V, R);

    if (cosNL < 0) cosNL = 0;
    if (cosVR < 0) cosVR = 0;

    const float RILIO = params.lightColor->x() * params.backColor->x();
    const float GILIO = params.lightColor->y() * params.backColor->y();
    const float BILIO = params.lightColor->z() * params.backColor->z();

    const float kd_cosNL_ks_cosmVR = *params.kd * cosNL + *params.ks * qPow(cosVR, *params.m);

    int pR = 255 * RILIO * (kd_cosNL_ks_cosmVR);
    int pG = 255 * GILIO * (kd_cosNL_ks_cosmVR);
    int pB = 255 * BILIO * (kd_cosNL_ks_cosmVR);

    if (pR > 255) pR = 255;
    if (pG > 255) pG = 255;
    if (pB > 255) pB = 255;

    image->setPixelColor(position, QColor(pR, pG, pB));
}
