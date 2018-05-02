#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <QString>

#include <Logger.h>
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <RF24Mesh/RF24Mesh.h>

#include "iradio.h"
#include "radiorf24.h"
#include "protos/nanopb/lunapb.h"
#include "message/message.h"

using namespace luna;
using namespace radio;

RF24 radiorf24(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);
RF24Network network(radiorf24);
RF24Mesh mesh(radiorf24,network);

RadioRF24::RadioRF24(QObject *parent) : RadioBase(parent)
{
    //setParent(parent);

    _name = QString("radioRF24");
    _radioId = RadioId_RID_NRF24;
    _gameover = false;

    // Set the nodeID to 0 for the master node
    mesh.setNodeID(0);

    // Connect to the mesh
    printf("start\n");
    mesh.begin();
    radiorf24.printDetails();
}

bool RadioRF24::start()
{
    while (not _gameover)
    {
        // Call network.update as usual to keep the network updated
        mesh.update();

        // In addition, keep the 'DHCP service' running on the master node so addresses will
        // be assigned to the sensor nodes
        mesh.DHCP();

        // Check for incoming data from the sensors
        while (network.available())
        {
            RF24NetworkHeader header;
            network.peek(header);
            RF24NetworkHeader& header_tonode = *(RF24NetworkHeader*)network.frame_buffer;
            const char *msg = "olaqase";

            uint8_t buffer[256];
            size_t message_length;

            switch (header.type)
            {
            // Display the incoming millis() values from the sensor nodes
            case 'M':
            {
                message_length = network.read(header, buffer, sizeof(buffer));
                RemoteDevMessage *message = new RemoteDevMessage;
                *message = RemoteDevMessage_init_zero;

                // Create a stream that reads from the buffer
                pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

                RepeatedDevData *repeatedData = new RepeatedDevData;
                repeatedData->num = 0;
                RemoteDevData *devData = new RemoteDevData[8]; // TODO parameterize, allowed max sensor data
                repeatedData->data = devData;

                message->data.funcs.decode = &decode_devdata;
                message->data.arg = repeatedData;

                bool status = pb_decode(&stream, RemoteDevMessage_fields, message);

                LOG_DEBUG(QString("node: %3").arg(QString::number(header.from_node, 8)));

                //header_tonode.to_node = header.from_node;
                //bool ret = network.write(header_tonode, (const void *)msg, (unsigned short int)strlen(msg));
                //if (not ret)
                //{
                //    LOG_WARNING(QString("Error writing"));
                //}

                if (!status)
                {
                    LOG_WARNING(QString("Decoding failed: %1").arg(QString(PB_GET_ERROR(&stream))));
                    break;
                }

                header_tonode.to_node = header.from_node;
                _deviceTable[message->header.unique_id.id32] = header_tonode;

                emit rxMessage(message);

                break;
            }
            default:
                network.read(header, 0, 0);
                LOG_WARNING(QString("Rcv bad type %1 from 0%2")
                            .arg(header.type)
                            .arg(header.from_node));
                break;
            }
        }
        delay(2);
    }
    return 0;
}

void RadioRF24::stop()
{
    LOG_INFO("Stopping rf24...");
    _gameover = true;
    LOG_INFO("Stopped rf24");
}

void RadioRF24::quit()
{
    emit finished();
}

bool RadioRF24::send(QSharedPointer<message::Message<RepeatedDevData>> message)
{
    RemoteDevMessage *nanopb = message->getProto();
    quint32 devid = nanopb->header.unique_id.id32;

    uint8_t buffer[1024];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    bool status = pb_encode(&stream, RemoteDevMessage_fields, nanopb);

    if (not status)
    {
        LOG_WARNING(QString("Decoding failed: %1").arg(QString(PB_GET_ERROR(&stream))));
        return false;
    }

    if (not _deviceTable.contains(devid))
    {
        LOG_WARNING(QString("Device %1 not found").arg(devid));
        return false;
    }

    LOG_DEBUG(QString("sending %1 bytes").arg(stream.bytes_written));
    bool ret = network.write(_deviceTable[devid], (const void *)&buffer, static_cast<uint16_t>(stream.bytes_written));
    if (not ret)
    {
        LOG_WARNING(QString("Error writing"));
    }

    return true;
}
