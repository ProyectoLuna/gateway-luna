#include <unistd.h>

#include <stdint.h>
#include <string.h>

#include <QString>

#include <Logger.h>

#include "iradio.h"
#include "radiorf24.h"

#include "lunapb.h"

using namespace luna;
using namespace radio;

RF24 radiorf24(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);
RF24Network network(radiorf24);
RF24Mesh mesh(radiorf24,network);

RadioRF24::RadioRF24(QObject *parent) : IRadio()
{
    setParent(parent);

    _name = QString("radioRF24");
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
            bool status;
            //uint32_t dat = 0;
            switch (header.type)
            {
            // Display the incoming millis() values from the sensor nodes
            case 'M':
            {
                message_length = network.read(header, buffer, sizeof(buffer));
                RemoteDevMessage message = RemoteDevMessage_init_zero;

                /* Create a stream that reads from the buffer. */
                pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

                RepeatedSensorData repeatedData;
                repeatedData.num = 0;
                SensorData sensorData[8]; // TODO parameterize
                repeatedData.data = sensorData;

                message.data.funcs.decode = &decode_sensordata;
                message.data.arg = &repeatedData;

                status = pb_decode(&stream, RemoteDevMessage_fields, &message);

                if (!status)
                {
                    LOG_ERROR(QString("Decoding failed: %1").arg(QString(PB_GET_ERROR(&stream))));
                    break;
                }

                LOG_INFO(QString("ID: %1, radioID: %2, transaction: %3")
                         .arg(message.header.unique_id.id32)
                         .arg(message.header.unique_id.radio_id)
                         .arg(message.header.transaction_id)
                         );

                for (int i = 0; i < repeatedData.num; ++i)
                {
                    LOG_INFO(QString("Unit: %1, value: %2")
                             .arg(repeatedData.data[i].unit)
                             .arg(repeatedData.data[i].value)
                             );
                }

                /* Print the data contained in the message. */
                LOG_INFO(QString("node: %3").arg(QString::number(header.from_node, 8)));

                header_tonode.to_node = header.from_node;

                int attempts = 0;
                while (attempts < 3)
                {
                    bool ret = network.write(header_tonode, (const void *)msg, (unsigned short int)strlen(msg));
                    if (ret)
                    {
                        break;
                    }
                    LOG_INFO(QString("Error writing"));
                    ++attempts;
                }

                //network.read(header, &dat, sizeof(dat));
                //LOG_INFO(QString("Rcv %1 from %2").arg(dat).arg(QString::number(header.from_node, 8)));
                //header_tonode.to_node = header.from_node;
                //network.write(header_tonode, (const void *)msg, (unsigned short int)strlen(msg));

                //emit rxMessage(QString::fromLocal8Bit(reinterpret_cast<char*>(buffer, message_length)));
                //emit rxMessage(QString::number(message.header.unique_id.radio_id));

                break;
            }
            default:
                network.read(header, 0, 0);
                LOG_WARNING(QString("Rcv bad type %1 from 0%2").arg(header.type,
                                                                    header.from_node));
                break;
            }
        }
        delay(2);
    }
    return 0;
}

QString RadioRF24::getName()
{
    return _name;
}

QObject *RadioRF24::getObject()
{
    return this;
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
