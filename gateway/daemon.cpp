#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QString>
#include <QSocketNotifier>
#include <QFileInfo>

#include "daemon.h"

using namespace std;
using namespace luna;

static int signalFd[2];

static bool setup_unix_signal_handlers(int signal)
{
    struct sigaction sig = {0};

    sig.sa_handler = Daemon::signalHandler;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags |= SA_RESTART;

    if (sigaction(signal, &sig, 0) > 0)
    {
        return false;
    }

    return true;
}

Daemon::Daemon(QObject *parent) : QObject(parent)
{
}

Daemon::~Daemon()
{
    finalize();
}

pid_t Daemon::readPid(const QString &pidFile)
{
    QFile file(pidFile);
    if (!file.open(QIODevice::ReadWrite))
    {
        return 0;
    }

    QTextStream in(&file);
    QString line = in.readLine();

    return line.toInt();
}

int Daemon::writePid()
{
    QFile file(_pidFile);
    if (!file.open(QIODevice::ReadWrite))
    {
        return -1;
    }

    QTextStream stream(&file);
    stream << getpid();
    return 0;
}

int Daemon::checkPid(const QString &pidFile)
{
    pid_t pid = readPid(pidFile);

    // Daemon is already holding the pid file
    if (!pid || pid == getpid())
    {
        return 0;
    }

    return pid;
}

bool Daemon::daemonize(const QString &filePath)
{
    _pidFile = filePath;

    if (checkPid(_pidFile))
    {
        std::cout << "Daemon already exists" << std::endl;
        return false;
    }

    // Daemonize process
    if (daemon(1, 1))
    {
        return false;
    }

    writePid();

    emit daemonized();

    _daemonized = true;

    return true;
}

bool Daemon::registerSignals(const QList<int> &sigList)
{
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, signalFd))
    {
        return false;
    }
    _sn = new QSocketNotifier(signalFd[1], QSocketNotifier::Read, this);
    connect(_sn, SIGNAL(activated(int)), this, SLOT(handleSignal()));

    for (auto sig : sigList)
    {
        setup_unix_signal_handlers(sig);
    }

    return true;
}

void Daemon::finalize()
{
    QFileInfo checkFile(_pidFile);

    if (checkFile.exists() && checkFile.isFile())
    {
        QFile file(_pidFile);
        file.remove();
    }
}

void Daemon::signalHandler(int signal)
{
    ::write(signalFd[0], &signal, sizeof(int));
}

void Daemon::handleSignal()
{
    _sn->setEnabled(false);
    int signal;
    ::read(signalFd[1], &signal, sizeof(signal));

    qInfo() << QString("Received %1 signal (%2), proceed to exit").arg(QString(strsignal(signal)), QString::number(signal));

    finalize();
    emit stopped();

    _sn->setEnabled(true);
}

bool Daemon::isDaemonized() const
{
    return _daemonized;
}

QString Daemon::name() const
{
    return _name;
}

void Daemon::setName(const QString &name)
{
    _name = name;
}
