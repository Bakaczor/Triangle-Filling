#include "Coloring.h"
#include "Maths.h"

void colorPixel(QSharedPointer<QImage> image, const QPoint& position, const QVector3D& coordinate, const QVector3D& Np, const Lambert& params)
{
    QVector3D N = Np;
    if (params.useTexture)
    {
        QVector3D B;
        QVector3D E(0, 0, 1);
        if (qFuzzyCompare(Np, E))
        {
            B = QVector3D(0, 1, 0);
        }
        else
        {
            B = QVector3D::crossProduct(Np, E);
        }
        QVector3D T = QVector3D::crossProduct(B, Np);
        QVector3D Nt = params.texture->at(ij2k(position.y(), position.x(), params.width));
        N = QVector3D(
            T.x() * Nt.x() + B.x() * Nt.y() + Np.x() * Nt.z(),
            T.y() * Nt.x() + B.y() * Nt.y() + Np.y() * Nt.z(),
            T.z() * Nt.x() + B.z() * Nt.y() + Np.z() * Nt.z()
        ).normalized();
    }
    const QVector3D L = (params.lightCoordinate - coordinate).normalized();
    float cosNL = QVector3D::dotProduct(N, L);
    if (cosNL > 1) cosNL = 1;

    const QVector3D R = 2 * cosNL * N - L;
    float cosVR = QVector3D::dotProduct(params.V, R);
    if (cosVR > 1) cosVR = 1;

    if (cosNL < 0) cosNL = 0;
    if (cosVR < 0) cosVR = 0;

    const QVector3D color = params.useBackColor ? *params.backColor : params.colors->at(ij2k(position.y(), position.x(), params.width));
    const float RILIO = color.x() * params.lightColor->x();
    const float GILIO = color.y() * params.lightColor->y();
    const float BILIO = color.z() * params.lightColor->z();

    const float kd_cosNL_ks_cosmVR_255 = 255 * (*params.kd * cosNL + *params.ks * qPow(cosVR, *params.m));

    int pR = RILIO * kd_cosNL_ks_cosmVR_255;
    int pG = GILIO * kd_cosNL_ks_cosmVR_255;
    int pB = BILIO * kd_cosNL_ks_cosmVR_255;

    if (pR > 255) pR = 255;
    if (pG > 255) pG = 255;
    if (pB > 255) pB = 255;

    image->setPixelColor(position, QColor(pR, pG, pB));
}
