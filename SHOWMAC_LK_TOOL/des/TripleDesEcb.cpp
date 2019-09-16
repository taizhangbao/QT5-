/*--------------------------------------------------------------------------------------------------
 *                                                                               
 *   Module Name: TripleDesEcb.cpp
 *
 *   The body file of the TripleDesEcb
 *
 *--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------
 * (c) Copyright Motorola 2007, All Rights Reserved
 *
 * Revision History:
 *
 * Developer                   Date            Version      TripleDesEcbcription of Changes
 * -------------------------   ------------    ----------   ----------------------------------------
 * Brian Li                    2007/01/26      0.1          create this file
 *--------------------------------------------------------------------------------------------------*/
 
//
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------

#include "TripleDesEcb.h"
#include "DesAlgo.h"
#include "DeaFunc.h"

//--------------------------------------------------------------------------------------------------
//                                     CLASS IMPLEMENTATION
//--------------------------------------------------------------------------------------------------


/**
 * Constructor
 */
TripleDesEcb::TripleDesEcb(const QByteArray& key,unsigned char padding):mPadding(padding)
{
    for(int i=0; i< key.length(); i++)
    {
        if(i<8)
        {
            mKey1[i] = key.at(i);
        }
        if(i>=8 && i<16)
        {
            mKey2[i-8] = key.at(i);
        }
    }     
}

TripleDesEcb::~TripleDesEcb()
{
}


/**
 * Dec
 */
QByteArray TripleDesEcb::doDec(const QByteArray& src)
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
        DesDecrypt(mKey1,data);
        DesEncrypt(mKey2,data);
        DesDecrypt(mKey1,data);    
        rt += QByteArray((const char*)data,8);
        qDebug() << "rt : " << rt.toHex();
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
        DesDecrypt(mKey1,data);
        DesEncrypt(mKey2,data);
        DesDecrypt(mKey1,data);    
        rt += QByteArray((const char*)data,8);
    }
    return rt;
}    

/**
 * Enc
 */
QByteArray TripleDesEcb::doEnc(const QByteArray& src)
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
        DesEncrypt(mKey1,data);
        DesDecrypt(mKey2,data);
        DesEncrypt(mKey1,data);    
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
        DesEncrypt(mKey1,data);
        DesDecrypt(mKey2,data);
        DesEncrypt(mKey1,data); 
        rt += QByteArray((const char*)data,8);
    }
    return rt;
}
