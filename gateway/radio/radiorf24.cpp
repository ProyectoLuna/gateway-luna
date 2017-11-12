#include <stdint.h>
#include <string.h>

#include <QString>

#include <Logger.h>

#include "iradio.h"
#include "radiorf24.h"

using namespace luna;

RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);
RF24Network network(radio);
RF24Mesh mesh(radio,network);

RadioRF24::RadioRF24(QObject *parent) : IRadio()
{
    setParent(parent);

    // Set the nodeID to 0 for the master node
    mesh.setNodeID(0);

    // Connect to the mesh
    printf("start\n");
    mesh.begin();
    radio.printDetails();
}

RadioRF24::~RadioRF24() {
    // TODO Auto-generated destructor stub
}

int RadioRF24::check_remotes(void) {
    // Call network.update as usual to keep the network updated
    mesh.update();

    // In addition, keep the 'DHCP service' running on the master node so addresses will
    // be assigned to the sensor nodes
    mesh.DHCP();

    // Check for incoming data from the sensors
    while (network.available()) {
        RF24NetworkHeader header;
        network.peek(header);
        RF24NetworkHeader& header_tonode = *(RF24NetworkHeader*)network.frame_buffer;
        const char *msg = "olaqase";

        uint32_t dat = 0;
        switch (header.type) {
        // Display the incoming millis() values from the sensor nodes
        case 'M':
            network.read(header, &dat, sizeof(dat));
            LOG_INFO(QString("Rcv %1 from 0%2").arg(dat, header.from_node));
            header_tonode.to_node = header.from_node;
            network.write(header_tonode, (const void *)msg, (unsigned short int)strlen(msg));

            emit rxMessage(QString(msg));

            break;
        default:
            network.read(header, 0, 0);
            LOG_WARNING(QString("Rcv bad type %1 from 0%2").arg(header.type,
                    header.from_node));
            break;
        }
    }
    delay(2);
    return 0;
}

bool RadioRF24::dummy()
{
    return true;
}
