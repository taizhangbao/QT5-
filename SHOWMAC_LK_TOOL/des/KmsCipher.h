//------------------------------------------------------------------------
/* (c) Copyright Showmac 2018, All Rights Reserved
*
* Revision History:
*
* Developer                   Date            Version      Description of Changes
* -------------------------   ------------    ----------   ----------------------------------------
* taizhangbao                 2018/07/20      1.0.1          create this file
*--------------------------------------------------------------------------------------------------
*/
//      Description: include the type definiton for public using
//------------------------------------------------------------------------

#ifndef _KMSCIPHER_H_
#define _KMSCIPHER_H_


#define OK 0x0
#define ERR 0x1

#define SCMAC_CONSTANT 0x0101      // Constant data used to generate C-MAC session key
#define SRMAC_CONSTANT 0x0201      // Constant data used to generate R-MAC session key
#define SENC_CONSTANT 0x8201       // Constant data used to generate S-ENC session key
#define DEK_CONSTANT 0x8101        // Constant data used to generate DEK session key
#define SEQUENCE_COUNTER_INDEX 12  // The sequence counter address in response data
#define CRYPTOGRAM_INDEX 20        // The cryptogram address in respose data

#define SESSION_KEY_TYPE 1
#define CMAC_KEY_TYPE    2
#define RMAC_KEY_TYPE    3
#define DEK_KEY_TYPE     4

#define NOT_SET_SECURE   0
#define SET_SECURE       1
#endif //_KMSCIPHER_H_


class KmsCipher
{
public:
    KmsCipher();
    
    //---------------------------------------------------------------------
    // unsigned char LoadKey(unsigned char* pKeyA, unsigned char* pKeyB, unsigned char* pKeyC)
    //
    // Input parameters: 
    //					pKeyA: session based key, used to initiation and authentication
    //                  		pKeyB: C-MAC based key, used to create C-MAC
    //					pKeyC: DEK based key, used to data field encryption if secure 3 is required
    // Output parameters: 
    //					OK(0): success
    //                  ERR(1): the pointers inputted are NULL
    //
    // Description: Load the three sets of key for the subsequent calculation.
    //---------------------------------------------------------------------
    unsigned char LoadKey(unsigned char* pKeyA, unsigned char* pKeyB, unsigned char* pKeyC);
    
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
    unsigned char LoadHostChanllenge(unsigned char* pHostChanllenge);
    
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
    unsigned char VerifyCardCrypto(unsigned char* pAPDUResponse);
    
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
    unsigned char PreSendEXTERNALCommand(unsigned char* pAPDUCommand, unsigned char* pAPDUResponse);
    
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
    //              
    //---------------------------------------------------------------------
    unsigned char PreSendForSecure3(unsigned char* pAPDUCommand, unsigned short bSequenceCounter);
    

        
    QByteArray getEKKey()
    {
        return QByteArray((const char*)gDEKKey,16);
    }

private:
        
    unsigned char gKeyA[16] ;            //session based key, used to initiation and authentication
    unsigned char gKeyB[16];            //C-MAC based key, used to create C-MAC
    unsigned char gKeyC[16] ;            //DEK based key, used to data field encryption if secure 3 is required
    unsigned char gHostChanllenge[8] ; // Host's chanllenge
    unsigned char gSessionKey[16] ;    // Session key, used to authenticate
    unsigned char gCMACKey[16] ;       // C-MAC key, used to C-MAC
    unsigned char gRMACKey[16] ;       // R-MAC key, used to R-MAC
    unsigned char gDEKKey[16] ;        // Data encrypt key, used to data field encryption/decryption
    unsigned char gCryptogram[8];
    unsigned char gCMAC[8] ;
    unsigned char gRMAC[8] ;
    unsigned char gSecureSetFlag ;         // Indicate the security level is or not set to 3
    
    // internal subroutine interfaces
    unsigned char GenerateSessionKey(unsigned char* pDerivaData, unsigned char* pStaticKey, unsigned char bType);
    unsigned char CalcCARDCryptogram(unsigned char* pDerivaData);
    unsigned char CalcCMAC(unsigned char* pData);
    unsigned char CalcDataCMAC(unsigned char* pData, unsigned char bByteNum);
    unsigned char DataEncrypt(unsigned char* pAPDUCommand);
        
    
};
