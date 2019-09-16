/*--------------------------------------------------------------------------------------------------
 *                                                                               
 *   Module Name: DesCbc.cpp
 *
 *   The body file of the DesCbc
 *
 *--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------
 * (c) Copyright Motorola 2007, All Rights Reserved
 *
 * Revision History:
 *
 * Developer                   Date            Version      DesCbccription of Changes
 * -------------------------   ------------    ----------   ----------------------------------------
 * Brian Li                    2007/01/26      0.1          create this file
 *--------------------------------------------------------------------------------------------------*/
 
//
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------
//#include <openssl\des.h>

#include "DesCbc.h"
#include "DesAlgo.h"
#include "DeaFunc.h"

//--------------------------------------------------------------------------------------------------
//                                     CLASS IMPLEMENTATION
//--------------------------------------------------------------------------------------------------


/**
 * Constructor
 */
DesCbc::DesCbc(const QByteArray& key,const QByteArray& icv,unsigned char padding):mIcv(icv),mPadding(padding)
{
    for(int i=0; i< key.length(); i++)
    {
        if(i<8)
        {
            mKey[i] = key.at(i);
        }
        
    }
        
}

DesCbc::~DesCbc()
{
}


/**
 * Dec
 */
QByteArray DesCbc::doDec(const QByteArray& src)
{
	if(src.length() % 8)
	{
		return QByteArray();
	}
	
	unsigned char icv[8]={0};
	
	for(int i=0;i<8&&i<mIcv.length();i++)
	{
		icv[i] = mIcv.at(i);
	}
	int len = src.length();
	unsigned char* input = new unsigned char[len];
	for(int i=0; i<src.length(); i++)
	{
		input[i] = src.at(i);
	}
	
	DES_key_schedule ks;
	DES_key_sched( (const_DES_cblock*)mKey,&ks);
	DES_cbc_encrypt(input ,input ,len, &ks,(DES_cblock*)icv,0);
	
        DesDecrypt(mKey,input);
	QByteArray rt = QByteArray((const char*)input,len);
	delete input;
	return rt;	     
 
}    

/**
 * Enc
 */
QByteArray DesCbc::doEnc(const QByteArray& src)
{
	unsigned char icv[8]={0};
	
	for(int i=0;i<8&&i<mIcv.length();i++)
	{
		icv[i] = mIcv.at(i);
	}
	int len = src.length();
	if(len%8)
	{
	    len += ( 8-(len%8) );
	}
	unsigned char* input = new unsigned char[len];
	for(int i=0; i<src.length(); i++)
	{
		input[i] = src.at(i);
	}
	for(int i=src.length();i<len;i++)
	{
		input[i] = mPadding;
	}
		
	DES_key_schedule ks;
	DES_key_sched( (const_DES_cblock*)mKey,&ks);
	DES_cbc_encrypt(input ,input ,len, &ks,(DES_cblock*)icv,1);
	
	QByteArray rt = QByteArray((const char*)input,len);
	delete input;
	return rt;	     
 
 
}
