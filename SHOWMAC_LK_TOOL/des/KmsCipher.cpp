//------------------------------------------------------------------------
//      Copyright (c) 2007, TAISYS Technologies Co., Ltd
//      All rights reserved.
//------------------------------------------------------------------------
//      File: Nvm.c
//------------------------------------------------------------------------
//      Update History:                            
//      2007/10/23 - V1.00 - MatchDuan :  First official release
//
//------------------------------------------------------------------------
//      Description: KMS Subroutines for SCP02 
//------------------------------------------------------------------------
#include <stddef.h>

#include <cstring>

#include <QtCore>

#include "KmsCipher.h"

#include "DeaFunc.h"
#include "DesAlgo.h"
#include "Xtypes.h"


KmsCipher::KmsCipher()
{
    
    memset(gKeyA,0,16); 
    memset(gKeyB,0,16);
    memset(gKeyC,0,16);
    memset(gHostChanllenge,0,8);
    memset(gSessionKey,0,16);
    memset(gCMACKey,0,16);
    memset(gRMACKey,0,16);
    memset(gDEKKey,0,16);
    memset(gCryptogram,0,8);
    memset(gCMAC,0,8);
    memset(gRMAC,0,8);
    
    gSecureSetFlag = NOT_SET_SECURE;         // Indicate the security level is or not set to 3
}
    
//---------------------------------------------------------------------
// unsigned char LoadKey(unsigned char* pKeyA, unsigned char* pKeyB, unsigned char* pKeyC)
//
// Input parameters: 
//					pKeyA: the first set of key
//                  pKeyB: the second set of key
//					pKeyC: the third set of key
// Output parameters: 
//					OK(0): success
//                  ERR(1): the pointers inputted are NULL
//
// Description: Load the three sets of key for the subsequent calculation.
//---------------------------------------------------------------------
unsigned char KmsCipher::LoadKey(unsigned char* pKeyA, unsigned char* pKeyB, unsigned char* pKeyC)
{
	unsigned char i;

	if ((NULL == pKeyA) || (NULL == pKeyB) || (NULL == pKeyC)) //The inputted pointer should not be NULL
	{
		return ERR;
	}

	for (i = 0; i < 16; i++) //Copy all of the key to dedicated buffer
	{
		gKeyA[i] = pKeyA[i];
		gKeyB[i] = pKeyB[i];
		gKeyC[i] = pKeyC[i];
	}

	return OK;
}

//---------------------------------------------------------------------
// unsigned char LoadHostChanllenge(unsigned char* pHostChanllenge)
//
// Input parameters: 
//					pHostChanllenge: the HostChanllenge of host
//
// Output parameters: 
//					OK(0): success
//                  ERR(1): the pointers inputted are NULL
//
// Description: Load the host chanllenge for the subsequent calculation.
//---------------------------------------------------------------------
unsigned char KmsCipher::LoadHostChanllenge(unsigned char* pHostChanllenge)
{
	unsigned char i;

	if (NULL == pHostChanllenge) //The inputted pointer should not be NULL
	{
		return ERR;
	}

	for (i = 0; i < 8; i++) //Copy all of the key to dedicated buffer
	{
		gHostChanllenge[i] = pHostChanllenge[i];
	}

	return OK;
}

//---------------------------------------------------------------------
// unsigned char VerifyCardCrypto(unsigned char* pAPDUResponse)
//
// Input parameters: 
//					pAPDUResponse: The response of INITIALIZE UPDATE Command recieved from CARD
//                  Note: The APDU reseponse here don't include the status 
//                        words, i.e 0x9000
// Output parameters: 
//					OK(0): verify the CARD cryptogram success
//                  ERR(1): verify the CARD cryptogram fail
//             
// Description: Verify the cryptogram recieved from CARD.
//---------------------------------------------------------------------
unsigned char KmsCipher::VerifyCardCrypto(unsigned char* pAPDUResponse)
{
	unsigned char i = 0;
	unsigned char bTempBuff[24] = {0};  // store the derivation data

	if (NULL == pAPDUResponse)
	{
		return ERR;
	}

	// Put constant data and sequence counter to tempbuff
	((unsigned short*)bTempBuff)[0] = SENC_CONSTANT;
	((unsigned short*)bTempBuff)[1] = ((unsigned short*)pAPDUResponse)[SEQUENCE_COUNTER_INDEX/2];

	if (OK != GenerateSessionKey(bTempBuff, gKeyA, SESSION_KEY_TYPE)) // Create session key fail
	{
		return ERR;
	}

	for (i = 0; i < 8; i++)   // Put host chanllenge and card chanllenge to temp buff
	{
		bTempBuff[i] = gHostChanllenge[i];  
		bTempBuff[8+i] = pAPDUResponse[12+i];
		bTempBuff[16+i] = 0;  // Padding
	}
	bTempBuff[16] = 0x80;      // Padding: 80 00 00 00 00 00 00 00	

	if (OK != CalcCARDCryptogram(bTempBuff)) // Calculate card cryptogram fail
	{
		return ERR;
	}

	for (i = 0; i < 8; i++)
	{
		if (gCryptogram[i] != pAPDUResponse[CRYPTOGRAM_INDEX+i])
		{
			return ERR;       // Verify fail
		}
	}

	return OK;                // Verify success
}

//---------------------------------------------------------------------
// unsigned char PreSendEXTERNALCommand(unsigned char* pAPDUCommand, unsigned char* pAPDUResponse)
//
// Input parameters: 
//					pAPDUResponse: The response INITIALIZE UPDATE Command recieved from CARD
//                  pAPDUCommand:  The EXTERNAL AUTHENTICATE Command witch would be send to CARD
//                  Note: 1,The APDU reseponse here don't include the status 
//                          words, i.e 0x9000
//                        2,The APDU Command here is a modified APDU command, i.e. 84 82 00 00 00(not include the length of C-MAC)
//                         
// Output parameters: 
//					OK(0): Prepare the EXTERNAL command success
//                  ERR(1): Prepare the EXTERNAL command fail
//             
// Description: Get the host cryptogram and C-MAC, and if ok, the result of 8 bytes cryptogram 
//              and 8 bytes of C-MAC will be append to end of the APDU command as:
//              84 82 00 00 10 XX XX XX XX XX XX XX XX YY YY YY YY YY YY YY YY.
//              XX XX XX XX XX XX XX XX -- HOST cryptogram
//              YY YY YY YY YY YY YY YY -- C-MAC
//              
//        Note: this function can be called only after 
//              calling of the function VerifyCardCrypto()
//---------------------------------------------------------------------
unsigned char KmsCipher::PreSendEXTERNALCommand(unsigned char* pAPDUCommand, unsigned char* pAPDUResponse)
{
	unsigned char i = 0;
	unsigned char bTempBuff[24] = {0};  // store the derivation data

	if ((NULL == pAPDUCommand) || (NULL == pAPDUResponse))
	{
		return ERR;
	}

	pAPDUCommand[4] = 0x10;  // Set the length of data including data field and C-MAC
	
	for (i = 0; i < 8; i++)   // Put card chanllenge and host chanllenge to temp buff
	{
		bTempBuff[i] = pAPDUResponse[12+i];  
		bTempBuff[8+i] = gHostChanllenge[i];
		bTempBuff[16+i] = 0;  // Padding 00 00 00 00 00 00 00 00
	}
	bTempBuff[16] = 0x80;      // Padding: 80	

	if (OK != CalcCARDCryptogram(bTempBuff)) // Calculate card cryptogram fail
	{
		return ERR;
	}

	for (i = 0; i < 8; i++)   // Append HOST cryptogram to EXTERNAL command
	{
		pAPDUCommand[5+i] = gCryptogram[i];
	}
	
	// Put constant data and sequence counter to tempbuff
	((unsigned short*)bTempBuff)[0] = SCMAC_CONSTANT;
	((unsigned short*)bTempBuff)[1] = ((unsigned short*)pAPDUResponse)[SEQUENCE_COUNTER_INDEX/2];
	for (i = 4; i < 16; i++)
	{
		bTempBuff[i] = 0;
	}
	// Derivation data construction over
	
	// Calculate C-MAC session key	
	if (OK != GenerateSessionKey(bTempBuff, gKeyB, CMAC_KEY_TYPE))  
	{
		return ERR;
	}
	//generate DEK session key
	((unsigned short*)bTempBuff)[0] = DEK_CONSTANT;
	if (OK != GenerateSessionKey(bTempBuff, gKeyC, DEK_KEY_TYPE))  
	{
		return ERR;
	}
	// Padding 80 00 00 to APDU Command
	pAPDUCommand[13] = 0x80;
	pAPDUCommand[14] = 0;
	pAPDUCommand[15] = 0;

	// Calculate C-MAC
	if (OK != CalcCMAC(pAPDUCommand))         
	{
		return ERR;
	}

	for (i = 0; i < 8; i++)   // Append HOST C-MAC to EXTERNAL command
	{
		pAPDUCommand[13+i] = gCMAC[i];
	}
	// Prepare the EXTERNAL command over and it can be send to CARD directly

	if (3 == pAPDUCommand[2]) // Required security 3
		gSecureSetFlag = SET_SECURE;
	else
		gSecureSetFlag = NOT_SET_SECURE;

	return OK;
}


//---------------------------------------------------------------------
// unsigned char PreSendForSecure3(unsigned char* pAPDUCommand, unsigned short bSequenceCounter)
//
// Input parameters: 
//					bSequeceCounter: The sequence counter recieved from CARD
//                  pAPDUCommand:    The normal APDU Command witch would be send to CARD
//                  
// Output parameters: 
//					OK(0): Prepare the command with secure 3 success
//                  ERR(1): Prepare the command with secure 3 fail
//             
// Description: prepare the normal command with secure level 3
//---------------------------------------------------------------------
unsigned char KmsCipher::PreSendForSecure3(unsigned char* pAPDUCommand, unsigned short bSequenceCounter)
{
	unsigned char i = 0;
	unsigned char bTempBuff[24] = {0};  // store the derivation data
	
	if (NULL == pAPDUCommand)
	{
		return ERR;
	}

	// Put constant data and sequence counter to tempbuff
	bTempBuff[0] = (SCMAC_CONSTANT/0x100)&0xFF;
	bTempBuff[1] = SCMAC_CONSTANT&0xFF;
	bTempBuff[2] = (bSequenceCounter/0x100)&0xFF;
	bTempBuff[3] = (bSequenceCounter&0x00FF);
	
	for (i = 4; i < 16; i++)
	{
		bTempBuff[i] = 0;
	}
	// Derivation data construction over
	
	// Calculate C-MAC session key	
	if (OK != GenerateSessionKey(bTempBuff, gKeyB, CMAC_KEY_TYPE))  
	{
		return ERR;
	}
	
	i = 5+pAPDUCommand[4];
	// Padding 80 00 00 ... to normal APDU Command
	if (i%8)   // Need to padding 80
	{
		pAPDUCommand[i] = 0x80;
		i += 1;
		while (i%8) // Need to padding 00 ...
		{
			pAPDUCommand[i] = 0x0;
			i += 1;
		}
	}
    else   // Need padding 80 00 00 00 00 00 00 00 when i is multiple of 8
	{
		pAPDUCommand[i] = 0x80;
		i++;
		for (int j = 0; j < 7; j++)
		{
			pAPDUCommand[i++] = 0x00;
		}		
	}
	pAPDUCommand[4] += 8;  // add the length of C-MAC 

	// Calculate C-MAC with normal APDU command (not EXTERNAL APDU COMMAND)
	if (OK != CalcDataCMAC(pAPDUCommand, i))         
	{
		return ERR;
	}

	for (i = 0; i < 8; i++)   // Append HOST C-MAC to APDU command
	{
		pAPDUCommand[5+pAPDUCommand[4]-8+i] = gCMAC[i];
	}
	// Prepare the  command with C-MAC over
	
	if (NOT_SET_SECURE == gSecureSetFlag) // Secure level is not required
	{
		return OK;
	}

	// Calculate DEK session key	
	((unsigned short*)bTempBuff)[0] = (unsigned short)SENC_CONSTANT;
	if (OK != GenerateSessionKey(bTempBuff, gKeyA, SESSION_KEY_TYPE))  
	{
		return ERR;
	}
	
	if (OK != DataEncrypt(pAPDUCommand))  // Data field encrypt fail
	{
		return ERR;
	}
	// The normal APDU command include C-MAC and data field encryption now and can be sent to CARD directly 

	return OK;
}

//---------------------------------------------------------------------
// unsigned char GenerateSessionKey(unsigned char* pDerivaData, unsigned char* pStaticKey, unsigned char bType)
//
// Input parameters: 
//					pDerivaData: The derivation data, used to generate session key
//                  pStaticKey:  The static key, used to generate session key
//                  bType:       The key type
//                               1 - session key
//                               2 - C-MAC key
//                               3 - R-MAC key
//                               4 - DEN key
//                        
// Output parameters: 
//					OK(0):  Generate session key success
//                  ERR(1): Generate session key fail
//             
// Description: Generate session key with static Key.
//---------------------------------------------------------------------
unsigned char KmsCipher::GenerateSessionKey(unsigned char* pDerivaData, unsigned char* pStaticKey, unsigned char bType)
{
	unsigned char i = 0;
	unsigned char bICV[8] = {0};

	if ((NULL == pDerivaData) || (NULL == pStaticKey))
	{
		return ERR;
	}

	for (i = 0; i < 8; i++)
	{
		bICV[i] = pDerivaData[i];
	}

	// Triple DES the first 8 bytes
	DesEncrypt(pStaticKey, bICV);
	DesDecrypt(pStaticKey+8, bICV);
	DesEncrypt(pStaticKey, bICV);
	
	switch(bType)
	{
	case 1:         // Session key
		for (i = 0; i < 8; i++)
		{
			gSessionKey[i] = bICV[i];
			bICV[i] ^= pDerivaData[8+i];
		}
		break;
	case 2:         // C-MAC kdy
		for (i = 0; i < 8; i++)
		{
			gCMACKey[i] = bICV[i];
			bICV[i] ^= pDerivaData[8+i];
		}
		break;
	case 3:         // R-MAC key
		for (i = 0; i < 8; i++)
		{
			gRMACKey[i] = bICV[i];
			bICV[i] ^= pDerivaData[8+i];
		}
		break;
	case 4:         // DEN kdy
		for (i = 0; i < 8; i++)
		{
			gDEKKey[i] = bICV[i];
			bICV[i] ^= pDerivaData[8+i];
		}
		break;
	default :
		return ERR;
	}

	// Triple DES the second 8 bytes
	DesEncrypt(pStaticKey, bICV);
	DesDecrypt(pStaticKey+8, bICV);
	DesEncrypt(pStaticKey, bICV);

	switch(bType)
	{
	case 1:
		for (i = 0; i < 8; i++)
		{
			gSessionKey[8+i] = bICV[i];
		}
		break;
	case 2:
		for (i = 0; i < 8; i++)
		{
			gCMACKey[8+i] = bICV[i];
		}
		break;
	case 3:
		for (i = 0; i < 8; i++)
		{
			gRMACKey[8+i] = bICV[i];
		}
		break;
	case 4:
		for (i = 0; i < 8; i++)
		{
			gDEKKey[8+i] = bICV[i];
		}
		break;
	default :
		return ERR;
	}
	
	return OK;
}

//---------------------------------------------------------------------
// unsigned char CalcCARDCryptogram(unsigned char* pDerivaData)
//
// Input parameters: 
//					pDerivaData:    The derivation data, used to create CARD's cryptogram
//                        
// Output parameters: 
//					OK(0):  Calculate the CARD cryptogram success
//                  ERR(1): Calculate the CARD cryptogram fail
//             
// Description: Calculate the CARD cryptogram with gSessionKey.
//---------------------------------------------------------------------
unsigned char KmsCipher::CalcCARDCryptogram(unsigned char* pDerivaData)
{
	unsigned char i = 0;
	unsigned char bICV[8] = {0};

	if (NULL == pDerivaData)
	{
		return ERR;
	}

	// Triple DES the first 8 bytes
	for (i = 0; i < 8; i++)
	{
		bICV[i] = pDerivaData[i];
	}	
	DesEncrypt(gSessionKey, bICV);
	DesDecrypt(gSessionKey+8, bICV);
	DesEncrypt(gSessionKey, bICV);

	// Triple DES the second 8 bytes
	for (i = 0; i < 8; i++)
	{
		bICV[i] ^= pDerivaData[8+i];
	}
	DesEncrypt(gSessionKey, bICV);
	DesDecrypt(gSessionKey+8, bICV);
	DesEncrypt(gSessionKey, bICV);

	// Triple DES the third 8 bytes
	for (i = 0; i < 8; i++)
	{
		bICV[i] ^= pDerivaData[16+i];
	}
	DesEncrypt(gSessionKey, bICV);
	DesDecrypt(gSessionKey+8, bICV);
	DesEncrypt(gSessionKey, bICV);

	for (i = 0; i < 8; i++)
	{
		gCryptogram[i] = bICV[i];
	}

	return OK;
}

//---------------------------------------------------------------------
// unsigned char CalcCMAC(unsigned char* pData)
//
// Input parameters: 
//					pData:    The original data
//                        
// Output parameters: 
//					OK(0):  Calculate the C-MAC success
//                  ERR(1): Calculate the C-MAC fail
//             
// Description: Calculate the C-MAC with gCMACKey.
//---------------------------------------------------------------------
unsigned char KmsCipher::CalcCMAC(unsigned char* pData)
{
	unsigned char i = 0;
	unsigned char bICV[8] = {0};

	if (NULL == pData)
		return ERR;

	// DES the first 8 bytes
	for (i = 0; i < 8; i++)
	{
		bICV[i] = pData[i];
	}	
	DesEncrypt(gCMACKey, bICV);

	// Triple DES the second 8 bytes
	for (i = 0; i < 8; i++)
	{
		bICV[i] ^= pData[8+i];
	}
	DesEncrypt(gCMACKey, bICV);
	DesDecrypt(gCMACKey+8, bICV);
	DesEncrypt(gCMACKey, bICV);

	for (i = 0; i < 8; i++)
	{
		gCMAC[i] = bICV[i];
	}

	return OK;
}

//---------------------------------------------------------------------
// unsigned char CalcDataCMAC(unsigned char* pData, unsigned char bByteNum)
//
// Input parameters: 
//					pData:    The original data
//                  bByteNum: The bytes num of pData
//                        
// Output parameters: 
//					OK(0):  Calculate the C-MAC success
//                  ERR(1): Calculate the C-MAC fail
//             
// Description: Calculate the C-MAC with gCMACKey.
//---------------------------------------------------------------------
unsigned char KmsCipher::CalcDataCMAC(unsigned char* pData, unsigned char bByteNum)
{
	unsigned char i,j;
	unsigned char bICV[8] = {0};

	if ((NULL == pData) || ((bByteNum % 8) != 0))
	{
		return ERR;
	}

	for (i = 0; i < 8; i++)
	{
		bICV[i] = gCMAC[i];  // Use the previous C-MAC as ICV
	}

	DesEncrypt(gCMACKey, bICV);

	j = 0;
	while (j < (bByteNum/8 - 1))
	{
		for (i = 0; i < 8; i++)
		{
			bICV[i] ^= pData[j*8+i];
		}	

		// DES
		DesEncrypt(gCMACKey, bICV);
		//DesDecrypt(gDEKKey+8, bICV);
		//DesEncrypt(gDEKKey, bICV);

		j++;
	}

	for (i = 0; i < 8; i++)
	{
		bICV[i] ^= pData[j*8+i];
	}	

	// Triple DES
	DesEncrypt(gCMACKey, bICV);
	DesDecrypt(gCMACKey+8, bICV);
	DesEncrypt(gCMACKey, bICV);	

	for (i = 0; i < 8; i++)
	{
		gCMAC[i] = bICV[i];
	}

	return OK;
}

//---------------------------------------------------------------------
// unsigned char DataEncrypt(unsigned char* pAPDUCommand)
//
// Input parameters: 
//					pAPDUCommand: The original data (Normal APDU command)
//                        
// Output parameters: 
//					OK(0):  Encrypt data field success
//                  ERR(1): Encrypt data field fail
//             
// Description: Encrypt data field for a normal APDU command.
//---------------------------------------------------------------------
unsigned char KmsCipher::DataEncrypt(unsigned char* pAPDUCommand)
{
	unsigned char i,j;
	unsigned char bICV[8] = {0};
	unsigned char bBackMAC[8];           // Backup the MAC before encrypt data field

	if (NULL == pAPDUCommand)
	{
		return ERR;
	}

	for (i = 0; i < 8; i++)   // Backup the MAC before encrypt data field
	{
		bBackMAC[i] = pAPDUCommand[5+pAPDUCommand[4]-8+i];
	}

	if ((pAPDUCommand[4]-8)%8) // Need padding 0x80
	{
		pAPDUCommand[5+pAPDUCommand[4]-8] = 0x80;
		pAPDUCommand[4] += 1;
		while ((pAPDUCommand[4]-8)%8) // Need padding 0x0 ...
		{
			pAPDUCommand[5+pAPDUCommand[4]-8] = 0x0;
			pAPDUCommand[4] += 1;
		}
	}
	else   // Need padding 80 00 00 00 00 00 00 00 when P3 is multiple of 8
	{
		pAPDUCommand[5+pAPDUCommand[4]-8] = 0x80;
		
		for (i = 0; i < 7; i++)
		{
			pAPDUCommand[5+pAPDUCommand[4]-7+i] = 0x00;
		}
		pAPDUCommand[4] += 8;
	}

	j = 0;
	while (j < ((pAPDUCommand[4]-8)/8))
	{
		for (i = 0; i < 8; i++)
		{
			bICV[i] ^= pAPDUCommand[5+j*8+i];
		}	

		// Triple DES
		DesEncrypt(gSessionKey, bICV);
		DesDecrypt(gSessionKey+8, bICV);
		DesEncrypt(gSessionKey, bICV);

		for (i = 0; i < 8; i++)
		{
			 pAPDUCommand[5+j*8+i] = bICV[i];			 
		}

		j++;
	}

	for (i = 0; i < 8; i++)
	{
		pAPDUCommand[5+pAPDUCommand[4]-8+i] = bBackMAC[i];
	}

	return OK;
}
