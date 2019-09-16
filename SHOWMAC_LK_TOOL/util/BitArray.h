#ifndef BitArray_H
#define BitArray_H
/*----------------------------------------------------------------------------
 * (c) Copyright Showmac 2018, All Rights Reserved
 *
 * Revision History:
 *
 * Developer                   Date            Version      Description of Changes
 * -------------------------   ------------    ----------   ----------------------------------------
 * taizhangbao                 2018/07/20      1.0.1          create this file
 *--------------------------------------------------------------------------------------------------*/
 
#ifndef __cplusplus
#error "This is a C++ header file; it requires C++ to compile."
#endif

/**
 * @file BitArray.h
 * @brief Brief description of this header
 *
 * Detailed description if needed
 *
 */

//----------------------------------------------------------------------------
//                               INCLUDE FILES
//----------------------------------------------------------------------------
#include<QtCore>

//----------------------------------------------------------------------------
//                                 CONSTANTS
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//                                   ENUMS
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//                         STRUCTURES AND OTHER TYPEDEFS
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//                              UNION DECLARATION
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//                                 CLASS
//----------------------------------------------------------------------------

class BitArray
{
public:
	
	enum SHIFT_WAY{Left,Right};
	
     BitArray();
     
     BitArray(const QByteArray& d);
     
     void setData(const QByteArray& d);
     
     QByteArray shiftBit(int num, SHIFT_WAY w = Left);
     
     QByteArray data() const;

//private:
	QByteArray toBitArray(const QByteArray& d);
	
	QByteArray toByteArray(const QByteArray& d);
	
	char getByte(const QByteArray& d);
	
private:
	QByteArray mData; 
	
	
};
           
//----------------------------------------------------------------------------
#endif  // BitArray_H


