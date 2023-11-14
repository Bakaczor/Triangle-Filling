#include <QVector3D>
#include <QPoint>
#include <QImage>
#include <QSharedPointer>

class ControlPoint
{
    public:
        QPoint position;
        QVector3D coordinate;
        bool modifiable;

        ControlPoint(QPoint position, QVector3D coordinate, bool modifiable);
        void paint(QSharedPointer<QImage> image) const;
        bool equals(const int& x, const int& y) const;
        void select();
        void unselect();

    private:
        const static uint m_radius = 25;
        QColor m_color;
};
