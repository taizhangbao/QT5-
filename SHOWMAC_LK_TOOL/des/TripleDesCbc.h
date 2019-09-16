#ifndef TRIPLEDESCBC_H
#define TRIPLEDESCBC_H
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
 * @file TripleDesCbc.h
 * @brief The header file of class TripleDesCbc, it's the interface define of the TripleDesCbc algo.
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
 * @class TripleDesCbc
 * @brief This class defined the interfaces of the TripleDesCbc class
 *
 */
class TripleDesCbc 
{
  
public:

     /**
      * constructor
      */
     TripleDesCbc(const QByteArray& key,unsigned char padding = 0x00);
     
          
    /**
     * the destructor
     */
    ~TripleDesCbc();
    
    /**
     * do the dec operation
     */
    QByteArray doDec(const QByteArray& src, unsigned char *pIcv);
    
    
    
    /**
     * do the ENC operation
     */
    QByteArray doEnc(const QByteArray& src, unsigned char *pIcv);
    
     

private:

   unsigned char mPadding;
   
   unsigned char mKey1[8];
   unsigned char mKey2[8];
};
          
           
#endif  // TRIPLEDESECB_H
