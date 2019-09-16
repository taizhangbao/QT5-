#ifndef UTIL_H
#define UTIL_H
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
 * @file Util.h
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

class Util
{
public:
	
	static QByteArray swap(const QByteArray& d);

        static QByteArray swapByte(const QByteArray& d);

	static void copyByteArray(const QByteArray& src,unsigned char* d);

        static QByteArray toByteArray(const QString& s);

        static QByteArray rightPadding(const QByteArray& d, int len = 8, char pad = 0xFF);

        static QString cdmaData2TelNo(const QByteArray& d,bool& ok);

        static QByteArray cdmaTPUD2Data(const QByteArray& d);

        static QByteArray cdmaTelno2Tpoa(const QByteArray& d);

        static QByteArray cdmaData2TPUD(const QByteArray& d);

        static QByteArray sid2CDMAData(const QByteArray& d);

        static QByteArray delHead_0(const QByteArray& d);

        static QByteArray imsimToImsit(const QString& s);

        static QByteArray intToByte(ulong i);

        static int caclCdma(const QString& s);

        static QByteArray toUimid(const QByteArray& d);

        static QByteArray exchangeKiOpc(const QByteArray& pKi, const QByteArray& pOpc);

        static QByteArray reductionKiOpc(const QByteArray& pKi, const QByteArray& pOpc);

        static void generateUniqueRandomNumber();

        static bool isCDMA(){return mQueenCardMode == CDMA;}        

        static int mWorkingMode;
        const static int SIM = 0xA0;
        const static int USIM = 0x80;
        const static int PBOC = 0x00;
    
    
 	static int mQueenCardMode;
 	const static int CDMA = 0x01;
 	const static int GSM = 0x00;
 	const static int G3G = 0x02;
 	
};
           
//----------------------------------------------------------------------------
#endif  // UTIL_H


