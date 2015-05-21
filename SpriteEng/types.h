#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdio.h>
#include <stdlib.h>
#include <new.h>

#define WINDOWS_FAMILY
//#define UNIX_FAMILY
//#define MOBILE_DEVICE
//#define COCOS_2D_


typedef __int8 I8;
typedef unsigned __int8 UI8;
typedef __int16 I16;
typedef unsigned __int16 UI16;
typedef __int32 I32;
typedef unsigned __int32 UI32;
typedef __int64 I64;
typedef unsigned __int64 UI64;
typedef unsigned long ULONG;
typedef char CHAR_;
typedef float F32;
typedef double F64;
typedef UI8 PIXCOMP;
typedef UI8 FBYTE;
typedef unsigned long PRIM_ID;
typedef unsigned int ShaderID;
typedef unsigned int TextureID;
typedef UI32 Index;


#define ARRAY_SIZE( arr ) (sizeof( arr )/sizeof( arr[0] ))
#define FIELD_OFFSET_( struct_, field_ ) ULONG(&(((struct_ *)0)->field_))

#define FMalloc( iSize ) malloc( iSize )
#define FFree( lpMem ) free( lpMem )

#define va_parg( ap, t )    ( *(t *)((ap -= _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_pstart( ap, v ) ( ap = (va_list)_ADDRESSOF(v) )


struct RGB
{
	PIXCOMP r, g, b;
};

struct RGBA
{
	PIXCOMP r, g, b, a;
};

#define DWORD_ALIG( w ) ((w * 24 +31) & ~31) /8

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define TOUCHARA( cChar ) *((UI8 *)&cChar)
#define TOCHARA( cChar ) *((CHAR_ *)&cChar)

#define MAX_HASH 256
#define MAX_BUFFER 256

///FString настройки
#define STR_ALLOC_SIZE (1024*1024)//память для строковых переменных
#define STR_NODE_COUNT 10//кол-во делений строк по длине, для упрощения поиска свободных блоков
#define STR_NODE_STEP 15//шаг деления по длине
#define STR_LEN_DEV 10//макс отклонение по длине от искомой, т.е. если в списке свободных есть строка длине искомой на STR_LEN_DEV то свободная строка не используется
#define STR_MAX_HASH 1024
#define STR_NODE_HASH_COUNT 16384//максимальное число одновременно выделеных строк в программе
#define STR_ALLOCATED_GLOBAL 1//использовать глобальное выделение памяти, в случае если память закончилась
#define STR_OUT_MEM_MESS 2//если закончится память то строковая переменная будет равна "out of mem"

#ifdef WINDOWS_FAMILY
#define lpEndL "\r\n"
#elif UNIX_FAMILY
#define lpEndL "\n"
#endif


///Стековые и круговые константы
#define MAX_STACK_ELEMENTS 1024
#define MAX_STACK_SIZE (2048*1024)
#define MAX_LOOPALLOC_SIZE (4096*1024)

//
#define OBJ_NODE_COUNT 16384

struct FRect
{
	F32 fLeft, fRight;
	F32 fBottom, fTop;

	FRect() : fLeft( 0.f ), fRight( 0.f ), fTop( 0.f ), fBottom( 0.f ){}
	FRect( F32 fLeft, F32 fRight, F32 fBottom, F32 fTop ) : fLeft( fLeft ), fRight( fRight ), fTop( fTop ), fBottom( fBottom ){}
};

#define MAX_ALPHABIT 256

#endif