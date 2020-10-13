
#ifndef JMLOG_DATATYPE_H
#define JMLOG_DATATYPE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace jm {

enum _DataType {
    CHAR,
    UCHAR,
    WCHAR,
    STR,
    WSTR,
    SHORT,
    USHORT,
    INT,
    UINT,
    LONG,
    ULONG,
    LONGLONG,
    ULONGLONG,
    FLOAT,
    DOUBLE,
    VOID_PTR,
    LAST_TYPE = VOID_PTR
};

enum DataType {
    i8,
    u8,
    i16,
    u16,
    i32,
    u32,
    l32,
    ul32,
    i64,
    u64,
    Bool,
    Char,
    Byte,
    WChar,
    String,
    WString,
    Float,
    Double,
    VoidPtr,
    LastType = VoidPtr
};

} // namespace jm

#endif // JMLOG_DATATYPE_H
