#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtPlugin>

auto main(int argc, char* argv[]) -> int
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/");
    engine.load(QUrl{QStringLiteral("qrc:/main.qml")});

    return QGuiApplication::exec();
}
