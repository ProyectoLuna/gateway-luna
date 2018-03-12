#ifndef LUNA_GATEWAY_DAEMON_H
#define LUNA_GATEWAY_DAEMON_H

#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <QObject>
#include <QSocketNotifier>

namespace luna
{
namespace gateway
{

class Daemon: public QObject
{
    Q_OBJECT

public:
    Daemon(QObject *parent = nullptr);
    virtual ~Daemon();
    static pid_t readPid(const QString &pidFile);
    int writePid();
    static int checkPid(const QString &name);
    bool daemonize(const QString &filePath = "/var/run/luna_daemon.pid");
    bool registerSignals(const QList<int> &sigList);
    void finalize();

    static void signalHandler(int unused);

    QString name() const;
    void setName(const QString &name);
    bool isDaemonized() const;

public slots:
    void handleSignal();

signals:
    void daemonized();
    void started();
    void stopped();

private:
    QString _name;
    QString _pidFile;
    bool _daemonized = false;

    QSocketNotifier *_sn = nullptr;
};

}
}

#endif // LUNA_GATEWAY_DAEMON_H
