#ifndef DESCBC_H
#define DESCBC_H
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
 * @file DesCbc.h
 * @brief The header file of class DesCbc, it's the interface define of the DesCbc algo.
 *
 *
 */

//----------------------------------------------------------------------------
//                               INCLUDE FILES
//----------------------------------------------------------------------------
#include <QtCore>



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
/**
 * @class DesCbc
 * @brief This class defined the interfaces of the DesCbc class
 *
 */
class DesCbc 
{
  
public:

     /**
      * constructor
      */
     DesCbc(const QByteArray& key,const QByteArray& icv = QByteArray::fromHex("0000000000000000"),unsigned char padding = 0x20);
     
          
    /**
     * the destructor
     */
    ~DesCbc();
    
    /**
     * do the dec operation
     */
    QByteArray doDec(const QByteArray& src); 
    
    
    
    /**
     * do the ENC operation
     */
    QByteArray doEnc(const QByteArray& src);    
    
     

private:

   QByteArray mIcv;
   
   unsigned char mPadding;
   
   unsigned char mKey[8];
};
          
           
#endif  // DESECB_H
