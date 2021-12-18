#ifndef GX_PB_INTERFACE_H_2012_03_07
#define GX_PB_INTERFACE_H_2012_03_07
#include <windows.h>

//#define PROTOBUF_USE_DLLS
//
#pragma warning(disable:4996)
#pragma warning(disable:4251)

#include <google/protobuf/stubs/common.h>

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>

#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>

#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/compiler/parser.h>

#define	GX_MSG google::protobuf::Message
#pragma warning(default:4996)
#pragma warning(default:4251)

#endif
