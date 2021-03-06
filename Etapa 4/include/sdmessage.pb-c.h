/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: sdmessage.proto */

#ifndef PROTOBUF_C_sdmessage_2eproto__INCLUDED
#define PROTOBUF_C_sdmessage_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _MessageT MessageT;
typedef struct _MessageT__StatsT MessageT__StatsT;
typedef struct _MessageT__Entries MessageT__Entries;


/* --- enums --- */

typedef enum _MessageT__Opcode {
  MESSAGE_T__OPCODE__OP_BAD = 0,
  MESSAGE_T__OPCODE__OP_SIZE = 10,
  MESSAGE_T__OPCODE__OP_DEL = 20,
  MESSAGE_T__OPCODE__OP_GET = 30,
  MESSAGE_T__OPCODE__OP_PUT = 40,
  MESSAGE_T__OPCODE__OP_GETKEYS = 50,
  MESSAGE_T__OPCODE__OP_PRINT = 60,
  MESSAGE_T__OPCODE__OP_STATS = 70,
  MESSAGE_T__OPCODE__OP_ERROR = 99,
  MESSAGE_T__OPCODE__OP_ERROR_WRITE = 100
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(MESSAGE_T__OPCODE)
} MessageT__Opcode;
typedef enum _MessageT__CType {
  MESSAGE_T__C_TYPE__CT_BAD = 0,
  MESSAGE_T__C_TYPE__CT_KEY = 10,
  MESSAGE_T__C_TYPE__CT_VALUE = 20,
  MESSAGE_T__C_TYPE__CT_ENTRY = 30,
  MESSAGE_T__C_TYPE__CT_KEYS = 40,
  MESSAGE_T__C_TYPE__CT_RESULT = 50,
  MESSAGE_T__C_TYPE__CT_TABLE = 60,
  MESSAGE_T__C_TYPE__CT_NONE = 70
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(MESSAGE_T__C_TYPE)
} MessageT__CType;

/* --- messages --- */

struct  _MessageT__StatsT
{
  ProtobufCMessage base;
  int32_t nputs;
  int32_t ngets;
  int32_t ndels;
  int32_t nsizes;
  int32_t ngetkeys;
  int32_t ntable_prints;
  double avg_per_operation;
};
#define MESSAGE_T__STATS_T__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&message_t__stats_t__descriptor) \
    , 0, 0, 0, 0, 0, 0, 0 }


struct  _MessageT__Entries
{
  ProtobufCMessage base;
  char *keys;
  ProtobufCBinaryData table_data;
};
#define MESSAGE_T__ENTRIES__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&message_t__entries__descriptor) \
    , (char *)protobuf_c_empty_string, {0,NULL} }


struct  _MessageT
{
  ProtobufCMessage base;
  MessageT__Opcode opcode;
  MessageT__CType c_type;
  ProtobufCBinaryData data;
  int32_t table_size;
  size_t n_keys;
  char **keys;
  size_t n_table_data;
  ProtobufCBinaryData *table_data;
  MessageT__StatsT *stats;
  size_t n_n_entries_lista;
  int32_t *n_entries_lista;
  size_t n_entries;
  MessageT__Entries **entries;
};
#define MESSAGE_T__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&message_t__descriptor) \
    , MESSAGE_T__OPCODE__OP_BAD, MESSAGE_T__C_TYPE__CT_BAD, {0,NULL}, 0, 0,NULL, 0,NULL, NULL, 0,NULL, 0,NULL }


/* MessageT__StatsT methods */
void   message_t__stats_t__init
                     (MessageT__StatsT         *message);
/* MessageT__Entries methods */
void   message_t__entries__init
                     (MessageT__Entries         *message);
/* MessageT methods */
void   message_t__init
                     (MessageT         *message);
size_t message_t__get_packed_size
                     (const MessageT   *message);
size_t message_t__pack
                     (const MessageT   *message,
                      uint8_t             *out);
size_t message_t__pack_to_buffer
                     (const MessageT   *message,
                      ProtobufCBuffer     *buffer);
MessageT *
       message_t__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   message_t__free_unpacked
                     (MessageT *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*MessageT__StatsT_Closure)
                 (const MessageT__StatsT *message,
                  void *closure_data);
typedef void (*MessageT__Entries_Closure)
                 (const MessageT__Entries *message,
                  void *closure_data);
typedef void (*MessageT_Closure)
                 (const MessageT *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor message_t__descriptor;
extern const ProtobufCMessageDescriptor message_t__stats_t__descriptor;
extern const ProtobufCMessageDescriptor message_t__entries__descriptor;
extern const ProtobufCEnumDescriptor    message_t__opcode__descriptor;
extern const ProtobufCEnumDescriptor    message_t__c_type__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_sdmessage_2eproto__INCLUDED */
