/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9 at Mon Mar 12 21:32:06 2018. */

#ifndef PB_REMOTE_DEVS_PB_H_INCLUDED
#define PB_REMOTE_DEVS_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
typedef enum _RadioId {
    RadioId_RID_UNKNOWN = 0,
    RadioId_RID_NRF24 = 1
} RadioId;
#define _RadioId_MIN RadioId_RID_UNKNOWN
#define _RadioId_MAX RadioId_RID_NRF24
#define _RadioId_ARRAYSIZE ((RadioId)(RadioId_RID_NRF24+1))

typedef enum _SensorUnits {
    SensorUnits_SU_UNKNOWN = 0,
    SensorUnits_SU_MAH = 1,
    SensorUnits_SU_CELSIUS_DEGREE = 2,
    SensorUnits_SU_FAHRENHEIT_DEGREE = 3,
    SensorUnits_SU_KELVIN_DEGREE = 4,
    SensorUnits_SU_BOOL = 5,
    SensorUnits_SU_TOTAL_NUMBER = 6
} SensorUnits;
#define _SensorUnits_MIN SensorUnits_SU_UNKNOWN
#define _SensorUnits_MAX SensorUnits_SU_TOTAL_NUMBER
#define _SensorUnits_ARRAYSIZE ((SensorUnits)(SensorUnits_SU_TOTAL_NUMBER+1))

typedef enum _SensorCommandType {
    SensorCommandType_SCT_UNKNOWN = 0,
    SensorCommandType_SCT_TOGGLE = 1,
    SensorCommandType_SCT_TOTAL_NUMBER = 2
} SensorCommandType;
#define _SensorCommandType_MIN SensorCommandType_SCT_UNKNOWN
#define _SensorCommandType_MAX SensorCommandType_SCT_TOTAL_NUMBER
#define _SensorCommandType_ARRAYSIZE ((SensorCommandType)(SensorCommandType_SCT_TOTAL_NUMBER+1))

/* Struct definitions */
typedef struct _RemoteDevData {
    pb_callback_t sendor_data;
    pb_callback_t sensor_command;
/* @@protoc_insertion_point(struct:RemoteDevData) */
} RemoteDevData;

typedef struct _RemoteDevUniqueId {
    RadioId radio_id;
    uint32_t id32;
/* @@protoc_insertion_point(struct:RemoteDevUniqueId) */
} RemoteDevUniqueId;

typedef struct _SensorCommand {
    SensorCommandType command;
    bool has_data;
    uint32_t data;
/* @@protoc_insertion_point(struct:SensorCommand) */
} SensorCommand;

typedef struct _SensorData {
    SensorUnits unit;
    uint32_t value;
/* @@protoc_insertion_point(struct:SensorData) */
} SensorData;

typedef struct _RemoteDevHeader {
    uint32_t transaction_id;
    RemoteDevUniqueId unique_id;
/* @@protoc_insertion_point(struct:RemoteDevHeader) */
} RemoteDevHeader;

typedef struct _RemoteDevMessage {
    RemoteDevHeader header;
    pb_callback_t data;
/* @@protoc_insertion_point(struct:RemoteDevMessage) */
} RemoteDevMessage;

/* Default values for struct fields */

/* Initializer values for message structs */
#define RemoteDevUniqueId_init_default           {(RadioId)0, 0}
#define RemoteDevHeader_init_default             {0, RemoteDevUniqueId_init_default}
#define SensorData_init_default                  {(SensorUnits)0, 0}
#define SensorCommand_init_default               {(SensorCommandType)0, false, 0}
#define RemoteDevData_init_default               {{{NULL}, NULL}, {{NULL}, NULL}}
#define RemoteDevMessage_init_default            {RemoteDevHeader_init_default, {{NULL}, NULL}}
#define RemoteDevUniqueId_init_zero              {(RadioId)0, 0}
#define RemoteDevHeader_init_zero                {0, RemoteDevUniqueId_init_zero}
#define SensorData_init_zero                     {(SensorUnits)0, 0}
#define SensorCommand_init_zero                  {(SensorCommandType)0, false, 0}
#define RemoteDevData_init_zero                  {{{NULL}, NULL}, {{NULL}, NULL}}
#define RemoteDevMessage_init_zero               {RemoteDevHeader_init_zero, {{NULL}, NULL}}

/* Field tags (for use in manual encoding/decoding) */
#define RemoteDevData_sendor_data_tag            1
#define RemoteDevData_sensor_command_tag         2
#define RemoteDevUniqueId_radio_id_tag           1
#define RemoteDevUniqueId_id32_tag               2
#define SensorCommand_command_tag                1
#define SensorCommand_data_tag                   2
#define SensorData_unit_tag                      1
#define SensorData_value_tag                     2
#define RemoteDevHeader_transaction_id_tag       1
#define RemoteDevHeader_unique_id_tag            2
#define RemoteDevMessage_header_tag              1
#define RemoteDevMessage_data_tag                2

/* Struct field encoding specification for nanopb */
extern const pb_field_t RemoteDevUniqueId_fields[3];
extern const pb_field_t RemoteDevHeader_fields[3];
extern const pb_field_t SensorData_fields[3];
extern const pb_field_t SensorCommand_fields[3];
extern const pb_field_t RemoteDevData_fields[3];
extern const pb_field_t RemoteDevMessage_fields[3];

/* Maximum encoded size of messages (where known) */
#define RemoteDevUniqueId_size                   8
#define RemoteDevHeader_size                     16
#define SensorData_size                          8
#define SensorCommand_size                       8
/* RemoteDevData_size depends on runtime parameters */
/* RemoteDevMessage_size depends on runtime parameters */

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define REMOTE_DEVS_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif