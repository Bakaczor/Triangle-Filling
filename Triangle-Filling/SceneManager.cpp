#include <QtConcurrent/QtConcurrent>
#include <QColorDialog>
#include <QPainter>
#include <QApplication>

#include "SceneManager.h"

SceneManager::SceneManager(QObject* parent, QApplication* app) : QObject(parent), selectedIdx(-1), m_ks(0.5), m_kd(0.5), m_m(50), m_triNum(2),
    m_showGrid(false), m_isInCPView(false), m_isPlaying(false), m_lightColor(1, 1, 1), m_backColor(1, 1, 1), m_lightCoordinate(), m_app(app)
{
    image = QSharedPointer<QImage>(new QImage(m_size, QImage::Format_RGB888));
    image->fill(m_white);

    // set up control points (manually)
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
        tri.calculateNormalVersorsAndZ(controlPoints);
    });
}

void SceneManager::paint(const QVector3D& lightCoordinate)
{
    image->fill(m_white);
    if (m_isInCPView)
    {
        for (const auto& p : controlPoints)
        {
            p.paint(image);
        }
    }
    else
    {
        Lambert params;
        params.ks = &m_ks;
        params.kd = &m_kd;
        params.m = &m_m;
        params.lightColor = &m_lightColor;
        params.backColor = &m_backColor;
        if (lightCoordinate == QVector3D(-1, -1, -1))
        {
            params.lightCoordinate = m_lightCoordinate.current();
        }
        else
        {
            params.lightCoordinate = lightCoordinate;
        }


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
    const int frame = 33;
    QElapsedTimer timer;
    timer.start();
    while (m_isPlaying)
    {
        timer.restart();
        paint(m_lightCoordinate.next());
        const int remaining = frame - timer.elapsed();
        const int toWait = remaining < 0 ? 0 : remaining;
        QThread::msleep(toWait);
        emit imageChanged();
        // check if there were any actions in GUI
        m_app->processEvents(QEventLoop::AllEvents);
    }
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
    emit updateCurrentZ();

    paint();
    emit imageChanged();
}

void SceneManager::modifyPoint(float z)
{
    if (selectedIdx != -1 && controlPoints.at(selectedIdx).modifiable)
    {
        controlPoints[selectedIdx].coordinate.setZ(z);

        // if modified, recalculate
        calculateGrid();
    }
}

int SceneManager::getCurrentZ()
{
    if (selectedIdx != -1)
    {
        return controlPoints.at(selectedIdx).coordinate.z();
    }
    return 0;
}

void SceneManager::pickLightColor()
{
    const float _255 = 255.0f;
    QColor color = QColorDialog::getColor();
    m_lightColor = QVector3D(color.red() / _255, color.green() / _255, color.blue() / _255);
    emit lightColorChanged();
}

void SceneManager::pickBackColor()
{
    const float _255 = 255.0f;
    QColor color = QColorDialog::getColor();
    m_backColor = QVector3D(color.red() / _255, color.green() / _255, color.blue() / _255);
    emit backColorChanged();
}

// PROPERTIES

float SceneManager::ks() const
{
    return m_ks;
}

void SceneManager::setKs(float newKs)
{
    if (m_ks == newKs)
        return;
    m_ks = newKs;

    // if changed and not playing, repaint
    if (!m_isPlaying)
    {
        paint();
        emit imageChanged();
    }
}

float SceneManager::kd() const
{
    return m_kd;
}

void SceneManager::setKd(float newKd)
{
    if (m_kd == newKd)
        return;
    m_kd = newKd;

    // if changed and not playing, repaint
    if (!m_isPlaying)
    {
        paint();
        emit imageChanged();
    }
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

    // if changed and not playing, repaint
    if (!m_isPlaying)
    {
        paint();
        emit imageChanged();
    }
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
    // can't play in CPView
    if (m_isInCPView)
    {
        m_isPlaying = false;
        return;
    }

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
    return QColor(255 * m_lightColor.x(), 255 * m_lightColor.y(), 255 * m_lightColor.z());
}

void SceneManager::setLightColor(QColor newLightColor)
{
    const float _255 = 255.0f;
    QVector3D newColor(newLightColor.red() / _255, newLightColor.green() / _255, newLightColor.blue() / _255);
    if (m_lightColor == newColor)
        return;
    m_lightColor = newColor;

    // if changed and not playing, repaint
    if (!m_isPlaying)
    {
        paint();
        emit imageChanged();
    }
}

QColor SceneManager::backColor() const
{
    return QColor(255 * m_backColor.x(), 255 * m_backColor.y(), 255 * m_backColor.z());
}

void SceneManager::setBackColor(QColor newBackColor)
{
    const float _255 = 255.0f;
    QVector3D newColor(newBackColor.red() / _255, newBackColor.green() / _255, newBackColor.blue() / _255);
    if (m_backColor == newColor)
        return;
    m_backColor = newColor;

    // if changed and not playing, repaint
    if (!m_isPlaying)
    {
        paint();
        emit imageChanged();
    }
}
