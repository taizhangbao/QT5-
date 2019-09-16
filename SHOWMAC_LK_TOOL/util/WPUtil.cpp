/*--------------------------------------------------------------------------------------------------
 *                                                                               
 *   Module Name: WPUtil.cpp
 *
 *   The body file of the WPUtil
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
#include <qstring.h>
#include <qfile.h>
#include <QTextStream>
#include <QVariant>
//#include <qdom.h>

#include "WPUtil.h"
//#include "WPDebug.h"

//#include <openssl\aes.h>
//#include <openssl\sha.h>
//#include <openssl\md5.h>
#include <string>
//--------------------------------------------------------------------------------------------------
//                                     CLASS IMPLEMENTATION
//--------------------------------------------------------------------------------------------------
unsigned char  WPUtil::mStkCmdLen = 0;
QString WPUtil::mStkCmd = QString();
QString WPUtil::mDefaultV = QString();
QString WPUtil::mDuration = QString();
int WPUtil::mMax = 0;
int WPUtil::mMin = 0;
int WPUtil::mQualifier = 0;
char WPUtil::mdisplayTag = 0;
	
/**
 *Constructor
 */
WPUtil::WPUtil() 
{
}



/**
 * Destructor
 */
WPUtil::~WPUtil()
{
}


/**
 * get the path
 */
QString WPUtil::getPath(const QString& fileName)
{
    QString path = fileName;
    int index = path.lastIndexOf("/");
    if(index>0)
    {
        return path.left(index);
    }
    return path;
    
}

void WPUtil::string2Bytes(const QString& str,unsigned char* bytes)
{
    int index =0;
    QString src = str.toUpper();
    for(int i=1; i<src.length(); i+=2)
    {
        unsigned char left = a2c( (unsigned char)(src.at(i-1).cell() ) );
        unsigned char right= a2c( (unsigned char)(src.at(i).cell() ) );
        
        bytes[index] = ((left<<4)&0xF0) + (right&0x0F) ;
        index++;
    }
}


/**
 * convert the bytes to string
 * @param bytes, the source
 * @param str, the converted string
 * @param len, the length of the source
 */
void WPUtil::bytes2String(unsigned char* bytes,QString& str,int len)
{
    QString rt;
    for(int i=0; i<len; i++)
    {
        char l = (bytes[i]>>4)&0x0F;
        char r= bytes[i]&0x0F;
        QChar c[2] = { c2a(l),c2a(r)};
        rt += QString(c,2);
    }
    str = rt.toUpper();
}

QString WPUtil::filtSpace(const QString& str)
{
    QString rt;
    for(int i=0; i<str.length(); i++)
    {
        QChar c = str.at(i);
        if(c != ' ')
        {
            rt += c;
        }
    }
    return rt;
}

QString WPUtil::insertSpace(const QString& str)
{
    QString rt;
    for(int i=0; i<str.length(); i+=2)
    {
        rt += str.at(i);
        rt += str.at(i+1);
        rt += ' ';
    }
    return rt;
}

    
QString WPUtil::conv2Hex(const QString& str)
{
    if(isHex(str))
    {
        //remove "0x"
        QString s = str.right(str.length()-2);
        if(s.length()%2)
        {
            s = "0" + s;
        }
        return s;
    }
    else if(isStr(str))
    {
    	  
        return conv2Hex(str,'c');
    }
    
    //Dec format
    return conv2Hex(str,1);
    
}

QString WPUtil::conv2UCS2(const QString& str)
{
    if(isStr(str))
    {
        //remove the ""
        QString s = str.mid(1,str.length() - 2);
        unsigned char c[512];
        int j=0;
        for(int i=0; i< s.length(); i++)
        {
            c[j++] = s.at(i).row();
            c[j++] = s.at(i).cell();
        }
        
        bytes2String(c,s,s.length()*2);
        return s;    
    }
    
    //not string, return directly
    if(str.length()%2)
    {
        return "0" + str;
    }
    return str;
    return conv2Hex(str);
}

QString WPUtil::conv2UCS2ForLittleFormad(const QString& str)
{
    if(isStr(str))
    {
        //remove the ""
        QString s = str.mid(1,str.length() - 2);
        unsigned char c[512];
        int j=0;
        for(int i=0; i< s.length(); i++)
        {
            c[j++] = s.at(i).cell();
            c[j++] = s.at(i).row();
        }

        bytes2String(c,s,s.length()*2);
        return s;
    }

    //not string, return directly
    if(str.length()%2)
    {
        return "0" + str;
    }
    return str;
    return conv2Hex(str);
}

QString WPUtil::getLen(const QString& str)
{
    unsigned char c[1];
    c[0] = str.length()/2;
    QString s;
    bytes2String(c,s,1);
    return s;
}
         
QString WPUtil::getFileInfo(const QString& str)
{
    unsigned char c[20];
    string2Bytes(str,c);
    
    //MF or DF
    if(c[6] == 1 || c[6] == 2)
    {
        return getFileInfo(c,DF());
    }
    else if(c[6] == 4)
    {
        return getFileInfo(c,EF());
    }
    
    return "Unknown file type";
}
    
QString WPUtil::getFileInfo(unsigned char* c, DF)
{
    QString fileType = "File Type: MF";
    if(c[6] == 2)
    {
        fileType = "File type: DF";
    }
    
    //pin status
    //13, chv1 status
    QString chv1;
    if(c[13] & 0x80)
    {
        chv1 = "chv1 is disabled";
    }
    else
    {
        chv1 = "chv1 is enabled";
    }
    
        
    //18, chv1, 19, puk1, 20, chv2, 21, puk2
    QString chv1Rem = "chv1 remainning number is: " + QString().setNum(c[18]&0x0F);
    QString puk1Rem = "puk1 remainning number is: " + QString().setNum(c[19]&0x0F);
    QString chv2Rem = "chv2 remainning number is: " + QString().setNum(c[20]&0x0F);
    QString puk2Rem = "puk2 remainning number is: " + QString().setNum(c[21]&0x0F);
    
    return fileType + "\r\n" + chv1 + "\r\n" + chv1Rem + "\r\n"
           + puk1Rem + "\r\n" + chv2Rem + "\r\n" + puk2Rem;
}
        

QString WPUtil::getFileInfo(unsigned char* c, EF)
{
    //size info
    int fileSize =c[2]*256 + c[3];
    QString fileSizeStr = "File Size: \t\t" + QString().setNum(fileSize);
    
    int recordLen = 1;
    QString recordLenStr;
        
    //get the file type
    QString fileType;

    switch(c[13])
    {
        case 0:
           fileType = "Transparent";
           break;
        case 1:
           fileType = "Linear Fix";
           recordLen = c[14]&0xFF;
           recordLenStr = QString().setNum(recordLen);
           break;
        case 3:
           fileType = "Cylic";
           break;
        default:
           fileType = "Unknow";
    }

    fileType = "File Type: \t\t" + fileType;
    
    //access condition
    //8-10  
    unsigned char pin = (c[8]>>4)&0x0F;
    QString readCon = "Read Condition: \t\t";
    if(pin == 0)
    {
        readCon += "Always";
    }
    else
    {
        readCon += "chv" + QString().setNum(pin);
    }
    
    pin = c[8]&0x0F;    
    QString updateCon = "Update Condition: \t";
    if(pin == 0)
    {
        updateCon += "Always";
    }
    else
    {
        updateCon += "chv" + QString().setNum( c[8]&0x0F);
    }
    
    pin = (c[9]>>4)&0x0F;
    QString increaseCon = "Increase Condition: \t";
    if(pin == 0)
    {
        increaseCon += "Always";
    }
    else
    {
        increaseCon += "chv" + QString().setNum(pin);
    }
    
    pin = c[10]&0x0F;
    QString invalidateCon = "Invalidate Condition: \t";
    if(pin == 0)
    {
        invalidateCon += "Always";
    }
    else
    {
        invalidateCon += "chv" + QString().setNum( pin );
    }
    
    pin = (c[10]>>4)&0x0F;
    QString rehabilitateCon = "Rehabilitate Condition: \t";
    if(pin ==0)
    {
        rehabilitateCon += "Always";
    }
    else
    {
        rehabilitateCon += "chv" + QString().setNum(pin);
    }
    
    
    QString rt = fileType + "\r\n";
    
    rt += fileSizeStr + "\r\n";
    
    if(!recordLenStr.isEmpty())
    {
    	rt += "Record Number: \t\t" + QString().setNum(fileSize/recordLen) + "\r\n";
        rt += "Record Length: \t\t" + recordLenStr + "\r\n";
    }
    rt += readCon + "\r\n";
    rt += updateCon + "\r\n";
    rt += increaseCon + "\r\n";
    rt += invalidateCon + "\r\n";
    rt += rehabilitateCon + "\r\n";
        
        // access status
    return rt;
}


QString WPUtil::rightPadding(const QString& src, const QString& padding, int len)
{
    if(src.length()/2 > len)
    {
        return src;
    }
    QString rt = src;
    for(int i=0; i<len - src.length()/2; i++)
    {
        rt += padding;
    }
    return rt;
}


    
unsigned char WPUtil::a2c(unsigned char a)
{
    if(a<=0x39)
    {
        return a-0x30;
    }
    return a- 'A' + 0x0A;
}
    
unsigned char WPUtil::c2a(unsigned char c)
{
    if(c<=9)
    {
        return c+0x30;
    }
    return c-0x0A+'A';
}
 
bool WPUtil::isHex(const QString& str)
{
    if(str.length()> 3 && str.left(2).toLower() == "0x" )
    {
        return true;
    }
    return false;
}
    

bool WPUtil::isStr(const QString& str)
{
    
    if(str.at(0) == '\"' && 
       str.at(str.length()-1) == '\"' )
    {
        return true;
    }
    return false;
}

QString WPUtil::conv2Hex(const QString& str,int)
{
	  bool ok;
	  QString rt;
	  unsigned char c[4];
	  
	  
	  unsigned long d = str.toLong(&ok);
	  if(!ok)
	  {
	  	d = str.toLong(&ok,16);
	  }
	  if(!ok)
	  {
	  	return str;
	  }	
	  
	  c[3] = d&0xFF;
	  c[2] = (d&0xFF00)>>8;
	  c[1] = (d&0xFF0000)>>16;
	  c[0] = (d&0xFF000000)>>24;
	  
	  if(!ok)
	  {
	  	return rt;
	  }
	  if(d>0x00FFFFFFFF)
	  {
	  	bytes2String(c,rt,4);
	  }
	  else if(d>0xFFFF)
	  {
	  	bytes2String(c+1,rt,3);
	  }
	  else if(d>0xFF)
	  {
	  	bytes2String(c+2,rt,2);
	  }	
	  else
	  {
	  	bytes2String(c+3,rt,1);
	  }
	  return rt;
	  
	  /*
    typedef union data
    {
        int d;
        unsigned char c[8];
    };
    data x;
    bool ok;
    x.d = str.toInt(&ok);
    
    if(ok)
    {
        //convert to hex format
        QString s;
        if(x.d < 0x100)
        {
            bytes2String(x.c,s,1);
        }
        else
        {
            bytes2String(x.c,s,2);
        }
                   
        return s;
    } 
    
    return str;
    */
}

    

QString WPUtil::conv2Hex(const QString& str,char)     
{
    QString s = str.mid(1,str.length()-2);
    unsigned char c[256];
    for(int i=0; i< s.length(); i++)
    {
        c[i] = s.at(i).cell();
       
    }
    bytes2String(c,s,s.length());
    
    return s;
}



QByteArray WPUtil::aesEnc(const QByteArray& key, const QByteArray& d,unsigned char p)
{
	//check aes
	AES_KEY aesKey;
	
	unsigned char k[16];
	copyByteArray(key,k);
	int len = d.length();
	int pl = len%16;
	len += pl;
	
	unsigned char* pIn = new unsigned char[len];
	//unsigned char pIn[20000];
	copyByteArray(d,pIn);
	for(int i=d.length(); i<len; i++)
	{
		pIn[i] = p;
	}
	unsigned char* pOut = new unsigned char[len];
	//unsigned char pOut[20000];
	
	AES_set_encrypt_key(k, 16*8,&aesKey);
	for(int i=0;i<len;i+=16)
	{
		AES_encrypt(pIn+i,pOut+i,&aesKey);
	}
	
	QByteArray rt= QByteArray((const char*)pOut,len);
	
	delete pIn;
	delete pOut;
	
	return rt;
}	

QByteArray WPUtil::aesDec(const QByteArray& key, const QByteArray& d)
{
		//check aes
	AES_KEY aesKey;
	
	unsigned char k[16];
	copyByteArray(key,k);


	int len = d.length();
	unsigned char* pIn = new unsigned char[len];
	copyByteArray(d,pIn);
	
	unsigned char* pOut = new unsigned char[len];
	

	AES_set_decrypt_key(k, 16*8,&aesKey);
	for(int i=0;i<len;i+=16)
	{
		AES_decrypt(pIn+i,pOut+i,&aesKey);
		
	}
	QByteArray rt = QByteArray((const char*)pOut,len);
	
	delete pIn;
	delete pOut;
	
	return rt;
}	
	
QByteArray WPUtil::sha1(const QByteArray& d)
{
	unsigned char md[20];
	SHA1( (const unsigned char *)d.data(), d.length(), md);
	
	return QByteArray( (const char*)md,20);
	
}

QByteArray WPUtil::md5(const QByteArray& d)
{
	unsigned char md[16];
	memset(md,0,16);
	MD5( (const unsigned char *)d.data(), d.length(), md);
	
	return QByteArray( (const char*)md,16);
	
}

void WPUtil::copyByteArray(const QByteArray& src, unsigned char* dest)	
{
	//strncpy((char*)dest,(const char*)src.data(),src.length() );
	for(int i=0; i<src.length(); i++)
	{
		dest[i] = src.at(i);
	}
}

QByteArray WPUtil::getRandom()
{
	char c[4];
	for(int i=0;i<4;i++)
	{
		c[i] = qrand();
	}
	return QByteArray(c,4);
}	


char WPUtil::readFlag()
{
    return mdisplayTag;
}

void WPUtil::setFlag(char flag)
{
    mdisplayTag = flag;
}

