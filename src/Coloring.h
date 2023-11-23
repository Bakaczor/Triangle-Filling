#pragma once

#include <QSharedPointer>
#include <QVector3D>
#include <QImage>
#include <QtMath>

#include <vector>

struct Lambert
{
    int width;
    const float* ks;
    const float* kd;
    const uint* m;
    const QVector3D* lightColor;
    const QVector3D* backColor;
    QVector3D lightCoordinate;
    const std::vector<QVector3D>* colors;
    bool useBackColor;
    const std::vector<QVector3D>* texture;
    bool useTexture;
    // new functionality
    const float* alfa;
    const float* beta;

    const QVector3D V = { 0, 0, 1 };

    Lambert(int width, float* ks, float* kd, uint* m, QVector3D* lightColor, QVector3D* backColor, float* alfa, float* beta,
        QVector3D lightCoordinate, std::vector<QVector3D>* colors, bool useBackColor, std::vector<QVector3D>* texture, bool useTexture) :
        width(width), ks(ks), kd(kd), m(m), lightColor(lightColor), backColor(backColor), lightCoordinate(lightCoordinate), colors(colors),
        useBackColor(useBackColor), texture(texture), useTexture(useTexture), alfa(alfa), beta(beta) {}
};

/*!
 * \brief Colors a pixel on an image
 * \param image A shared pointer to a QImage object
 * \param position The position of the pixel on the image
 * \param coordinate The 3D coordinate of the pixel
 * \param N The normal vector of the pixel
 * \param params The Lambert parameters
 */
void colorPixel(QSharedPointer<QImage> image, const QPoint& position, const QVector3D& coordinate, const QVector3D& N, const Lambert& params);
