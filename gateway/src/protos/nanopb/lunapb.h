#ifndef LUNA_PROTOS_NANOPB_LUNAPB_H
#define LUNA_PROTOS_NANOPB_LUNAPB_H

extern "C"
{
#include "pb_encode.h"
#include "pb_decode.h"
#include "remote_devs.pb.h"

typedef struct RepeatedDevData
{
    RemoteDevData *data;
    int num;
} RepeatedDevData;

static bool decode_devdata(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    RepeatedDevData *repeatedData = *((RepeatedDevData**)arg);

    // We could read block-by-block to avoid the large buffer...
    if (stream->bytes_left > sizeof(RemoteDevData) - 1)
        return false;

    if (not pb_decode(stream, RemoteDevData_fields, &repeatedData->data[repeatedData->num]))
        return false;

    repeatedData->num++;

    return true;
}

static bool encode_repeated_devdata(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    RepeatedDevData *repeatedData = *((RepeatedDevData**)arg);

    for (int i = 0; i < repeatedData->num; ++i)
    {
        RemoteDevData *devData = repeatedData->data;

        if (not pb_encode_tag_for_field(stream, field))
            return false;

        if (not pb_encode_submessage(stream, RemoteDevData_fields, &devData[i]))
            return false;
    }
    return true;
}

}
#endif // LUNA_PROTOS_NANOPB_LUNAPB_H
