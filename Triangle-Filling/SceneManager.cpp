#include <QtConcurrent/QtConcurrent>
#include <QColorDialog>
#include <QPainter>

#include "SceneManager.h"

SceneManager::SceneManager(QObject* parent) : QObject(parent), selectedIdx(-1), m_ks(0), m_kd(0), m_m(1), m_triNum(2),
    m_showGrid(false), m_isInCPView(false), m_isPlaying(false), m_lightColor(m_white), m_backColor(m_white), m_lightCoordinate(0.5, 0.5, 2)
{
    image = QSharedPointer<QImage>(new QImage(m_size, QImage::Format_RGB888));
    image->fill(m_white);

    // set up control points
    controlPoints = QList<ControlPoint>();

    // ensure float division
    const float _3 = 3.0f;

    controlPoints.append(ControlPoint(QPoint(0, 0), QVector3D(0, 1, 0), false));
    controlPoints.append(ControlPoint(QPoint(m_size.width() / 3, 0), QVector3D(1 / _3, 1, 0), true));
    controlPoints.append(ControlPoint(QPoint(2 * m_size.width() / 3, 0), QVector3D(2 / _3, 1, 0), true));
    controlPoints.append(ControlPoint(QPoint(m_size.width(), 0), QVector3D(1, 1, 0), false));

    controlPoints.append(ControlPoint(QPoint(0, m_size.height() / 3), QVector3D(0, 2 / _3, 0), true));
    controlPoints.append(ControlPoint(QPoint(m_size.width() / 3, m_size.height() / 3), QVector3D(1 / _3, 2 / _3, 0), true));
    controlPoints.append(ControlPoint(QPoint(2 * m_size.width() / 3, m_size.height() / 3), QVector3D(2 / _3, 2 / _3, 0), true));
    controlPoints.append(ControlPoint(QPoint(m_size.width(), m_size.height() / 3), QVector3D(1, 2 / _3, 0), true));

    controlPoints.append(ControlPoint(QPoint(0, 2 * m_size.height() / 3), QVector3D(0, 1 / _3, 0), true));
    controlPoints.append(ControlPoint(QPoint(m_size.width() / 3, 2 * m_size.height() / 3), QVector3D(1 / _3, 1 / _3, 0), true));
    controlPoints.append(ControlPoint(QPoint(2 * m_size.width() / 3, 2 * m_size.height() / 3), QVector3D(2 / _3, 1 / _3, 0), true));
    controlPoints.append(ControlPoint(QPoint(m_size.width(), 2 * m_size.height() / 3), QVector3D(1, 1 / _3, 0), true));

    controlPoints.append(ControlPoint(QPoint(0, m_size.height()), QVector3D(0, 0, 0), false));
    controlPoints.append(ControlPoint(QPoint(m_size.width() / 3, m_size.height()), QVector3D(1 / _3, 0, 0), true));
    controlPoints.append(ControlPoint(QPoint(2 * m_size.width() / 3, m_size.height()), QVector3D(2 / _3, 0, 0), true));
    controlPoints.append(ControlPoint(QPoint(m_size.width(), m_size.height()), QVector3D(1, 0, 0), false));

    triangles = QList<Triangle>();
    calculateGrid();
}

void SceneManager::calculateGrid()
{
    triangles.clear();
    const float dx = m_size.width() / static_cast<float>(m_triNum);
    const float dy = m_size.height() / static_cast<float>(m_triNum);
    for (uint i = 0; i < m_triNum; i++) {
        for (uint j = 0; j < m_triNum; j++) {
            triangles.append(Triangle({ QPoint(i * dx, j * dy), QPoint((i + 1) * dx, j * dy), QPoint(i * dx, (j + 1) * dy) }, m_size));
            triangles.append(Triangle({ QPoint((i + 1) * dx, j * dy), QPoint(i * dx, (j + 1) * dy), QPoint((i + 1) * dx, (j + 1) * dy) }, m_size));
        }
    }

    QtConcurrent::blockingMap(triangles, [this](Triangle& tri) {
        tri.calculateNormalVersors(controlPoints);
    });
}

void SceneManager::paint()
{
    if (!m_isPlaying)
    {
        image->fill(m_white);
    }
    if (m_isInCPView)
    {
        for (const auto& p : controlPoints)
        {
            p.paint(image);
        }
    }
    else
    {
        const float _255 = 255.0f;
        Lambert params;
        params.ks = m_ks;
        params.kd = m_kd;
        params.m = m_m;
        params.lightCoordiante = m_lightCoordinate;
        params.lightColor = QVector3D(m_lightColor.red()/ _255, m_lightColor.green()/ _255, m_lightColor.blue()/ _255);
        params.backColor = QVector3D(m_backColor.red()/ _255, m_backColor.green()/ _255, m_backColor.blue()/ _255);

        QtConcurrent::blockingMap(triangles, [this, &params](Triangle& tri) {
            tri.fill(image, params);
        });

        if (m_showGrid)
        {
            for (const auto& tri : triangles)
            {
                tri.paint(image);
            }
        }
    }
}

void SceneManager::play()
{
    // pętla while z odmierzaniem klatek
    paint();
    emit imageChanged();
}

void SceneManager::selectPoint(int x, int y)
{
    if (selectedIdx != -1)
    {
        controlPoints[selectedIdx].unselect();
        selectedIdx = -1;
    }
    for (int i = 0; i < controlPoints.count(); i++)
    {
        if (controlPoints.at(i).equals(x, y))
        {
            if (controlPoints.at(i).modifiable)
            {
                selectedIdx = i;
                controlPoints[i].select();
            }
            break;
        }
    }
    paint();
    emit imageChanged();
}

void SceneManager::modifyPoint(float z)
{
    if (selectedIdx != -1 && controlPoints.at(selectedIdx).modifiable)
    {
        controlPoints[selectedIdx].coordinate.setZ(z);
    }
}

void SceneManager::pickLightColor()
{
    QColor color = QColorDialog::getColor();
    m_lightColor = color;
    emit lightColorChanged();
}

void SceneManager::pickBackColor()
{
    QColor color = QColorDialog::getColor();
    m_backColor = color;
    emit backColorChanged();
}

// PROPERTIES

uint SceneManager::ks() const
{
    return m_ks;
}

void SceneManager::setKs(uint newKs)
{
    if (m_ks == newKs)
        return;
    m_ks = newKs;
}

uint SceneManager::kd() const
{
    return m_kd;
}

void SceneManager::setKd(uint newKd)
{
    if (m_kd == newKd)
        return;
    m_kd = newKd;
}

uint SceneManager::m() const
{
    return m_m;
}

void SceneManager::setM(uint newM)
{
    if (m_m == newM)
        return;
    m_m = newM;
}

uint SceneManager::triNum() const
{
    return m_triNum;
}

void SceneManager::setTriNum(uint newTriNum)
{
    if (m_triNum == newTriNum)
        return;
    m_triNum = newTriNum;

    // if changed, stop, recalculate and repaint
    setIsPlaying(false);
    calculateGrid();
    paint();
    emit imageChanged();
}

bool SceneManager::isInCPView() const
{
    return m_isInCPView;
}

void SceneManager::setIsInCPView(bool newIsInCPView)
{
    if (m_isInCPView == newIsInCPView)
        return;
    m_isInCPView = newIsInCPView;

    // if changed, stop and repaint
    setIsPlaying(false);
    paint();
    emit imageChanged();
}

bool SceneManager::isPlaying() const
{
    return m_isPlaying;
}

void SceneManager::setIsPlaying(bool newIsPlaying)
{
    if (m_isPlaying == newIsPlaying)
        return;
    m_isPlaying = newIsPlaying;

    // if true, start animation
    if (m_isPlaying) play();
}


bool SceneManager::showGrid() const
{
    return m_showGrid;
}

void SceneManager::setShowGrid(bool newShowGrid)
{
    if (m_showGrid == newShowGrid)
        return;
    m_showGrid = newShowGrid;

    // if changed and not playing, repaint
    if (!m_isPlaying)
    {
        paint();
        emit imageChanged();
    }
}

QColor SceneManager::lightColor() const
{
    return m_lightColor;
}

void SceneManager::setLightColor(QColor newLightColor)
{
    if (m_lightColor == newLightColor)
        return;
    m_lightColor = newLightColor;
}

QColor SceneManager::backColor() const
{
    return m_backColor;
}

void SceneManager::setBackColor(QColor newBackColor)
{
    if (m_backColor == newBackColor)
        return;
    m_backColor = newBackColor;
}
