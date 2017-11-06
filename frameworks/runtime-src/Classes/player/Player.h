#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Role.h"
#include "ui/CocosGUI.h"
class Player :public Role
{
public:
	CREATE_FUNC(Player);
	bool init();

	virtual void shoot(int propId, int rate);
	virtual void useProp(int propId,int count = 1);
	virtual void cannonRateChange(int rate);

	void setMoneyUI(int propId, ui::TextBMFont* text, ui::TextBMFont* addText);
	void setGoldTextView(ui::TextBMFont* view); 
	void setAddGoldTextTextView(ui::TextBMFont* view);
	ui::TextBMFont* getGlodTextView() { return mGoldText; }
	ui::TextBMFont* getGemTextView() { return mAddGoldText; }

	//金币ui
	void refreshGoldShow(int showCount,int addCount = 0);
	//水晶ui
	void refreshGemShow(int showCount, int addCount = 0);

	//道具ui
	void refreshPropShow(int propId, int showCount, int addCount = 0, int propItemId = 0, ValueMap data = ValueMap());
	
	//积分
	void refreshScoreShow(int showCount, int addCount = 0);
	//子弹个数
	void refreshBulletShow(int showCount, int addCount = 0);

private:
	ui::TextBMFont* mGoldText;
	ui::TextBMFont* mAddGoldText;

	ui::TextBMFont* mGemText;

	ui::TextBMFont* mScoreText;	//积分
	ui::TextBMFont* mAddScoreText;
	ui::TextBMFont* mBulletText;//子弹个数

};


#endif // !__PLAYER_H__
