#include "ParseGameData.h"
#include "cocos2d.h"

static ParseGameData * parseGameData = NULL;

static DataMap dataMap;
ParseGameData * ParseGameData::getInstance()
{
	if (!parseGameData)
	{
		parseGameData = new ParseGameData();
	}
	return parseGameData;
}

void ParseGameData::loadGameData(const char * FileName)
{
	ssize_t lFileSize = 0;
	unsigned char * dataBin = cocos2d::FileUtils::getInstance()->getFileData(FileName, "rb", &lFileSize);

	for (ssize_t i = 0; i < lFileSize; i++)
	{
		dataBin[i] = ~dataBin[i];
	}

	if (dataBin)
	{
		long lDataOffset = 0;
		while (lDataOffset < lFileSize)
		{
			int iTableSize = 0;//表名长度int
			memcpy(&iTableSize, dataBin + lDataOffset, 4); 
			lDataOffset += 4;

			char * cTableName = new char[iTableSize + 1];//表名string
			memset(cTableName, 0, iTableSize + 1);
			memcpy(cTableName, dataBin + lDataOffset, iTableSize); 
			lDataOffset += iTableSize;

			int iRow = 0;//表行数int
			memcpy(&iRow, dataBin + lDataOffset, 4);
			lDataOffset += 4;

			TableMap tm;

			for (int i1 = 0; i1 < iRow; i1++)
			{
				int iRowKey = 0;//行Key值长度int
				memcpy(&iRowKey, dataBin + lDataOffset, 4);
				lDataOffset += 4;

				char * cRowKey = new char[iRowKey + 1];//行Key值string
				memset(cRowKey, 0, iRowKey + 1);
				memcpy(cRowKey, dataBin + lDataOffset, iRowKey); 
				lDataOffset += iRowKey;

				int iCol = 0;//列数int
				memcpy(&iCol, dataBin + lDataOffset, 4);
				lDataOffset += 4;

				FieldMap fm;

				for (int i2 = 0; i2 < iCol; i2++)
				{


					int iFieldName = 0;//字段名长度int
					memcpy(&iFieldName, dataBin + lDataOffset, 4);
					lDataOffset += 4;

					char * cFieldName = new char[iFieldName + 1];//字段名string
					memset(cFieldName, 0, iFieldName + 1);
					memcpy(cFieldName, dataBin + lDataOffset, iFieldName);
					lDataOffset += iFieldName;


					int iField = 0;//字段长度int
					memcpy(&iField, dataBin + lDataOffset, 4);
					lDataOffset += 4;

					char * cField = new char[iField + 1];//字段string
					memset(cField, 0, iField + 1);
					memcpy(cField, dataBin + lDataOffset, iField);
					lDataOffset += iField;


					DataValue dv;
					dv.strVal = cField;
					
					bool isNumber = true;
					bool isFloat = false;
					
					
					for (int i = 0; i < dv.strVal.size();i++)
					{
						if (i != 0 && dv.strVal[i]=='-')
						{
							isNumber = false;
							break;
						}

						if ((dv.strVal[i] >= 48 && dv.strVal[i] <= 57) || dv.strVal[i]=='-')
						{
						}
						else
						{
							if (dv.strVal[i] == 46)
							{
								if (isFloat)
								{
									isNumber = false;
									break;
								}
								else
								{
									isFloat = true;
								}
							}
							else
							{
								isNumber = false;
								break;
							}

						}
					}

					if (isNumber)
					{
						dv.intVal = atoi(cField);
						dv.floVal = atof(cField);
					}
					else
					{
						dv.intVal = 0;
						dv.floVal = 0.f;
					}

					fm.insert(make_pair(cFieldName, dv));
					delete[] cFieldName;
					delete[] cField;

				}

				tm.insert(make_pair(cRowKey, fm));
				delete[] cRowKey;
			}
			dataMap.insert(make_pair(cTableName, tm));
			delete[] cTableName;

		}
		delete[] dataBin;
	}

// 	DataMap::iterator iter = dataMap.begin();
// 	for (; iter != dataMap.end();iter++)
// 	{
// 		CCLOG(" ---- Table Name = %s", (*iter).first.c_str());
// 	}

	//std::string lau = GAMEVALUE_STRING("language", "800000008", "ch");
}

DataValue ParseGameData::getField(string sTableName, string sKeyName, string sFieldName)
{
	return dataMap[sTableName][sKeyName][sFieldName];
}

TableMap &ParseGameData::getTabMap(string sTableName)
{
	return dataMap[sTableName];
}

int ParseGameData::getTableSize(string sTableName)
{
	return dataMap[sTableName].size();
}

bool ParseGameData::hasData(string sTableName, string sKeyName)
{
	return GAMEVALUE_INT(sTableName, sKeyName,"id") == atoi(sKeyName.c_str());
}

