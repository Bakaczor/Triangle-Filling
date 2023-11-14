#include <QSize>
#include <QVector2D>
#include <QList>

class Circle
{
    public:
        Circle(uint numOfPoints = 120);
        QVector3D current() const;
        QVector3D next();
    private:
        const uint m_lightZ = 2;
        uint m_i;
        uint m_numOfPoints;
        QList<QVector2D> m_points;
};
