#pragma once

#include <QQuickImageProvider>

/*!
 * \brief The ImageProvider class
 * This class provides an interface for supporting pixmaps and threaded image requests in QML.
 */
class ImageProvider : public QQuickImageProvider
{
    Q_OBJECT
    public:
        /*!
        * \brief Constructs an ImageProvider object
        * \param image A shared pointer to a QImage object
        */
        explicit ImageProvider(QSharedPointer<QImage> image = nullptr);
        /*!
        * \brief Requests an image
        * \param id The id of the image
        * \param size A pointer to a QSize object that will be filled with the size of the image
        * \param requestedSize The requested size of the image
        * \return A QImage object representing the requested image
        */
        QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    private:
        QSharedPointer<QImage> m_image;
};
