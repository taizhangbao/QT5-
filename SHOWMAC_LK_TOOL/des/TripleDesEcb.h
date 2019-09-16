#ifndef TRIPLEDESECB_H
#define TRIPLEDESECB_H
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
 * @file TripleDesEcb.h
 * @brief The header file of class TripleDesEcb, it's the interface define of the TripleDesEcb algo.
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
 * @class TripleDesEcb
 * @brief This class defined the interfaces of the TripleDesEcb class
 *
 */
class TripleDesEcb 
{
  
public:

     /**
      * constructor
      */
     TripleDesEcb(const QByteArray& key,unsigned char padding = 0x00);
     
          
    /**
     * the destructor
     */
    ~TripleDesEcb();
    
    /**
     * do the dec operation
     */
    QByteArray doDec(const QByteArray& src); 
    
    
    
    /**
     * do the ENC operation
     */
    QByteArray doEnc(const QByteArray& src);    
    
     

private:

   unsigned char mPadding;
   
   unsigned char mKey1[8];
   unsigned char mKey2[8];
};
          
           
#endif  // TRIPLEDESECB_H
