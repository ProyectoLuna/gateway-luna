#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>

#include <exception>

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QString>
#include <QSocketNotifier>

#include "daemon.h"

using namespace std;
using namespace luna;

static int sighupFd[2];
static int sigtermFd[2];

static int setup_unix_signal_handlers()
{
    struct sigaction hup, term;

    hup.sa_handler = Daemon::hupSignalHandler;
    sigemptyset(&hup.sa_mask);
    hup.sa_flags = 0;
    hup.sa_flags |= SA_RESTART;

    if (sigaction(SIGHUP, &hup, 0) > 0)
       return 1;

    term.sa_handler = Daemon::termSignalHandler;
    sigemptyset(&term.sa_mask);
    term.sa_flags |= SA_RESTART;

    if (sigaction(SIGTERM, &term, 0) > 0)
       return 2;

    return 0;
}

Daemon::Daemon(QObject* parent, QString filePath) : QObject(parent)
{
    if (checkPid(filePath))
    {
        qDebug() << "Daemon already exists";
        throw "Daemon already exists.";
    }

    qDebug() << "Deamonizing process...";
    _pidFile = filePath;
    writePid();

    daemonize();

    qDebug() << QString("Process Deamonized, pidfile: %2").arg(_pidFile);

    emit daemonized();
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

    qDebug() << QString("Read pid: %1").arg(line);

    return line.toInt();
}

int Daemon::writePid(void)
{
    QFile file(_pidFile);
    if (!file.open(QIODevice::ReadWrite))
    {
        qDebug() << QString("Error opening pidfile: %1, %2").arg(_pidFile, strerror(errno));
        return -1;
    }

    qDebug() << "ola que ase";
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

void Daemon::daemonize(void)
{
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sighupFd))
       qFatal("Couldn't create HUP socketpair");

    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigtermFd))
       qFatal("Couldn't create TERM socketpair");
    snHup = new QSocketNotifier(sighupFd[1], QSocketNotifier::Read, this);
    connect(snHup, SIGNAL(activated(int)), this, SLOT(handleSigHup()));
    snTerm = new QSocketNotifier(sigtermFd[1], QSocketNotifier::Read, this);
    connect(snTerm, SIGNAL(activated(int)), this, SLOT(handleSigTerm()));

    // Register signal handler
    setup_unix_signal_handlers();

    // Daemonize process
    if(daemon(0, 0))
        exit(0);
}

void Daemon::finalize(void)
{
    unlink(_pidFile.toStdString().c_str());
}

void Daemon::hupSignalHandler(int)
{
    char a = 1;
    ::write(sighupFd[0], &a, sizeof(a));
}

void Daemon::termSignalHandler(int)
{
    char a = 1;
    ::write(sigtermFd[0], &a, sizeof(a));
}

void Daemon::handleSigTerm()
{
    snTerm->setEnabled(false);
    char tmp;
    ::read(sigtermFd[1], &tmp, sizeof(tmp));

    finalize();
    emit finished();

    snTerm->setEnabled(true);
}

void Daemon::handleSigHup()
{
    snHup->setEnabled(false);
    char tmp;
    ::read(sighupFd[1], &tmp, sizeof(tmp));

    finalize();
    emit finished();

    snHup->setEnabled(true);
}
