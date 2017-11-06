/**
	@file		Des.h
	@date		2012年8月29日 9:27
	@brief		DES加密/解密
	@by 		来自网络,修改by首席村支书
	@Email 		wangbing323@163.com
	@QQ			32996758
*********************************************************************/
#pragma once


	/**
	* @brief Des加密/解密 
	*/
	class DL_NOEXPORT CDes
	{
		bool m_bSubKey[16][48];
		char m_cDESKey[16];

		char m_cOpBuff[512];//!<操作缓冲

		size_t m_nOpBuffSize;//!<缓冲使用大小

	public:
		CDes(){}
		/**
		* @brief 加密数据
		* @param [in] pData 需要加密的数据内容
		* @param [in] nLen 加密的数据长度(pData的长度），单位：字节
		* @param [in] pKey 加密密钥
		* @param [in] nKeyLen 加密密钥长度（pKey的长度)，单位：字节
		* @retval 如果加密成功，则返回加密后的内容，否则返回NULL
		* @note 加密长度不能超过512个字节
		*/
		void* Encrypt(const void* pData,size_t nLen, const void* pKey,size_t nKeyLen);

		//解密后长度不能超过512个字符
		/**
		* @brief 解密数据
		* @param [in] pData 待解密的数据
		* @param [in] nLen 待解密的数据长度（pData的长度)，必须是8的整数倍，单位：字节 
		* @param [in] pKey 解密密钥
		* @param [in] nKeyLen 解密密钥长度(pKey的长度),单位：字节
		* @return 如果解密成功，则返回解密后的内容，否则返回NULL
		* @note 解密长度不能超过512字节
		*/
		void* Decrypt(const void* pData,size_t nLen,const void* pKey,size_t nKeyLen);

		/**
		* @brief 加密或者解密后数据的长度
		*/
		size_t GetDataBuffSize(void)const
		{
			return m_nOpBuffSize;
		}
	private:

		void BuildKey(const char* pKey,size_t nKeyLen);

		static void CharsToBits(const char* pSrc,bool* pDest,size_t nSize);

		static void BitsToChars(bool* pSrc,char* pDest,size_t nSize);

		//矩阵映射
		static void TransMatrix(bool* pSrc,bool* pDest,const char* pTable,size_t nSize);

		static void RotateL( bool *pData,size_t nSize,int nRotateChars);

		static void Des( char* pDest, const char* pSrc,const bool* pKey,bool bEncoder =false);

		static void F_Func( bool* pData, const bool* pKey);

		static void F_Xor( bool* pDest,const bool* pSrc,size_t nLen);

		static void S_Func(bool* pDest,const bool* pSrc);

	};

