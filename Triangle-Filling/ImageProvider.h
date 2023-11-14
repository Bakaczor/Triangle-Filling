#pragma once

#include <QQuickImageProvider>

class ImageProvider : public QQuickImageProvider
{
    Q_OBJECT
    public:
        QSharedPointer<QImage> image;

        explicit ImageProvider(QSharedPointer<QImage> image = nullptr);
        QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
};
