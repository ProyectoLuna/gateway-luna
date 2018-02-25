#ifndef LUNAPB_H
#define LUNAPB_H

extern "C"
{
#include "pb_encode.h"
#include "pb_decode.h"
#include "remote_devs.pb.h"
}

typedef struct RepeatedSensorData
{
    SensorData *data;
    int num;
} RepeatedSensorData;

typedef struct RepeatedSensorCommand
{
    SensorCommand *data;
    int num;
} RepeatedSensorCommand;

bool decode_sensordata(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    RepeatedSensorData *repeatedData = *((RepeatedSensorData**)arg);

    SensorData sensorData = SensorData_init_zero;

    // We could read block-by-block to avoid the large buffer...
    if (stream->bytes_left > sizeof(SensorData) - 1)
        return false;

    if (not pb_decode(stream, SensorData_fields, &sensorData))
        return false;

    repeatedData->data[repeatedData->num++] = sensorData;

    return true;
}

bool encode_repeated_sensordata(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    RepeatedSensorData *repeatedData = *((RepeatedSensorData**)arg);

    for (int i = 0; i < repeatedData->num; ++i)
    {
        SensorData *sensorData = repeatedData->data;

        if (not pb_encode_tag_for_field(stream, field))
            return false;

        if (not pb_encode_submessage(stream, SensorData_fields, &sensorData[i]))
            return false;
    }
    return true;
}

bool decode_sensorcommand(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    RepeatedSensorCommand *repeatedData = *((RepeatedSensorCommand**)arg);

    SensorCommand sensorCommand = SensorCommand_init_zero;

    // We could read block-by-block to avoid the large buffer...
    if (stream->bytes_left > sizeof(SensorCommand) - 1)
        return false;

    if (not pb_decode(stream, SensorCommand_fields, &sensorCommand))
        return false;

    repeatedData->data[repeatedData->num++] = sensorCommand;

    return true;
}

bool encode_repeated_sensorcommand(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    RepeatedSensorCommand *repeatedData = *((RepeatedSensorCommand**)arg);

    for (int i = 0; i < repeatedData->num; ++i)
    {
        SensorCommand *sensorCommand = repeatedData->data;

        if (not pb_encode_tag_for_field(stream, field))
            return false;

        if (not pb_encode_submessage(stream, SensorCommand_fields, &sensorCommand[i]))
            return false;
    }
    return true;
}

#endif // LUNAPB_H
