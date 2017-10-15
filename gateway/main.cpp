#include <QCoreApplication>
#include <QDebug>
#include "radiorf24.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Soy una raspberry";

    RadioRF24 radiorf24;
    radiorf24.check_remotes();
    return 0;
}
