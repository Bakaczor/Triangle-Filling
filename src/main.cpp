#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QApplication>

#include "SceneManager.h"
#include "ImageProvider.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPointer<SceneManager> manager = new SceneManager(nullptr, &app);
    QPointer<ImageProvider> provider = new ImageProvider(manager->image);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("SceneManager", manager);
    engine.addImageProvider(QString("ImageProvider"), provider);

    const QUrl url(u"qrc:/Triangle-Filling/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
