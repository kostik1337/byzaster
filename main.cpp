#include "byzanzproxy.h"
#include "clipboard.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QQuickWindow::setDefaultAlphaBuffer(true);
    
    qmlRegisterType<ByzanzProxy>("io.github.dotbread.byzaster", 1, 0, "ByzanzProxy");
    qmlRegisterType<Clipboard>("io.github.dotbread.byzaster", 1, 0, "Clipboard");
    
    QGuiApplication app(argc, argv);
    
    app.setOrganizationName(".bread");
    app.setOrganizationDomain("dotbread.github.io");
    app.setApplicationName("Byzaster");
    
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    
    return app.exec();
}
