#include <QCoreApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include <Logger.h>
#include <ConsoleAppender.h>
#include <FileAppender.h>

#include "radiorf24.h"
#include "daemon.h"

using namespace luna;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("gateway");
    QCoreApplication::setApplicationVersion("0.0.1");

    // Config logger
    ConsoleAppender* consoleAppender = new ConsoleAppender();
    consoleAppender->setFormat("[%{type}][%{time}] <%{function}> %{message}\n");
    FileAppender* fileAppender = new FileAppender("/var/log/luna.log");
    fileAppender->setFormat("[%{type}][%{time}] <%{function}> %{message}\n");
    Logger::globalInstance()->registerAppender(fileAppender);
    Logger::globalInstance()->registerAppender(consoleAppender);

    QCommandLineParser parser;
    parser.setApplicationDescription("Gateway to manage remote devices");
    parser.addHelpOption();
    parser.addVersionOption();

    // Daemon option
    QCommandLineOption daemonOption(QStringList() << "d" << "daemon",
            QCoreApplication::translate("main", "Run this app as a daemon."));
    parser.addOption(daemonOption);

    parser.process(a);
    bool daemonize = parser.isSet(daemonOption);

    LOG_INFO("Starting the application");

    QSharedPointer<Daemon> daemon;
    if (daemonize)
    {
        try
        {
            daemon = QSharedPointer<Daemon> (new Daemon(nullptr, QString("/var/run/gateway.pid")));

            // Quit application when work is finished
            QObject::connect(daemon.data(), SIGNAL(finished()), &a, SLOT(quit()));
        }
        catch (const std::exception &e)
        {
            e.what();
            exit(1);
        }
    }

    return a.exec();
}
