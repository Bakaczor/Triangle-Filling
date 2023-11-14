#include "ImageProvider.h"

ImageProvider::ImageProvider(QSharedPointer<QImage> image) : QQuickImageProvider(QQuickImageProvider::Image), image(image) {}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return *image.get();
}
