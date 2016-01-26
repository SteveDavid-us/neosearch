// *******************************************************************************************
// EndianSwap.h
// -------------------------------------------------------------------------------------------
// NeoSearch 2 -- 2006 -- JPS
// -------------------------------------------------------------------------------------------
// Header file for endian conversion routines
//
// *******************************************************************************************

#ifndef _ENDIANSWAP_H_
#define _ENDIANSWAP_H_


inline void endian_swap(unsigned short& x)
{
    x = (x>>8) | 
        (x<<8);
}

inline void endian_swap(short& x)
{
    endian_swap((unsigned short&)x);
}

inline void endian_swap(unsigned int& x)
{
    x = (x>>24) | 
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);
}

inline void endian_swap(int& x)
{
    endian_swap((unsigned int&)x);
}

#endif // _ENDIANSWAP_H_

