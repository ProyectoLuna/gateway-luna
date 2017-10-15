#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>

#include <QDebug>

using namespace std;

#include "daemon.h"

Daemon::Daemon(QString name, void (*inthandler)(int))
{
    _name = name;
    _pidFilename = QString("/var/run/%1").arg(name);
    sighandler = inthandler;

    qDebug() << QString("daemon name: %1, pidfile: %2").arg(_name, _pidFilename);
}

Daemon::~Daemon()
{
    finalize();
}

pid_t Daemon::read_pid(const char* name)
{
    int nsize = 16;
    char *data;
    pid_t pid;
    char pidfileaux[DAEMON_PIDFILE_LEN];
    name_to_pidfile(name, pidfileaux);
    ifstream file(pidfileaux);
    stringstream strvalue;

    if (!file.is_open())
        return 0;

    file.read(data, nsize);
    file.close();

    strvalue << data;
    strvalue >> pid;

    return pid;
}

int Daemon::write_pid(void)
{
    ofstream file(pidfilename);

    if (!file.is_open()) {
        cout << __func__ << " Error open file, " << strerror(errno) << endl;
        return 0;
    }

    pid_t pid = getpid();

    file << pid;
    file.close();
    return 0;
}

int Daemon::check_pid(const char* name)
{
    pid_t pid = read_pid(name);

    // Daemon is already holding the pid file
    if ((!pid) || (pid == getpid()))
        return 0;

    return pid;
}

void Daemon::daemonize(void)
{
    sact.sa_handler = sighandler;
    sact.sa_flags = 0;

    // Daemonize process
    if(daemon(0, 0))
        exit(0);

    // Register signal handler
    if (sigaction(SIGINT, &sact, NULL))
        cout << "sigaction error... SIGINT, " << strerror(errno) << endl;
    if (sigaction(SIGTERM, &sact, NULL))
        cout << "sigaction error... SIGTERM, " << strerror(errno) << endl;
}

void Daemon::finalize(void)
{
    unlink(pidfilename);
}
