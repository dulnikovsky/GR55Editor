#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "patch.h"

#include "gr55editorapplication.h"

#include "datawrappers/patchnamewrapper.h"
#include "datawrappers/patchpcmwrapper.h"

const char *appName = "GR-55 Editor";

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    GR55EditorApplication app(argc, argv, appName);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    qmlRegisterType<PatchNameWrapper>("GR55EditorTypes", 1, 0, "PatchNameWrapper");
    qmlRegisterType<PatchPcmWrapper>("GR55EditorTypes", 1, 0, "PatchPcmWrapper");

    PatchNameWrapper *pnamewrapper = new PatchNameWrapper(app.TemporaryPatch());
    pnamewrapper->setBaseOffset(static_cast<unsigned int>(Patch::SysExDataOffset::Common));
    engine.rootContext()->setContextProperty(QStringLiteral("patchnamewrapper"), pnamewrapper);

    PatchPcmWrapper *ppcmwrapper1 = new PatchPcmWrapper(app.TemporaryPatch());
    ppcmwrapper1->setBaseOffset(static_cast<unsigned int>(Patch::SysExDataOffset::PcmTone1));
    engine.rootContext()->setContextProperty(QStringLiteral("patchpcmwrapper1"), ppcmwrapper1);

    PatchPcmWrapper *ppcmwrapper2 = new PatchPcmWrapper(app.TemporaryPatch());
    ppcmwrapper2->setBaseOffset(static_cast<unsigned int>(Patch::SysExDataOffset::PcmTone2));
    engine.rootContext()->setContextProperty(QStringLiteral("patchpcmwrapper2"), ppcmwrapper2);

    engine.load(url);

    return app.exec();
}
