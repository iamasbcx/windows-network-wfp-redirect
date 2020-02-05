#pragma once


#include <assert.h>

#if _DEBUG


#define DBG_ASSERT( _X_ ) assert( _X_ )


#else


#define DBG_ASSERT( _X_ )

#endif