#ifndef __ROLE_H__
#define __ROLE_H__

#include "RoleData.h"

class Role : public Node
{
public:
	virtual void shoot(int propId, int rate) = 0;
	virtual void useProp(int propId,int count=1) = 0;
	virtual void cannonRateChange(int rate) = 0;

	RoleData* getRoleData() { return mRoleData; }
protected:
	RoleData *mRoleData;
};

#endif // !__ROLE_H__
