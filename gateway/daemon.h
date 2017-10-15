#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdbool>
#include <QString>

namespace luna
{
class Daemon {
private:
    QString _name;
    QString _pidFileName;
    struct sigaction _sact;
    void (*sighandler)(int);

public:
    DaemonLuna();
    DaemonLuna(QString name, void (*inthandler)(int));
    bool setName(QString name);
    bool setIntHandler(void (*inthandler)(int));
    ~DaemonLuna();
    void intHandler(int sig);
    static pid_t readPid(QString name);
    int writePid(void);
    static int checkPid(const char* name);
    void daemonize(void);
    void finalize(void);
};
}
