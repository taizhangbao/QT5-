#ifndef DESECB_H
#define DESECB_H
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
 * @file DesEcb.h
 * @brief The header file of class DesEcb, it's the interface define of the DesEcb algo.
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
 * @class DesEcb
 * @brief This class defined the interfaces of the DesEcb class
 *
 */
class DesEcb 
{
  
public:

     /**
      * constructor
      */
     DesEcb(const QByteArray& key,unsigned char padding = 0x00);
     
          
    /**
     * the destructor
     */
    ~DesEcb();
    
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
   
   unsigned char mKey[8];
};
          
           
#endif  // DESECB_H
