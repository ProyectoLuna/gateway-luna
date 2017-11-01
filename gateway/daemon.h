#ifndef DAEMON_H
#define DAEMON_H

#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdbool>

#include <QObject>
#include <QSocketNotifier>

namespace luna
{
class Daemon: public QObject {
    Q_OBJECT

public:
    Daemon(QObject* parent = nullptr, QString filePath = QString("/var/run/daemon-luna.pid"));
    virtual ~Daemon();
    bool setName(QString &name) {_name = name; return true;}
    static pid_t readPid(const QString &pidFile);
    int writePid(void);
    static int checkPid(const QString &name);
    void daemonize(void);
    void finalize(void);

    // Unix signal handlers.
    static void hupSignalHandler(int unused);
    static void termSignalHandler(int unused);

public slots:
    // Qt signal handlers.
    void handleSigHup();
    void handleSigTerm();

Q_SIGNALS:
    void daemonized();
    void started();
    void stopped();
    void finished();

private:
    QString _name;
    QString _pidFile;

    QSocketNotifier *snHup;
    QSocketNotifier *snTerm;
};
}
//#include "daemon.moc"
//#include "moc_daemon.cpp"
#endif // DAEMON_H
