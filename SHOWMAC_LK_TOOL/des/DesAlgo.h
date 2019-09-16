/************************************************************************
*
*  Module name         : DesAlgo.h
*
*  Module description  :
*     This header file declares function prototypes for DES encryption
*     functions.
*
*  Project             :
*
*  Target platform     :
*
*  Compiler & Library  :
*
*  Author              : Richard Shen
*
*  Creation date       : 20 January, 1999
*
************************************************************************/
#ifndef DesAlgo_h
#define DesAlgo_h

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void  DesEncrypt(unsigned char *key, unsigned char *data);
void  DesDecrypt(unsigned char *key, unsigned char *data);

#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* DesAlgo_h */
