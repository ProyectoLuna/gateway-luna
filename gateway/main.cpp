#include <QCoreApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include <Logger.h>
#include <ConsoleAppender.h>
#include <FileAppender.h>

#include "radiorf24.h"
#include "radio_manager.h"
#include "daemon.h"

extern "C"
{
#include "pb_encode.h"
#include "pb_decode.h"
#include "remote_devs.pb.h"
}

using namespace luna;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("gateway");
    QCoreApplication::setApplicationVersion("0.0.1");

    // Config logger
    ConsoleAppender* consoleAppender = new ConsoleAppender();
    consoleAppender->setFormat("[%{type}][%{time}][%{threadid}] <%{function}> %{message}\n");
    FileAppender* fileAppender = new FileAppender("/var/log/luna.log");
    fileAppender->setFormat("[%{type}][%{time}][%{threadid}] <%{function}> %{message}\n");
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

    LOG_INFO("Starting the application...");

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

    uint8_t buffer[256];
    size_t message_length;
    bool status;

    {
        RemoteDevMessage message = RemoteDevMessage_init_zero;

        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

        message.header.transaction_id = 1;
        message.header.unique_id.radio_id = RadioId_RID_NRF24;

        // TODO investigar el repeated

        status = pb_encode(&stream, RemoteDevMessage_fields, &message);
        message_length = stream.bytes_written;

        /* Then just check for any errors.. */
        if (!status)
        {
            qInfo() << "Encoding failed: " << PB_GET_ERROR(&stream);
            return EXIT_FAILURE;
        }
    }

    {
        RemoteDevMessage message = RemoteDevMessage_init_zero;

        /* Create a stream that reads from the buffer. */
        pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

        status = pb_decode(&stream, RemoteDevMessage_fields, &message);

        if (!status)
        {
            qInfo() << "Decoding failed: " << PB_GET_ERROR(&stream);
            return 1;
        }

        /* Print the data contained in the message. */
        LOG_INFO(QString("ID: %1, transaction: %2").arg(message.header.unique_id.radio_id).
                                                    arg(message.header.transaction_id));
    }

    RadioManager radioManager;

    return a.exec();
}
