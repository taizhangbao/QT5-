/*--------------------------------------------------------------------------------------------------
 *                                                                               
 *   Module Name: DesEcb.cpp
 *
 *   The body file of the DesEcb
 *
 *--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------
 * (c) Copyright Motorola 2007, All Rights Reserved
 *
 * Revision History:
 *
 * Developer                   Date            Version      DesEcbcription of Changes
 * -------------------------   ------------    ----------   ----------------------------------------
 * Brian Li                    2007/01/26      0.1          create this file
 *--------------------------------------------------------------------------------------------------*/
 
//
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------

#include "DesEcb.h"
#include "DesAlgo.h"
#include "DeaFunc.h"

//--------------------------------------------------------------------------------------------------
//                                     CLASS IMPLEMENTATION
//--------------------------------------------------------------------------------------------------


/**
 * Constructor
 */
DesEcb::DesEcb(const QByteArray& key,unsigned char padding):mPadding(padding)
{
    for(int i=0; i< key.length(); i++)
    {
        if(i<8)
        {
            mKey[i] = key.at(i);
        }
    }
        
}

DesEcb::~DesEcb()
{
}


/**
 * Dec
 */
QByteArray DesEcb::doDec(const QByteArray& src)
{
    unsigned char data[8];
    QByteArray rt;

    int len = src.length();    
    int count = 0;
    while(len/8)
    {
        
        len-=8;
        for(int i=0; i< 8; i++)
        {
            data[i] = src.at(count*8+i);
        }    
        count++;
        DesDecrypt(mKey,data);    
        rt += QByteArray((const char*)data,8);
    }
    if(len%8)
    {
        int i;
        for(i=0; i<len%8; i++)
        {
            data[i] = src.at(count*8 +i);
        }
        for(;i<8;i++)
        {
            data[i] = mPadding;
        }
        DesDecrypt(mKey,data);    
        rt += QByteArray((const char*)data,8);
    }
    return rt;
}    

/**
 * Enc
 */
QByteArray DesEcb::doEnc(const QByteArray& src)
{
    unsigned char data[8];
    QByteArray rt;

    int len = src.length();    
    int count = 0;
    while(len/8)
    {
        
        len-=8;
        for(int i=0; i< 8; i++)
        {
            data[i] = src.at(count*8+i);
        }    
        count++;
        DesEncrypt(mKey,data);    
        rt += QByteArray((const char*)data,8);
    }
    if(len%8)
    {
        int i;
        for(i=0; i<len%8; i++)
        {
            data[i] = src.at(count*8 +i);
        }
        for(;i<8;i++)
        {
            data[i] = mPadding;
        }
        DesEncrypt(mKey,data);    
        rt += QByteArray((const char*)data,8);
    }
    return rt;
    

}
