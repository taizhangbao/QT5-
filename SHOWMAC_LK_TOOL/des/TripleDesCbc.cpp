/*--------------------------------------------------------------------------------------------------
 *                                                                               
 *   Module Name: TripleDesCbc.cpp
 *
 *   The body file of the TripleDesCbc
 *
 *--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------
 * (c) Copyright Motorola 2007, All Rights Reserved
 *
 * Revision History:
 *
 * Developer                   Date            Version      TripleDesCbccription of Changes
 * -------------------------   ------------    ----------   ----------------------------------------
 * Brian Li                    2007/01/26      0.1          create this file
 *--------------------------------------------------------------------------------------------------*/
 
//
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------

#include "TripleDesCbc.h"
#include "DesAlgo.h"
#include "DeaFunc.h"
#include "dialog.h"
//--------------------------------------------------------------------------------------------------
//                                     CLASS IMPLEMENTATION
//--------------------------------------------------------------------------------------------------


/**
 * Constructor
 */
TripleDesCbc::TripleDesCbc(const QByteArray& key,unsigned char padding):mPadding(padding)
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

TripleDesCbc::~TripleDesCbc()
{
}


/**
 * Dec
 */
QByteArray TripleDesCbc::doDec(const QByteArray& src, unsigned char *pIcv)
{
    unsigned char data[8];
    unsigned char icv[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    QByteArray rt;

    if(pIcv != NULL)
    {
        memcpy(icv, pIcv, sizeof(icv));
    }

    int len = src.length();    
    int count = 0;
    while(len/8)
    {
        len-=8;
        for(int i=0; i< 8; i++)
        {
            data[i] = src.at(count*8+i);
        }   
        
        DesDecrypt(mKey1,data);
        DesEncrypt(mKey2,data);
        DesDecrypt(mKey1,data);    
        for(int i=0;i<8;i++)
        {
        	data[i] = data[i]^icv[i];
        	icv[i] = src.at(count*8+i);
        }
        count++;
        
        rt += QByteArray((const char*)data,8);
        
    }
    
    return rt;
}    

/**
 * Enc
 */
QByteArray TripleDesCbc::doEnc(const QByteArray& src, unsigned char *pIcv)
{
    unsigned char data[8];
    unsigned char icv[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    QByteArray rt;
    memcpy(icv, pIcv, sizeof(icv));
    int len = src.length();    
    int count = 0;
    while(len/8)
    {
        
        len-=8;
        for(int i=0; i< 8; i++)
        {
            data[i] = src.at(count*8+i)^icv[i];
        }    
        count++;
        DesEncrypt(mKey1,data);
        DesDecrypt(mKey2,data);
        DesEncrypt(mKey1,data);    
        for(int i=0;i<8;i++)
        {
        	icv[i] = data[i];
        }
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
        for(int i=0; i< 8; i++)
        {
            data[i] = data[i]^icv[i];
        }    
        DesEncrypt(mKey1,data);
        DesDecrypt(mKey2,data);
        DesEncrypt(mKey1,data); 
        rt += QByteArray((const char*)data,8);
    }
    return rt;
}
