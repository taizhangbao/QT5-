/************************************************************************
*
*  Module name         : DeaFunc.h
*
*  Module description  :
*
*  Project             :
*
*  Target platform     :
*
*  Compiler & Library  :
*
*  Author              : Richard Shen
*
*  Creation date       : 07 January, 1999
*
************************************************************************/
#ifndef DEAFUNC_H
#define DEAFUNC_H

#include "xtypes.h"
#include "DesAlgo.h"

#define SINGLE          8
#define DOUBLE          16

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void     DDES(unsigned char *key, unsigned char *data, bool encrypt);
uchar    *OWF(uchar *dest,  uchar *key, uchar *src);
uchar    *DOWF(uchar *dest, uchar *key, uchar *src);
void     Mod2(uchar *first, uchar *second, int length);
uchar    *MAB(uchar *dest, uchar *mabKey, uchar *src, int length);
uchar    *MAB_Keep(uchar *dest, uchar *mabKey, uchar *src, int length);
uchar    *ECBC(uchar *cbcData, uint cbcDataLen, uchar *cbcKey, uchar *iv);
uchar    *DCBC(uchar *cbcData, uint cbcDataLen, uchar *cbcKey, uchar *iv);
uchar    *OFB(uchar *ofbData, uint ofbDataLen, uchar *ofbKey, uchar *iv);
void     GenKey(uchar *key, int length, bool odd);
void     AdjustParity(uchar *data, int length, bool odd);
uchar    *KVC(uchar *kvcData, uchar *desKey);
uchar    *DKVC(uchar *kvcData, uchar *desKey);
uchar    LeftShift(uchar *data, int length, uchar fill);

#ifdef __cplusplus
};
#endif /* __cplusplus */


#endif /* DEAFUNC_H */
