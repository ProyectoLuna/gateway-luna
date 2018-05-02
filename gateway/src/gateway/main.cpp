#include <QCoreApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include <Logger.h>
#include <ConsoleAppender.h>
#include <FileAppender.h>
#include <qhttpserver.hpp>

#include "daemon.h"
#include "gateway.h"
#include "protos/nanopb/lunapb.h"

using namespace luna;
using namespace gateway;

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
//        RepeatedDevData repeatedData;
//
//        RemoteDevData devData[4];
//        int numData = 2;
//
//        devData[0].sensor_data.unit = SensorUnits_SU_MAH;
//        devData[0].sensor_data.value = 133;
//        devData[0].has_sensor_data = true;
//        devData[1].sensor_command.command = SensorCommandType_SCT_TOGGLE;
//        devData[1].sensor_command.data = 0;
//        devData[1].has_sensor_command = true;
//
//        repeatedData.data = devData;
//        repeatedData.num = numData;
//
//        message.data.funcs.encode = &encode_repeated_devdata;
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
//        RepeatedDevData repeatedData;
//        repeatedData.num = 0;
//
//        RemoteDevData devData[8];
//        repeatedData.data = devData;
//
//        message.data.funcs.decode = &decode_devdata;
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
//        LOG_INFO(QString("hasdata: %1, hascmd: %2").arg(devData[0].has_sensor_data).arg(devData[0].has_sensor_command));
//        LOG_INFO(QString("data unit: %1, data value: %2, cmd: %3, cmddata: %4")
//                 .arg(devData[0].sensor_data.unit)
//                 .arg(devData[0].sensor_data.value)
//                 .arg(devData[1].sensor_command.command)
//                 .arg(devData[1].sensor_command.data)
//                 );
//
//    }
//    return 0;
    // --------------------------------------------------------

    Gateway gateway;

    Daemon daemon;
    daemon.registerSignals({SIGTERM, SIGHUP, SIGINT});

    // Quit application when work is finished
    QObject::connect(&daemon, SIGNAL(stopped()), &gateway, SLOT(stop()));
    QObject::connect(&gateway, SIGNAL(stopped()), &a, SLOT(quit()));
    
    if (daemonize)
    {
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
