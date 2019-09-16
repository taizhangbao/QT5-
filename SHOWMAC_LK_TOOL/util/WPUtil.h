#ifndef WPUTIL_H
#define WPUTIL_H
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
 * @file WPUtil.h
 * @brief Brief description of this header
 *
 * Detailed description if needed
 *
 */

//----------------------------------------------------------------------------
//                               INCLUDE FILES
//----------------------------------------------------------------------------
#include <qstring.h>
//#include <qdom.h>

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

class WPUtil
{
public:

 
    /**
     * extract the path 
     */
    static QString getPath(const QString& fileName);
    
    /**
     * convert the string to unsigned chars, stored in the array
     * @param str, the string
     * @param bytes, the converted bytes
     */
    static void string2Bytes(const QString& str,unsigned char* bytes);
    
    /**
     * convert the bytes to string
     * @param bytes, the source
     * @param str, the converted string
     * @param len, the length of the source
     */
    static void bytes2String(unsigned char* bytes,QString& str,int len);
    
    /**
     * filt the space character
     */
    static QString filtSpace(const QString& str);
    
    /**
     * insert space between each two characters
     */
    static QString insertSpace(const QString& str);
    
    /**
     * there are three format of string
     * "xxx", xxx is one string
     * 0xxxxx, xxx is hex format
     * xxxxx, xxxx is decimal format
     * convert all the format to hex format
     * @return, the string which is hex format
     */
    static QString conv2Hex(const QString& str);
    
    /**
     * convert to ucs2 format
     * there are three format of the string
     * "xxxx", xxx is on string, converted it to ucs2
     * xxxxx, don't do any change 
     */
    static QString conv2UCS2(const QString& str);
    static QString conv2UCS2ForLittleFormad(const QString& str);
    
    /**
     * get the length of the string, then convert the value
     * to string
     */
    static QString getLen(const QString& str);
    
    /**
     * get the file info
     * @param, the response of the select or status
     */
    static QString getFileInfo(const QString& str); 
    
    /**
     * right padding 
     * @param src, the source of the string
     * @param paddding, the string to be padded
     * @param len, the length after padded.
     * @return the padding string
     */
    static QString rightPadding(const QString& src, const QString& padding, int len);
    
    
    
    
    static QByteArray aesEnc(const QByteArray& key, const QByteArray& d,unsigned char p = 0x20);
	
	  static QByteArray aesDec(const QByteArray& key, const QByteArray& d);
	
	  static QByteArray sha1(const QByteArray& d);
	  
	  static QByteArray md5(const QByteArray& d);
	  
	  static void copyByteArray(const QByteArray& src, unsigned char* dest);
    
    static QByteArray getRandom();

    static char readFlag();
    static void setFlag(char flag);
    
    /**
     * the stk session len
     */
    static unsigned char mStkCmdLen; 
    
    static QString mStkCmd;  
    
    static QString mDefaultV;
    
    static QString mDuration;
    
    //for getinput
    static int mMax;
    static int mMin;
    static int mQualifier;

    static char mdisplayTag;
    
private:
    
    /**
     *Constructor
     */
    WPUtil();
    
    /**
     * Destructor
     */
    ~WPUtil();
     

    static unsigned char a2c(unsigned char a);
    
    static unsigned char c2a(unsigned char c);
    
    static bool isHex(const QString& str);
    
    static bool isStr(const QString& str);
    
    static QString conv2Hex(const QString& str,int);
    
    static QString conv2Hex(const QString& str,char);
   
    class DF{};
    class EF{};
    
    static QString getFileInfo(unsigned char* in, DF);
    static QString getFileInfo(unsigned char* in, EF);
};
           
//----------------------------------------------------------------------------
#endif  // WPUTIL_H


