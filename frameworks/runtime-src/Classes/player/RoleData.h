#ifndef __ROLEDATA_H__
#define __ROLEDATA_H__
#include "cocos2d.h"
USING_NS_CC;

const int FRIEND_INDEX = 10000;
enum PropType
{
	GOLD = 1,
	GEM  = 2,
	PROP_FREEZE = 3,
	PROP_LOCK = 4,
	PROP_CALLFISH = 5,
	PROP_BOMB = 6,
	PROP_TIMEREVERT = 14,

	SENIOR_ROOMCARD = 1001,

	SCORE = FRIEND_INDEX + 100,
	USED_BULLET = FRIEND_INDEX + 101,
	BULLET = FRIEND_INDEX + 7,
};
struct PropData
{
	int propId;
	int realCount;//ʵ����ֵ
	int flyingCount;//���ڵ����е���ֵ
	int unSureCount;//δȷ������ֵ

	PropData(){
		propId = -1;
		realCount = 0;
		flyingCount = 0;
		unSureCount = 0;
	}
};

//�߼�����
struct SeniorPropData
{
	int propItemId;		//Ψһ��ʶid
	int propId;			//ͬ��id
	std::string stringProp;	//����
	int intProp1;		//
	int intProp2;		//

	SeniorPropData(){
		propItemId = -1;
		propId = -1;
		stringProp = "";
		intProp1 = 0;
		intProp2 = 0;
	}
};

struct PropFlyingData
{
	int propId;
	int flyingCount;
	std::string stringProp;	//����
	PropFlyingData() {
		propId = -1;
		flyingCount = 0;
		stringProp = "";
	}
};

struct PropFlyingRfreshData
{
	int playerId;
	int propId;
	int flyingCount;
	bool isSenior;
	int propItemId;
	ValueMap data;
	PropFlyingRfreshData()
	{
		playerId = -1;
		propId = -1;
		flyingCount = 0;
		isSenior = false;
		propItemId = 0;
		data = ValueMap();
	}
};
class RoleData : public Node
{
public:
	CREATE_FUNC(RoleData);
	bool init();

	Vec2 getCannonPos(){ return curPos; };
	//�ֶ���������,��ˢ�½���
	void setPropRealData(int propId, int propCount);

	//�Զ��������ݣ���ʱ��
	void refreshPropData(int propId, int realCount, int flyingCount = -1 , float updateDelayTime = 0.0f);
	PropData &getPropData(int propId);
	PropData &getGlod() { return getPropData(GOLD); }
	PropData &getGem() { return getPropData(GEM); }

	void isSurePropData(bool isSure, int propId, int unSureCount, std::string stringProp = "");

	//�ֶ���������
	void setAddFlyProp(bool isSure, int propId, int addCount);

	void setIsBankup(bool isBankup){ mIsBankup = isBankup; };
	bool getIsBankup(){ return mIsBankup; };

	ValueMap getAndSetPlayerData(ValueMap data);

	void clearNoRealData();

	void clearUnSureData();

	//�õ���ҵ�����
	ValueMap getPlayerData();


	//�߼�������ӣ�ɾ��,ʹ��  //useType 1.ֱ����� 2.��ʱ����  3.ֱ��ɾ��,ֱ��ʹ��  4.��ʱɾ�� 
								// 5.ʹ��,�ٿۣ����뻺��  6.ɾ������ 7.ʹ��,��ۣ�ɾ������
	void refreshSeniorPropData(int propId, ValueMap data, int useType, float updateDelayTime = 0.0f);

	//ValueMap��SeniorPropData��ת��
	void ValueMapToSeniorPropData(ValueMap& data, SeniorPropData& dataProp,bool isLeftToRighr);

	//��ͨ������ӣ�ɾ��,ʹ��  //useType 1.ֱ����� 2.��ʱ����  3.ֱ��ɾ��,ֱ��ʹ��  4.��ʱɾ�� 
	// 5.ʹ��,�ٿۣ����뻺��  6.ɾ������ 7.ʹ��,��ۣ�ɾ������
	void refreshNormalPropData(ValueMap data, int useType, float updateDelayTime = 0.0f);


	//�õ��߼������б�
	std::map<int, std::map<int, SeniorPropData>> getSeniorPropData(){ return mSeniorPropDataMap; };
	std::map<int, std::map<int, SeniorPropData>> getSeniorPropCache(){ return mSeniorPropCacheMap; };
	//ɾ���߼�����
	void eraseSeniorProp(int propId, int propItemId);
	void eraseSeniorPropCache(int propId, int propItemId);
	//���Ӹ߼�����
	void addSeniorProp(int propId, int propItemId, ValueMap data);
	void addSeniorPropCache(int propId, int propItemId, ValueMap data);

	ValueMap &getSeniorPropData(int propId, int propItemId);

	int getSeniorPropShowCount(int propId);

	void clearAllSeniorProp(int clearType);

	void clearTrueSeniorProp();
	void clearSeniorPropCache();

//��ɫ����
public:
	bool mIsBankup;
	int playerId;
	int currentGunRate;
	int maxGunRate;

	int vipExp;
	int vip_level;
	//...etc

	Vec2 curPos;

//��������
private:
	std::map<int, PropData> mPropDataMap;			//�ܵ����ݣ�������ʵ���ݺͷ�������
	std::vector <PropFlyingData> mPropFlyingVector;
	std::vector <PropFlyingData> mUnSureCountVector;

	std::map<int, std::map<int, SeniorPropData>> mSeniorPropDataMap;	//�߼�����
	std::map<int, std::map<int, SeniorPropData>> mSeniorPropCacheMap;	//�߼����߻����
};


#endif // !__ROLEDATA_H__
