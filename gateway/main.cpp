#include <QCoreApplication>
#include <QDebug>
#include "radiorf24.h"
#include "daemon.h"

using namespace luna;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Daemon* daemon;

    try
    {
        daemon = new Daemon(nullptr, QString("/var/run/gateway.pid"));

        // Quit application when work is finished
        QObject::connect(daemon, SIGNAL(finished()), &a, SLOT(quit()));
    }
    catch (const std::exception &e)
    {
        e.what();
        exit(1);
    }
//    RadioRF24 radiorf24;
//    radiorf24.check_remotes();

    return a.exec();
}
