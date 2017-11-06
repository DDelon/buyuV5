#ifndef __PARSE_GAME_DATA__
#define __PARSE_GAME_DATA__

#include <unordered_map>
#include <string>

using namespace std;


#define GAMEVALUE_FLOAT(_TABLE_,_KEY_,_FIELD_)			ParseGameData::getInstance()->getField(_TABLE_,_KEY_,_FIELD_).asFloat()
#define GAMEVALUE_INT(_TABLE_,_KEY_,_FIELD_)			ParseGameData::getInstance()->getField(_TABLE_,_KEY_,_FIELD_).asInt()
#define GAMEVALUE_STRING(_TABLE_,_KEY_,_FIELD_)			ParseGameData::getInstance()->getField(_TABLE_,_KEY_,_FIELD_).asString()

struct DataValue
{
	string strVal;
	int intVal;
	float floVal;
	

	string asString(){ return strVal; };
	int asInt(){ return intVal; };
	float asFloat(){ return floVal; };
};

typedef unordered_map<string, DataValue> FieldMap;
typedef unordered_map<string, FieldMap> TableMap;
typedef unordered_map<string, TableMap> DataMap;

class ParseGameData
{
public:
	static ParseGameData * getInstance();
	void loadGameData(const char * FileName);

	DataValue getField(string sTableName, string sKeyName, string sFieldName);
	TableMap &getTabMap(string sTableName);
	bool hasData(string sTableName, string sKeyName);
	int getTableSize(string sTableName);

	

};

#endif
