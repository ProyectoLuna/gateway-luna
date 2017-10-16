#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdbool>
#include <QString>
#include <QObject>

namespace luna
{
class Daemon: public QObject {
    Q_OBJECT

public:
    Daemon(QObject* parent = nullptr, QString filePath = "/var/run/daemon-luna.pid",
           void (*inthandler)(int) = nullptr);
    ~Daemon();
    bool setName(QString name);
    bool setIntHandler(void (*inthandler)(int));
    void intHandler(int sig);
    static pid_t readPid(QString pidFile);
    int writePid(void);
    static int checkPid(const char* name);
    void daemonize(void);
    void finalize(void);

private:
    QString _name;
    QString _pidFile;
    struct sigaction _sact;
    void (*sighandler)(int);

};
}
