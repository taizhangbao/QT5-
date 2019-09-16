/************************************************************************
*
*  Module name         : DeaFunc.cpp
*
*  Module description  :
*     This module contains functions for DES encryption/decryption.
*
*  Project             : Generic data encryption.
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
#include <memory.h>
#include <stdlib.h>
#include "DesAlgo.h"
#include "DeaFunc.h"

/************************************************************************
*  Function name   : DDES
*  Description     : Double length key DES encryption/decryption
*                  :
*  Parameters      :
*  Returns         :
*  Author          : Richard Shen
* ----------------------------------------------------------------------
*  Date     By       Description
* ----------------------------------------------------------------------
*  07Jan99  RCS      Created.
************************************************************************/
void DDES(unsigned char *key, unsigned char *data, bool encrypt)
{
   if (encrypt)
   {
      DesEncrypt(key,          data);
      DesDecrypt(key + SINGLE, data);
      DesEncrypt(key,          data);
   } /* end of if */
   else
   {
      DesDecrypt(key,          data);
      DesEncrypt(key + SINGLE, data);
      DesDecrypt(key,          data);
   } /* end of else */

   return;
} // DDES()

/************************************************************************
*  Function name   : OWF/DOWF
*  Description     : One Way Function. Single/Double key length.
*                  :
*  Parameters      : dest  - Buffer to store the result.
*                  : key   - Key.
*                  : src   - Source data.
*  Returns         :
*  Author          : Richard Shen
* ----------------------------------------------------------------------
*  Date     By       Description
* ----------------------------------------------------------------------
*  07Jan99  RCS      Created.
************************************************************************/
uchar *OWF(uchar *dest, uchar *key, uchar *src)
{
   memcpy(dest, src, SINGLE);       // Initialise destination
   DesDecrypt(key, dest);           // Descrypt it
   Mod2(dest, src, SINGLE);

   return(dest);
} // OWF()

uchar *DOWF(uchar *dest, uchar *key, uchar *src)
{
   memcpy(dest, src, SINGLE);       // Initialise destination
   DDES(key, dest, false);          // Decrypt it
   Mod2(dest, src, SINGLE);

   return(dest);
} // DOWF()

/************************************************************************
*  Function name   : Mod2
*  Description     : Modulo 2.
*                  :
*  Parameters      :
*  Returns         :
*  Author          : Richard Shen
* ----------------------------------------------------------------------
*  Date     By       Description
* ----------------------------------------------------------------------
*  07Jan99  RCS      Created.
************************************************************************/
void Mod2(uchar *first, uchar *second, int length)
{
   while (--length >= 0)
      *(first + length) ^= *(second + length);

   return;
} // Mod2()

uchar *MAB(uchar *dest, uchar *mabKey, uchar *src, int length)
{
   if (length <= 0)
      return(NULL);

   memset(dest, 0, SINGLE);
   return(MAB_Keep(dest, mabKey, src, length));
} // MAB()

uchar *MAB_Keep(uchar *dest, uchar *mabKey, uchar *src, int length)
{
   int      i;
   uchar    buf[SINGLE];

   for (i = 0; i * SINGLE < length; i++)
   {
      if ((length - i * SINGLE) >= SINGLE)
      {
         memcpy(buf, src + i * SINGLE, SINGLE);
      } /* end of if */
      else
      {
         memset(buf, 0, SINGLE);
         memcpy(buf, src + i * SINGLE, length - i * SINGLE);
      } /* end of else */

      Mod2(dest, buf, SINGLE);
      DesEncrypt(mabKey, dest);
   } /* end of for */

   return(dest);
} // MAB_Keep()

uchar *ECBC(uchar *cbcData, uint cbcDataLen, uchar *cbcKey, uchar *iv)
{
   uint     len;

   if (cbcDataLen >=  SINGLE)
   {
      Mod2(cbcData, iv, SINGLE);
      len = 0;
      while (1)
      {
         DesEncrypt(cbcKey, (unsigned char *)(cbcData + len));
         len += SINGLE;
         if ((len + SINGLE) > cbcDataLen)
         {
            memcpy(iv, cbcData + len - 8, 8);
            break;
         } /* end of if */

         Mod2(cbcData + len, cbcData + len - SINGLE, SINGLE);
      } /* end of while */
   } /* end of if */

   return(cbcData);
} // ECBC()

uchar *DCBC(uchar *cbcData, uint cbcDataLen, uchar *cbcKey, uchar *iv)
{
   uint     len = 0;
   uchar    tmpIV[SINGLE];

   if (cbcDataLen >=  SINGLE)
   {
      while (1)
      {
         memcpy(tmpIV, cbcData + len, SINGLE);
         DesDecrypt(cbcKey, cbcData + len);
         Mod2(cbcData + len, iv, SINGLE);
         memcpy(iv, tmpIV, SINGLE);
         len += SINGLE;
         if (len >= cbcDataLen)
            break;
      } /* end of while */
   } /* end of if */

   return(cbcData);
} // DCBC()

uchar *OFB(uchar *ofbData, uint ofbDataLen, uchar *ofbKey, uchar *iv)
{
   uint     len = 0;

   if (ofbDataLen >=  SINGLE)
   {
      while (len < ofbDataLen)
      {
         DesEncrypt(ofbKey, iv);
         Mod2(ofbData + len, iv, SINGLE);
         len += SINGLE;
      } /* end of while */
   } /* end of if */

   return(ofbData);
} // OFB()

static bool    firstTime = true;

void GenKey(uchar *key, int length, bool odd)
{
   int      i;

   if (firstTime)
   {
       firstTime = false;
   } /* end of if */

   for (i = 0; i < length; i++)
      *(key + i) = (uchar )(1 * 16 + 1);

   AdjustParity(key, length, odd);
} // Genkey()

void AdjustParity(uchar *data, int length, bool odd)
{
   uchar    sum;
   uchar    *chp;

   for (chp = data; (int )(chp - data) < length; chp++)
   {
      /* Clear bit 0 */
      *chp &= 0xfe;

      /* Calculate parity of bits 7-1 */
      sum = (((*chp) >> 7) & 0x01) ^
            (((*chp) >> 6) & 0x01) ^
            (((*chp) >> 5) & 0x01) ^
            (((*chp) >> 4) & 0x01) ^
            (((*chp) >> 3) & 0x01) ^
            (((*chp) >> 2) & 0x01) ^
            (((*chp) >> 1) & 0x01);

      /* Set bit 0 if required */
      if( (odd && sum == 0x00) || (!odd && sum == 0x01))
          *chp |= 0x01;
   } /* end of for */
} // AdjustParity()

uchar *KVC(uchar *kvcData, uchar *desKey)
{
   uchar    tmpBuf[SINGLE];

   memset(tmpBuf, 0, SINGLE);       // Initialise data
   DesEncrypt(desKey, tmpBuf);

   // Only bit 1 to 24 (ie. 3 bytes) of KVC is used, bit 25 to 64 are discarded.
   memcpy(kvcData, tmpBuf, 3);
   return(kvcData);
} // KVC()

uchar *DKVC(uchar *kvcData, uchar *desKey)
{
   uchar    tmpBuf[SINGLE];

   memset(tmpBuf, 0, SINGLE);             // Initialise data
   DDES(desKey, tmpBuf, true);            // Encrypt it

   // Only bit 1 to 24 (ie. 3 bytes) of KVC is used, bit 25 to 64 are discarded.
   memcpy(kvcData, tmpBuf, 3);
   return(kvcData);
} // DKVC()

uchar LeftShift(uchar *data, int length, uchar fill)
{
   uchar    rem1, rem2;

   rem1 = fill;
   for (int i = length - 1; i >= 0; i--)
   {
      rem2        = *(data + i) >> 7;
      *(data + i) = (*(data + i) << 1) | rem1;
      rem1        = rem2;
   } /* end of for */

   return(rem1);
} /* LeftShift() */
