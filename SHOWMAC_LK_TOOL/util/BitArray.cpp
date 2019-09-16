/*--------------------------------------------------------------------------------------------------
 *                                                                               
 *   Module Name: BitArray.cpp
 *
 *   The body file of the BitArray
 *
 *--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------
 * (c) Copyright Showmac 2018, All Rights Reserved
 *
 * Revision History:
 *
 * Developer                   Date            Version      Description of Changes
 * -------------------------   ------------    ----------   ----------------------------------------
 * taizhangbao                 2018/07/20      1.0.1          create this file
 *--------------------------------------------------------------------------------------------------*/
 
//
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------

#include<QtCore>

#include <openssl\aes.h>

#include "BitArray.h"

//--------------------------------------------------------------------------------------------------
//                                     CLASS IMPLEMENTATION
//--------------------------------------------------------------------------------------------------
BitArray::BitArray()
{
}
     
BitArray::BitArray(const QByteArray& d):mData(d)
{
}	
     
void BitArray::setData(const QByteArray& d)
{
	mData = d;
}	
     
QByteArray BitArray::shiftBit(int num, SHIFT_WAY w)
{
	char c[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	QByteArray t = toBitArray(mData);
	if(w == Left)
	{
		t = t.right(t.length() - num) + QByteArray(c,8);
	}
	else
	{
		t = QByteArray(num,0) + t + QByteArray(c,8);
	}
	mData = toByteArray(t);
	return mData;
}	
     
QByteArray BitArray::data() const
{
	return mData;
}

QByteArray BitArray::toBitArray(const QByteArray& d)
{
	QByteArray rt;
	const char mask[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x1};
	for(int i=0;i<d.length();i++)
	{
            for(int j=0;j<8;j++)
            {
                rt += d.at(i)&mask[j];
            }
	}
	
	return rt;
}		
	
QByteArray BitArray::toByteArray(const QByteArray& d)
{
	QByteArray rt;
	QByteArray data = d.left(d.length()&0xFFFFF8);
	for(int i=0;i<data.length();i+=8)
	{
		rt += getByte(data.mid(i,8) );
	}
	return rt;
}	

char BitArray::getByte(const QByteArray& d)
{
	char c = 0x00;
	const char mask[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x1};
	for(int i=0;i<8;i++)
	{
		if(d.at(i) )
		{	
		    c += mask[i];
		}    
	}
	return c;
}
	
