#pragma once

#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <RF24Mesh/RF24Mesh.h>

class RadioRF24 {
private:
public:
    RadioRF24();
    virtual ~RadioRF24();
    int check_remotes(void);
};
