#include "SceneManager.h"

SceneManager::SceneManager(QObject* parent, QApplication* app) : QObject(parent), selectedIdx(-1), m_alfa(0), m_beta(0), m_ks(0.5), m_kd(0.5),
    m_m(50), m_triNum(7), m_fillGrid(false), m_showGrid(true), m_isInCPView(false), m_isPlaying(false), m_gridType(true),
    m_lightColor(1, 1, 1), m_backColor(1, 1, 1), m_lightCoordinate(), m_app(app),
    m_colors(m_size.width() * m_size.height()), m_useBackColor(true), m_texture(m_size.width() * m_size.height()), m_useTexture(false)
{
    image = QSharedPointer<QImage>(new QImage(m_size, QImage::Format_RGB32));
    image->fill(m_white);

    // set up control points (manually)
    controlPoints = QList<ControlPoint>();

    // ensure float division
    const float _13 = 1.0f / 3.0f;
    const float _23 = 2.0f / 3.0f;
    const float w_13 = _13 * m_size.width();
    const float w_23 = _23 * m_size.width();
    const float h_13 = _13 * m_size.height();
    const float h_23 = _23 * m_size.height();

    controlPoints.append(ControlPoint(QPoint(0, 0), QVector3D(0, 1, 0), false));
    controlPoints.append(ControlPoint(QPoint(m_size.width(), h_23), QVector3D(1, _13, 0), true));
    controlPoints.append(ControlPoint(QPoint(m_size.width(), h_13), QVector3D(1, _23, 0), true));
    controlPoints.append(ControlPoint(QPoint(0, m_size.height()), QVector3D(0, 0, 0), false));

    controlPoints.append(ControlPoint(QPoint(w_23, m_size.height()), QVector3D(_23, 0, 0), true));
    controlPoints.append(ControlPoint(QPoint(w_13, h_13), QVector3D(_13, _23, 0), true));
    controlPoints.append(ControlPoint(QPoint(w_13, h_23), QVector3D(_13, _13, 0), true));
    controlPoints.append(ControlPoint(QPoint(w_23, 0), QVector3D(_23, 1, 0), true));

    controlPoints.append(ControlPoint(QPoint(w_13, m_size.height()), QVector3D(_13, 0, 0), true));
    controlPoints.append(ControlPoint(QPoint(w_23, h_13), QVector3D(_23, _23, 0), true));
    controlPoints.append(ControlPoint(QPoint(w_23, h_23), QVector3D(_23, _13, 0), true));
    controlPoints.append(ControlPoint(QPoint(w_13, 0), QVector3D(_13, 1, 0), true));

    controlPoints.append(ControlPoint(QPoint(m_size.width(), 0), QVector3D(1, 1, 0), false));
    controlPoints.append(ControlPoint(QPoint(0, h_23), QVector3D(0, _13, 0), true));
    controlPoints.append(ControlPoint(QPoint(0, h_13), QVector3D(0, _23, 0), true));
    controlPoints.append(ControlPoint(QPoint(m_size.width(), m_size.height()), QVector3D(1, 0, 0), false));

    triangles = QList<Triangle>();
    calculateGrid();
    paint();
}

void SceneManager::calculateGrid()
{
    triangles.clear();
    const float dx = (m_size.width() - 1) / static_cast<float>(m_triNum);
    const float dy = (m_size.height() - 1) / static_cast<float>(m_triNum);
    for (uint i = 0; i < m_triNum; i++) {
        for (uint j = 0; j < m_triNum; j++) {
            triangles.append(Triangle({ QPoint(i * dx, j * dy), QPoint((i + 1) * dx, j * dy), QPoint(i * dx, (j + 1) * dy) }, m_size));
            triangles.append(Triangle({ QPoint((i + 1) * dx, j * dy), QPoint(i * dx, (j + 1) * dy), QPoint((i + 1) * dx, (j + 1) * dy) }, m_size));
        }
    }

    QtConcurrent::blockingMap(triangles, [this](Triangle& tri) {
        tri.calculateNormalVersorsAndZ(controlPoints, m_gridType);
    });
}

void SceneManager::paint()
{
    image->fill(m_white);
    if (m_isInCPView)
    {
        for (const ControlPoint& p : controlPoints)
        {
            p.paint(image);
        }
    }
    else
    {
        if (m_fillGrid) {
            Lambert params(m_size.width(), &m_ks, &m_kd, &m_m, &m_lightColor, &m_backColor, &m_alfa, &m_beta,
                           m_isPlaying ? m_lightCoordinate.next() : m_lightCoordinate.current(), &m_colors, m_useBackColor, &m_texture, m_useTexture);

            QtConcurrent::blockingMap(triangles, [this, &params](Triangle& tri) {
                tri.fill(image, params);
            });
        }

        if (m_showGrid)
        {
            for (const Triangle& tri : triangles)
            {
                tri.paint(image, m_alfa, m_beta);
            }
        }
    }
}

void SceneManager::play()
{
    const int frame = 30;
    QElapsedTimer timer;
    timer.start();
    while (m_isPlaying)
    {
        timer.restart();
        paint();
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
    // update slider
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

float SceneManager::getCurrentZ()
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

    // if changed and not playing, repaint
    if (!m_isPlaying)
    {
        paint();
        emit imageChanged();
    }
}

void SceneManager::pickBackColor()
{
    const float _255 = 255.0f;
    QColor color = QColorDialog::getColor();
    m_backColor = QVector3D(color.red() / _255, color.green() / _255, color.blue() / _255);
    m_useBackColor = true;
    emit backColorChanged();

    // if changed and not playing, repaint
    if (!m_isPlaying)
    {
        paint();
        emit imageChanged();
    }
}

void SceneManager::pickImage()
{
    setIsPlaying(false);
    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open File"), "/home", tr("Images (*.png *.jpg)"));
    if (!fileName.isEmpty())
    {
        QImage background(fileName);
        background = background.scaled(m_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        background.convertTo(QImage::Format_RGB32);
        QRgb* data = reinterpret_cast<QRgb*>(background.bits());
        int N = m_size.width() * m_size.height();
        const float _255 = 255.0f;
        for (int i = 0; i < N; i++)
        {
           m_colors[i] = QVector3D(qRed(data[i]) / _255, qGreen(data[i]) / _255, qBlue(data[i]) / _255);
        }
        m_useBackColor = false;
    }
    else
    {
        m_useBackColor = true;
    }
    paint();
    emit imageChanged();
}

void SceneManager::pickTexture()
{
    setIsPlaying(false);
    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open File"), "/home", tr("Images (*.png *.jpg)"));
    if (!fileName.isEmpty())
    {
        QImage background(fileName);
        background = background.scaled(m_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        background.convertTo(QImage::Format_RGB32);
        QRgb* data = reinterpret_cast<QRgb*>(background.bits());
        int N = m_size.width() * m_size.height();
        const float _127_5 = 127.5f;
        for (int i = 0; i < N; i++)
        {
           m_texture[i] = QVector3D((qRed(data[i]) - _127_5) / _127_5, (qGreen(data[i]) - _127_5) / _127_5, (qBlue(data[i]) - _127_5) / _127_5);
        }
        m_useTexture = true;
    }
    else
    {
        m_useTexture = false;
    }
    paint();
    emit imageChanged();
}

void SceneManager::changeGridType()
{
    m_gridType = !m_gridType;
    // if changed, stop, recalculate and repaint
    setIsPlaying(false);
    calculateGrid();
    paint();
    emit imageChanged();
}

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

void SceneManager::setFillGrid(bool newFillGrid)
{
    if (m_fillGrid == newFillGrid)
        return;
    m_fillGrid = newFillGrid;

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
    m_useBackColor = true;

    // if changed and not playing, repaint
    if (!m_isPlaying)
    {
        paint();
        emit imageChanged();
    }
}

void SceneManager::setAlfa(float newAlfa)
{
    if (m_alfa == newAlfa)
        return;
    m_alfa = newAlfa;

    // if changed and not playing, repaint
    if (!m_isPlaying)
    {
        paint();
        emit imageChanged();
    }
}

void SceneManager::setBeta(float newBeta)
{
    if (m_beta == newBeta)
        return;
    m_beta = newBeta;

    // if changed and not playing, repaint
    if (!m_isPlaying)
    {
        paint();
        emit imageChanged();
    }
}

bool SceneManager::fillGrid() const
{
    return m_fillGrid;
}

float SceneManager::alfa() const
{
    return m_alfa;
}

float SceneManager::beta() const
{
    return m_beta;
}
