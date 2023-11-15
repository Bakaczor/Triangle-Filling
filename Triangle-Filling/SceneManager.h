#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QImage>

#include "Triangle.h"

class SceneManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float ks READ ks NOTIFY ksChanged FINAL)
    Q_PROPERTY(float kd READ kd NOTIFY kdChanged FINAL)
    Q_PROPERTY(uint m READ m NOTIFY mChanged FINAL)
    Q_PROPERTY(uint triNum READ triNum NOTIFY triNumChanged FINAL)
    Q_PROPERTY(bool showGrid READ showGrid NOTIFY showGridChanged FINAL)
    Q_PROPERTY(bool isInCPView READ isInCPView NOTIFY isInCPViewChanged FINAL)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged FINAL)
    Q_PROPERTY(QColor lightColor READ lightColor NOTIFY lightColorChanged FINAL)
    Q_PROPERTY(QColor backColor READ backColor NOTIFY backColorChanged FINAL)
    public:
        QSharedPointer<QImage> image;
        QList<ControlPoint> controlPoints;
        QList<Triangle> triangles;
        uint selectedIdx;

        explicit SceneManager(QObject* parent = nullptr, QApplication* app = nullptr);

        void calculateGrid();
        void paint(const QVector3D& lightCoordinate = QVector3D(-1, -1, -1));
        void play();

        float ks() const;
        float kd() const;
        uint m() const;
        bool isInCPView() const;
        bool isPlaying() const;
        uint triNum() const;
        bool showGrid() const;
        QColor lightColor() const;
        QColor backColor() const;

    public slots:
        int getCurrentZ();
        void selectPoint(int x, int y);
        void modifyPoint(float z);
        void pickLightColor();
        void pickBackColor();

        void setKs(float newKs);
        void setKd(float newKd);
        void setM(uint newM);
        void setIsInCPView(bool newIsInCPView);
        void setIsPlaying(bool newIsPlaying);
        void setTriNum(uint newTriNum);
        void setShowGrid(bool newShowGrid);
        void setLightColor(QColor newLightColor);
        void setBackColor(QColor newBackColor);

    signals:
        void imageChanged();
        void updateCurrentZ();

        void ksChanged();
        void kdChanged();
        void mChanged();

        void isInCPViewChanged();
        void isPlayingChanged();
        void triNumChanged();
        void showGridChanged();

        void lightColorChanged();
        void backColorChanged();

    private:
        const QColor m_white = QColor(255, 255, 255);
        const QSize m_size = QSize(600, 600);

        float m_ks;
        float m_kd;
        uint m_m;
        uint m_triNum;

        bool m_showGrid;
        bool m_isInCPView;
        bool m_isPlaying;

        QVector3D m_lightColor;
        QVector3D m_backColor;
        Circle m_lightCoordinate;

        QApplication* m_app;
};
