/*--------------------------------------------------------------------------------------------------
 *                                                                               
 *   Module Name: Util.cpp
 *
 *   The body file of the Util
 *
 *--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------
 * (c) Copyright Showmac 2018, All Rights Reserved
 *
 * Revision History:
 *
 * Developer                   Date            Version      Description of Changes
 * -------------------------   ------------    ----------   ----------------------------------------
 * taizhangbao                 2018/07/20      1.0.1          create this file
 *--------------------------------------------------------------------------------------------------*/
 
//
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------

#include<QtCore>

//#include <openssl\aes.h>
//#include <openssl\sha.h>

#include "Util.h"
#include "BitArray.h"
#include "WPUtil.h"
#include <QLatin1String>
//--------------------------------------------------------------------------------------------------
//                                     CLASS IMPLEMENTATION
//--------------------------------------------------------------------------------------------------
int Util::mWorkingMode = SIM;
	
int Util::mQueenCardMode = GSM;

QByteArray Util::swap(const QByteArray& d)
{
	QByteArray rt;
	int len = d.length();
	char* c = new char[len];
	const char* pd = d.data();
	for(int i=0;i<len;i++)
	{
		c[i] = pd[len-i-1];
	}
	rt = QByteArray(c,len);
	delete[] c;
        return rt;
        //return d;
}	
QByteArray Util::swapByte(const QByteArray& d)
{
        QByteArray rt;
        bool ok;
        int flag = d.mid(0,1).toHex().toInt(&ok,16);

        if(flag == 0x98)
        {
            return d;
        }

        int len = d.length();
        char* c = new char[len];
        const char* pd = d.data();

        for(int i=0;i<len;i++)
        {
            c[i] = ((pd[i]>>4)&0x0F) | ((pd[i]<<4)&0xF0);
        }
        rt = QByteArray(c,len);
        delete[] c;
        return rt;
        //return d;
}

QByteArray Util::exchangeKiOpc(const QByteArray& pKi, const QByteArray& pOpc)
{
    QByteArray ki;
    QByteArray opc;
    QByteArray kiOpc;

    //qDebug() << "pKi---- " << pKi.toHex().toUpper();
    //qDebug() << "pOpc---- " << pOpc.toHex().toUpper();

    for(int i=1;i<pKi.length();i+=2)
    {
        ki += (pKi.mid(i, 1)+pOpc.mid(i-1, 1));
        opc += (pKi.mid(i-1, 1)+pOpc.mid(i, 1));
    }

    qDebug() << "ki---- " << ki.toHex().toUpper();
    qDebug() << "opc---- " << opc.toHex().toUpper();

    kiOpc = ki + opc;
    return kiOpc;
}

QByteArray Util::reductionKiOpc(const QByteArray& pKi, const QByteArray& pOpc)
{
    QByteArray ki;
    QByteArray opc;
    QByteArray kiOpc;

    //qDebug() << "pKi---- " << pKi.toHex().toUpper();
    //qDebug() << "pOpc---- " << pOpc.toHex().toUpper();

    for(int i=1;i<pKi.length();i+=2)
    {
        ki += (pKi.mid(i, 1)+pOpc.mid(i-1, 1));
        opc += (pKi.mid(i-1, 1)+pOpc.mid(i, 1));
    }

    //qDebug() << "ki---- " << ki.toHex().toUpper();
    //qDebug() << "opc---- " << opc.toHex().toUpper();

    kiOpc = ki + opc;
    return kiOpc;
}

QByteArray Util::delHead_0(const QByteArray& d)
{
    QByteArray s1;
    //QByteArray s;
    int i;

   // s = d;
    for(i=0; i< d.length()-1; i++)
    {
        s1[i] = (d.at(i)<<4) | ((d.at(i+1)>>4)&0x0F);
    }
    s1 += (d.at(i)<<4);
    return s1;
}

QByteArray Util::toUimid(const QByteArray& d)
{
    QByteArray ba;
    ba.resize(1);
    ba[0]=d.length();
    QByteArray uimid = Util::swap(d);
    uimid = ba+uimid;
    uimid = Util::rightPadding(uimid, 8, 0);
    uimid = Util::rightPadding(uimid, 16, 0xFF);
    //qDebug() << "uimid " << uimid.toHex().toUpper();
    return uimid;
}

QByteArray Util::imsimToImsit(const QString& s)
{
    QByteArray rt;
    ulong num;
    QByteArray ba;

    qDebug() << "unsigned long" << sizeof(unsigned long);

    ulong dwResult = 0;
    //qDebug() << "s " << s;
    num = Util::caclCdma(s.left(3));
    //qDebug() << "num =" << num;
    QByteArray mcc = Util::intToByte(num);
    mcc = mcc.left(2);
    //qDebug() << "mcc : " << mcc.toHex().toUpper();

    num = Util::caclCdma(s.mid(3,2));
    //qDebug() << "num =" << num;
    QByteArray mnc = Util::intToByte(num);
    mnc = mnc.left(1);
    ba.resize(1);
    ba[0]=0x80;
    mnc.append(ba);
    //qDebug() << "mnc : " << mnc.toHex().toUpper();

    num = Util::caclCdma(s.mid(5,3));
    //qDebug() << "num =" << num;
    QByteArray aaaa = Util::intToByte(num);
    aaaa = aaaa.left(2);
    //qDebug() << "aaaa : " << aaaa.toHex().toUpper();

    dwResult=0x00;
    num = Util::caclCdma(s.mid(8,3));
    qDebug() << "num1 =" << num;
    dwResult = dwResult+(uchar)(num<<14);
    qDebug() << "dwResult1 =" << dwResult;

    num = s.mid(11,1).toInt();
    qDebug() << "num2 =" << num;
    dwResult = dwResult+(uchar)(num<<10);
    qDebug() << "dwResult2 =" << dwResult;

    num = Util::caclCdma(s.mid(12,3));
    qDebug() << "num3 =" << num;
    dwResult = dwResult+(uchar)(num<<0);
    qDebug() << "dwResult3 =" << dwResult;

    QByteArray bbbb= Util::intToByte(dwResult);
    bbbb = bbbb.left(3);
    qDebug() << "bbbb : " << bbbb.toHex().toUpper();

    ba.resize(1);
    ba[0]=0x0;
    rt = ba+aaaa+bbbb+mnc+mcc;
    rt = Util::rightPadding(rt, 16, 0xFF);
    return rt;
}

void Util::copyByteArray(const QByteArray& src,unsigned char* d)
{
	for(int i=0;i<src.length();i++)
	{
		d[i] = src.at(i);
	}
}

QByteArray  Util::intToByte(ulong i)
{
    QByteArray abyte0;
    abyte0.resize(4);
    abyte0[0] = (uchar)  (0x000000ff & i);
    abyte0[1] = (uchar) ((0x0000ff00 & i) >> 8);
    abyte0[2] = (uchar) ((0x00ff0000 & i) >> 16);
    abyte0[3] = (uchar) ((0xff000000 & i) >> 24);
    return abyte0;
}

int Util::caclCdma(const QString& s)
{
    int i;
    uchar bySub;
    ushort wResult;
    uchar byTemp;

    //qDebug() << "caclCdma  s =" << s;
    switch (s.length())
    {
        case 0x01:
            bySub=0x00;
            break;

        case 0x02:
            bySub=11;
            break;

        case 0x03:
            bySub=111;
            break;
    }

    wResult = 0;
    for(i=0; i<s.length(); i++)
    {
        byTemp = s.mid(i,1).toInt();

        //qDebug() << "byTemp = " << byTemp;
        if (byTemp==0)
        {
                byTemp=0x0A;
        }

        wResult=wResult*0x0A+byTemp;
        //qDebug() << "wResult = " << wResult;
    }
    //qDebug() << "bySub = " << bySub;
    return wResult-bySub;
}

QByteArray Util::toByteArray(const QString& s)
{
	bool ok;
	int v;
	v = s.toLong(&ok); //convert to dec

	if(!ok)
	{
		if(s.left(2).toLower() == "0x") //hex 0x....
		{
		    v= s.right(s.length()-2).toLong(&ok,16);
		}
		else //hex 7Fxx
		{	
		  v=s.toLong(&ok,16);
		}  
	}
	
	if(ok) //hex
	{
		QString str = QString().setNum(v,16);
		if(str.length()%2)
		{
			str = "0" + str;
		}
		return QByteArray::fromHex( str.toLatin1() );	
	}		
	
	if(s.left(1) == "'")
	{
		QString str = s.mid(1,s.length()-1);
		return QByteArray::fromHex(str.toLatin1());
	}
	if(s.left(1) == "\"")
	{
	   QString str = s.mid(1,s.length()-1);
	   return str.toLatin1();
	}
	
	return QByteArray::fromHex(s.toLatin1());
}

QByteArray Util::rightPadding(const QByteArray& d, int len, char pad)
{
	  char c[8] = {pad,pad,pad,pad,pad,pad,pad,pad};
          return (d + QByteArray((const char*)c,len) ).left(len);
}	

QString byte2str( char c)
{
	char cl = ((c&0xF0)>>4) + 0x30;
	char cr = (c&0x0F) + 0x30;
	if(cl == 0x3a) cl = 0x30;
	if(cr == 0x3a) cr = 0x30;
	return QString().append(cl).append(cr);
}

QString Util::cdmaData2TelNo(const QByteArray& d,bool& ok)
{
	BitArray ba(d);
	ok = true;
	QByteArray tpda = ba.shiftBit(2,BitArray::Left);
	tpda = tpda.left(d.length()-1);
	if(tpda.at(0) > (tpda.length()-1)*2)
	{
		ok = false;
	}
	
	QString rt;
	for(int i=1;i<tpda.length(); i++)
	{
		rt += byte2str(tpda.at(i) );
	}
	return rt.left(tpda.at(0));
}	
    
QByteArray Util::cdmaTPUD2Data(const QByteArray& d)
{
	BitArray ba(d);
	QByteArray rt = ba.shiftBit(3,BitArray::Right);
	return rt.left(rt.length()-1);
}

QByteArray Util::cdmaTelno2Tpoa(const QByteArray& d)
{
	BitArray ba(d);
	return ba.shiftBit(2,BitArray::Right);
}
    
QByteArray Util::cdmaData2TPUD(const QByteArray& d)		
{
	BitArray ba(d);
	return ba.shiftBit(3,BitArray::Left);
}	

QByteArray Util::sid2CDMAData(const QByteArray& d)
{
	BitArray ba(d);
	return ba.shiftBit(4,BitArray::Right);
}	


void Util::generateUniqueRandomNumber()
{

    int i,j;

    QList<int> numbersList;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    for(i=0;i<10;i++)

    {

        numbersList.append(qrand()%10);

        bool flag=true;

        while(flag)

        {

            for(j=0;j<i;j++)

            {

                if(numbersList[i]==numbersList[j])

                {

                    break;

                }

            }

            if(j<i)

            {

                numbersList[i]=rand()%10;

            }

            if(j==i)

            {

                flag=!flag;

            }

        }

    }

    for(i=0;i<10;i++)

    {

        qDebug()<<numbersList[i];

    }

}
