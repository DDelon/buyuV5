/**
@file	XMLConfig.h
@date	2009年10月9日13:59:57
@brief	XML文件解析类
* 修改:	2012年7月31日 09:55:12
@by 		朱心永恒
@Email	wangbing323@163.com
@QQ		32996758

*********************************************************************/
#pragma once


/**
* @brief XML文件编码 
*/
enum emCodeType
{
	///没有指定编码
	CODE_TYPE_NULL,
	///GB2312编码（多字节编码）
	CODE_TYPE_GB2312,
	///UTF8编码
	CODE_TYPE_UTF8,
	///WINDOWS 下默认 UNICODE编码(UTF-16LE)
	CODE_TYPE_UTF16LE,
		
	///UTF-16BE编码
	CODE_TYPE_UTF16BE,

	///未知编码
	CODE_TYPE_UNKOWN=-2,
	///错误编码
	CODE_TYPE_ERROR = -1,
};

/**
* @brief 节点类型
*/
enum xmlNodeType
{
	XML_NODE_TYPE_ROOT,					//!<根节点
	XML_NODE_TYPE_CHILD	= 1,			//!<子节点
	XML_NODE_TYPE_DATA	= 2,			//!<数据
	XML_NODE_TYPE_CDATA	= 4,			//!<CDATA数据
	XML_NODE_TYPE_REM	= 8,			//!<注释
};
class CXMLConfig;

///XML子节点列表
typedef std::list<CXMLConfig*> XMLCHILD;
	
///XML子节点列表迭代器
typedef std::list<CXMLConfig*>::iterator XMLCHILDIT;

///XML配置类
class DL_NOEXPORT CXMLConfig:public CBaseUserData
{
public:
	StringA						m_strName;			//!< 节点名
	///节点附带数据
	StringA						m_strData;
	///多级配置时，父节点配置
	CXMLConfig*					m_pParentConfig;	//多级配置

	///节点属性列表
	std::list<std::pair<StringA,StringA> >* m_pListProperty;

	///子节点列表
	std::list<CXMLConfig*>*		m_plstChild;	//子节点
		
	///空字符串（返回值用）
	static const StringA			m_strNull;		//空字符串

	DWORD						m_dwXmlNodeType;	///<节点类型
	/**
	* @brief 遍历函数定义。
	* @param [in] 1st 遍历到的子节点
	* @param [in] 2nd 由用户传递的数据,等于@ref EnumChildren 的第二个参数。
	* @retval 如果需要继续遍历，返回非0,停止遍历返回FALSE。
	*/
	typedef bool (*FnConfigEnum)(CXMLConfig*,void* );

public:
	///属性和方法注册
	static bool Register(lua_State* L);
	///lua类绑定
	LUACLASS(CXMLConfig,CBaseUserData);
public:
		
	/**
	* @brief 构造一个新节点
	* @param [in] strName 节点名
	* @param [in] pConfig 父节点指针，如果为顶级节点，则该参数为NULL
	* @param [in] nodeType 节点类型
	*/
	CXMLConfig( const StringA& strName = "" ,  CXMLConfig* pConfig = NULL, xmlNodeType nodeType = XML_NODE_TYPE_ROOT);
	
	/*virtual*/ ~CXMLConfig(void);

	/**
	* @brief 根据节点名查找节点\子节点
	* @param [in] strNodeName 节点名
	* @return 返回第一个找到的子节点指针，如果没找到则返回NULL
	* @note 查找时按深度查找
	*/
	CXMLConfig* FindNodeByName(const char* strNodeName);
	
	/**
	* @brief 根据节点属性查找节点
	* 查找某个节点的属性和属性值等于给定值
	* @param [in] strPropertyName 属性名
	* @param [in] strPropertyValue 属性值
	* @return 返回第一个找到的子节点指针，如果没找到则返回NULL
	* @note 查找时按深度查找
	*/
	CXMLConfig* FindNodeByProperty(const char* strPropertyName,const char* strPropertyValue);


	/**
	* @brief  根据节点附带内容查找
	* @param [in] strNodeData 节点附带内容
	* @return 返回第一个找到的子节点指针，如果没找到则返回NULL
	* @note 查找时按深度查找
	*/
	CXMLConfig* FindNodeByData(const char* strNodeData);

	///返回子节点链表
	const std::list< CXMLConfig* >& GetChild(void)const
	{
		return *m_plstChild;
	}

	///返回子节点链表
	std::list< CXMLConfig* >& GetChild(void)
	{
		return *m_plstChild;
	}

	///获得节点名
	const StringA& GetNodeName(void)const
	{
		return m_strName;
	}

	///获得节点数据
	const StringA& GetNodeData(void)const
	{
		return m_strData;
	}

	/**
	* @brief  设置节点名
	* @param [in] strName 节点名
	*/
	void SetNodeName(const char* strName)
	{
		m_strName = strName;
	}

	///设置节点数据
	void SetNodeData(const StringA& strNodeData)
	{
		m_strData = strNodeData;
	}

	/**
	* @brief  设置属性
	* @param [in] strName 属性名
	* @param [in] lpProperty 属性值，如果为NULL，则删除该属性
	*/
	void SetProperty(const char* strName, const char* lpProperty);

	/**
	* @brief  设置属性
	* @param [in] strName 属性名
	* @param [in] nValue 属性值,整型
	*/
	void SetValue(const char* strName,int nValue)
	{
		char szBuff[16];
		sprintf(szBuff,"%d",nValue);
		SetProperty(strName,szBuff);
	}

	/**
	* @brief 获得属性
	* @param [in] strName 属性名
	* @param [in] lpDef 默认属性值
	* @return 如果找到对应属性，则返回属性值，如果没找到，则返回lpDef
	*/
	const char* GetProperty(const StringA& strName,const char* lpDef = NULL )const;

	/**
	* @brief 获得属性
	* @param [in] strName 属性名
	* @param [in] nDefault 默认属性值,整型
	* @return 如果找到对应属性，则返回属性值，如果没找到，则返回nDefault
	*/
	int GetValue( const StringA& strName, int nDefault = 0)const;

	/**
	* @brief 获得颜色值
	* 可用10进制，16进制，或者RGB描述，比如256，0x0000Ff,RGB(255,,0)都是合法的值,都为256
	* @param [in] strName 属性名
	* @param [in] dwDefault 默认属性值
	* @return 如果找到对应属性，则返回属性值，如果没找到，则返回dwDefault
	*/
	DWORD GetColor(const StringA& strName,DWORD dwDefault = 0)const;

	/**
	* @brief 设置颜色值，存储为16进制
	* @param [in] strName 属性名
	* @param [in] dwColor 默认颜色
	*/
	void SetColor(const char* strName,DWORD dwColor)
	{
		char szBuff[16];
		sprintf(szBuff,"0x%08X",static_cast<unsigned>(dwColor));
		SetProperty(strName,szBuff);
	}
	///增加一个子节点
	void AddChildNode( CXMLConfig* pNode);

	///删除一个节点
	bool RemoveNode( CXMLConfig* pNode);

	/**
	* @brief  读取XML文件，支持DOM头
	* @param [in] strFileName XML文件路径
	* @return 成功返回true

	*/
	bool ReadXML( const char* strFileName);

	/**
	* @brief 从内存中读取XML数据，支持DOM头
	* @param [in] pData 内存数据指针
	* @param [in] dwLen 数据长度，pData的字节数，单位：字节
	* @return 成功返回true
	*/
	bool ReadXMLFromMemory( BYTE* pData, DWORD dwLen);


	/**
	* @brief 从字符串中读取XML内容(编码为当前项目配置编码)
	* @param [in] lpXML 字符串格式的XML内容
	* @return 成功返回true
	*/
	bool ReadXMLFromStringImp(char* lpXML)
	{
		try
		{
			Clear();
			m_pParentConfig = NULL;
			DeleteRemString(lpXML);
			TranslateString(this,lpXML);
			CompactNode(this);
			return true;
		}
		catch(...)
		{
			return false;
		}
	}

	/**
	* @brief 从字符串中读取XML内容(编码为当前项目配置编码)
	* @param [in] strXML 字符串格式的XML内容
	* @return 成功返回true
	*/
	inline bool ReadXMLFromString(const StringA& strXML)
	{
		StringA strTemp = strXML;
		char* lpXML = &strTemp.at(0);
		return ReadXMLFromStringImp(lpXML);
	}

	/**
	* @brief 将XML数据写入文件
	* @param [in] strFileName 要写入的XML文件名
	* @param [in] bSaveVersion 是否写入版本信息，true为写入
	* @param [in] bExpand 是否将属性展开为子节点，true为展开
	* @param [in] dwCodeType 使用编码类型，编码类型为@ref emCodeType 的一种
	* @return 成功生成xml文件返回非0
	*/
	bool SaveXML(  const StringA& strFileName, bool bSaveVersion = true, bool bExpand=false, DWORD dwCodeType=CODE_TYPE_UTF8 );

	/**
	* @brief 将XML数据写入二进制文件
	* @param [in] strFileName 要写入的XML文件名
	* @param [in] bCompress 是否压缩
	* @param [in] dwCodeType 使用编码类型，当前只支持默认值
	*/

	bool SaveRawXML(const StringA& strFileName,bool bCompress = true,DWORD dwCodeType

		=CODE_TYPE_UTF16LE

		);

	/**
	* @brief 保存编码数据到向量中
	* @param [out] vData 保存的数据
	* @param [in] bCompress 是否压缩
	* @param [in] dwCodeType 编码类型，为@ref emCodeType 值之一
	* @return 成功返回true
	*/
	bool SaveRawToVector(std::vector<BYTE>& vData,bool bCompress = true,DWORD dwCodeType
		=CODE_TYPE_UTF16LE

		);
	/**
	* @brief 将XML数据写入字符串 
	* @param [out] strSave 要写入的目标字符串
	* @param [in] bSaveVersion 是否写入版本信息，true为写入
	* @param [in] bExpand 是否将属性展开为子节点，true为展开
	* @param [in] dwCodeType 使用编码类型，编码类型为@ref emCodeType 的一种
	* @return 成功保存返回非0
	*/
	bool SaveToString( StringA& strSave, bool bSaveVersion = true , bool bExpand = false , DWORD dwCodeType=CODE_TYPE_UTF8 );

	/**
	* @brief 清空所有数据，包括子节点、属性等 
	*/
	void Clear(void);

	/**
	* @brief 分配一个新节点做为当前节点的子节点
	* @param [in] strName 节点名
	* @param [in] nodeType 节点类型
	* @return 新分配的节点指针
	*/
	CXMLConfig* NewChild(  const StringA& strName,xmlNodeType nodeType = XML_NODE_TYPE_CHILD);

	/**
	* @brief 获得当前节点的父节点
	* @return 如果存在父节点，则返回父节点指针，如果不存在，则返回NULL
	*/
	CXMLConfig* GetParent(void)
	{
		return dynamic_cast<CXMLConfig*>(m_pParentConfig);
	}


	///从pNode克隆数据
	void Clone(CXMLConfig* pNode)
	{
		if(pNode == this)
			return ;

		SetNodeData(pNode->GetNodeData());
		//克隆属性
		*m_pListProperty = *pNode->m_pListProperty;
		//克隆子节点
		std::list<CXMLConfig*>::iterator it = pNode->m_plstChild->begin();
		for(;it !=pNode->m_plstChild->end();++it)
		{
			CXMLConfig* pChild = *it;
			CXMLConfig* pNewChild = NewChild(pChild->GetNodeName(),static_cast<xmlNodeType>(pChild->m_dwXmlNodeType));
			pNewChild->Clone(pChild);
		}
	}

	/**
	* @brief 获得XML节点路径最后一个节点指针
	* @param [in] lpPath 节点路径，必须是类似格式xmlNode1/xmlNode2/xmlNode3
	* @param [in] bCreate 如果节点不存在，是否创建节点
	* @return 如果节点存在或者被创建，则返回节点指针，否则返回NULL
	*/
	CXMLConfig* GetPathNode( const char* lpPath,bool bCreate = false);

private:
	///翻译XML文件内容
	static void TranslateString( CXMLConfig* pNode, char*& lpXMLData);

	///翻译XML段
	static void TranslateXMLSection( CXMLConfig* pNode,char*& strXMLData,int nLen);

	///获得段内数据
	static bool GetXMLValue( char*& lpXMLData);


	///删除注释字符串
	static void DeleteRemString( char*& lpXML);

	///写入节点到流
	static void SaveNode( StringA& ofs, CXMLConfig* pNode, unsigned uLevel);

	///写入节点到流并展开
	static void SaveNodeExpand( StringA& ofs, CXMLConfig* pNode, unsigned uLevel);

	///简化节点
	static void CompactNode( CXMLConfig* pNode);

	///从字节流中获得编码
	static DWORD GetCodeType( BYTE* pData, DWORD dwSize);

public:
	/**
	* @brief 获得配置类型
	* @return 返回配置字符串，永远为"xml"
	*/
	const StringA& GetConfigType(void)const
	{
		static const StringA& strType = "xml";
		return strType;
	}

	/**
	* @brief 遍历所有子节点。
	* @param [in] fn 回调函数
	* @param [in] lpData 用户自定义数据,将传递给fn做为第一个参数
	* @param [in] bRecursion 是否遍历子节点的子节点。
	* @return 返回非0则遍历完毕，否则意味着fn返回FALSE中断了遍历
	*/
	bool EnumChildren( FnConfigEnum fn,void *lpData,bool bRecursion = false);

	/**
	* @brief LUA版的@ref ReadXMLFromMemory
	*/
	static int L_ReadXMLFromMemory(lua_State* L);

	/**
	* @brief 将xml转换为table(lua)
	*/
	static int L_ToTable(lua_State* L);
	/**
	* @brief 将table转换为xml(lua)
	*/
	static int L_FromTable(lua_State* L);
private:
	void ToTableImp(lua_State* L);
	void FromTableImp(lua_State* L);
private:
	static int StringFind(const char* lpText, const char* lpFindText,int nLen = -1);
	static void StringRemoveSpace(char*& lpText);
	static int StringRemoveSpaceFromBack(char*& lpText,int nLen = -1);

	bool ReadXMLFromMemoryWithRaw(BYTE* &pData, DWORD& dwLen );

	bool ReadXMLFromMemoryWithCodeType( DWORD dwCodeType, BYTE* &pData, DWORD dwLen );

	void WriteXMLToMemoryWithRaw(std::vector<BYTE>& vData );
	static bool ReadStringFromRaw(StringA& strRead,BYTE* & pData,DWORD& dwLen);
	static void WriteStringToRaw(const StringA& strUtf8,std::vector<BYTE>& vData)
	{
		StringW strWrite = CHelper::StringAToUtf16(strUtf8);
		vData.insert(vData.end(),reinterpret_cast<const BYTE*>(strWrite.c_str()),reinterpret_cast<const BYTE*>
					(strWrite.c_str())+sizeof(WORD)*(strWrite.length()+1));
	}
	template<typename T>
	static void WriteDataToRaw(T val,std::vector<BYTE>& vData)
	{
		vData.insert(vData.end(),reinterpret_cast<const BYTE*>(&val),reinterpret_cast<const BYTE*>(&val) + sizeof(T));
	}

private:
	static int L_GetChild(lua_State* L);
	static int L_GetProperty(lua_State* L);
	static int L_GetValue(lua_State* L);
	static int L_GetColor(lua_State* L);
// 	static int L_ReadXMLFromString(lua_State* L)
// 	{
// 		CXMLConfig* pThis =GetThis(L,1);
// 		StringA strXML;
// 		Check(L,2,strXML);
// 		Push(L,pThis->ReadXMLFromString(strXML));
// 		return 1;
// 	}
	static int L_SaveXML(lua_State* L);
	static int L_SaveRawXML(lua_State* L);
	static int L_SaveToString(lua_State* L);
	static int L_NewChild(lua_State* L);
};

