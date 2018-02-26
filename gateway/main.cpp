#include <QCoreApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include <Logger.h>
#include <ConsoleAppender.h>
#include <FileAppender.h>

#include "daemon.h"
#include "gateway.h"
#include "lunapb.h"

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

    // --------------------------------------------------------
//    uint8_t buffer[1024];
//    size_t message_length;
//    bool status;
//
//    // ENCODE
//    {
//        RemoteDevMessage message = RemoteDevMessage_init_zero;
//
//        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
//
//        message.header.transaction_id = 1;
//        message.header.unique_id.radio_id = RadioId_RID_NRF24;
//        message.header.unique_id.id32 = 13;
//
//        RepeatedSensorData repeatedData;
//
//        SensorData sensorData[4];
//        int numData = 4;
//        for (int i = 0; i < numData; ++i)
//        {
//            sensorData[i].unit = SensorUnits_SU_MAH;
//            sensorData[i].value = i;
//        }
//
//        repeatedData.data = sensorData;
//        repeatedData.num = numData;
//
//        message.data.funcs.encode = &encode_repeated_sensordata;
//        message.data.arg = &repeatedData;
//
//        status = pb_encode(&stream, RemoteDevMessage_fields, &message);
//        message_length = stream.bytes_written;
//
//        /* Then just check for any errors.. */
//        if (!status)
//        {
//            LOG_ERROR("Encoding failed");
//        }
//    }
//
//    // DECODE
//    {
//        RemoteDevMessage message = RemoteDevMessage_init_zero;
//
//        /* Create a stream that reads from the buffer. */
//        pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);
//
//        RepeatedSensorData repeatedData;
//        repeatedData.num = 0;
//        SensorData sensorData[8];
//        repeatedData.data = sensorData;
//
//        message.data.funcs.decode = &decode_sensordata;
//        message.data.arg = &repeatedData;
//
//        status = pb_decode(&stream, RemoteDevMessage_fields, &message);
//
//        if (!status)
//        {
//            LOG_ERROR(QString("Decoding failed: %1").arg(QString(PB_GET_ERROR(&stream))));
//        }
//
//        LOG_INFO(QString("ID: %1, radioID: %2, transaction: %3, num Sensor data: %4")
//                 .arg(message.header.unique_id.id32)
//                 .arg(message.header.unique_id.radio_id)
//                 .arg(message.header.transaction_id)
//                 .arg(repeatedData.num)
//                 );
//
//        for (int i = 0; i < repeatedData.num; ++i)
//        {
//            LOG_INFO(QString("Unit: %1, value: %2")
//                     .arg(repeatedData.data[i].unit)
//                     .arg(repeatedData.data[i].value)
//                     );
//        }
//
//    }
//    return 0;
    // --------------------------------------------------------

    Gateway gateway;

    Daemon daemon;
    daemon.registerSignals({SIGTERM, SIGHUP, SIGINT});

    // Quit application when work is finished
    QObject::connect(daemon.data(), SIGNAL(finished()), &gateway, SLOT(stop()));
    QObject::connect(&gateway, SIGNAL(stopped()), &a, SLOT(quit()));
    
    if (daemonize)
    {
        // Run as a UNIX service
        bool ret = daemon.daemonize("var/run/gateway.pid");
        if (not ret)
        {
            LOG_ERROR("Gateway service is already running");
            exit(EXIT_FAILURE);
        }
    }

    bool ret = gateway.start();
    if (not ret)
    {
        LOG_ERROR("Error starting gateway");
        exit(EXIT_FAILURE);
    }

    return a.exec();
}
