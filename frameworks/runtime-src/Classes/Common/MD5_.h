/**
@file MD5_.h
@brief MD5加密
@note	来源：CSDN
@note	修改原因：将原来的代码封装成类，提供MakeMD5函数实现返回结果
@note 修改时间：2009年3月31日11:40:01
@note 修改者：朱心永恒
*/
#pragma once
#ifndef MD5_H
#define MD5_H

///32位整型定义
typedef unsigned int UINT4;
///md5数据结构
typedef struct {
	UINT4 i[2];                   ///< number of _bits_ handled mod 2^64
	UINT4 buf[4];                  ///<scratch buffer
	unsigned char in[64];           ///<input buffer
	unsigned char digest[16];      ///<actual digest after MD5Final call
} MD5_CTX;

///MD5编码实现
class DL_NOEXPORT CMd5
{
private:


public:
	CMd5();

	/**
	* @brief MD5编码
	* @param [in] strIn 传入数据(需要编码的数据)
	* @param [in] nMD5Len 传输数据的长度，字节数
	* @return 返回经过编码后的值，值为二进制，非字符串
	*/
	const unsigned char* MakeMD5(const unsigned char* strIn, int nMD5Len);


	/*
	*/
	const unsigned char* GetRet()
	{
		return m_mdContext.digest;//m_mdContext.digest;
	}
public:
	void Init();
	void Update(const unsigned char *inBuf, unsigned int inLen);
	void Final();
private:
	void Transform(UINT4 *buf, UINT4 *in);
private:
	unsigned char	m_cPADDING[64];
	MD5_CTX			m_mdContext;
};

#endif
