#pragma once

template<typename T>
class ICondition
{
public:
	virtual bool Test(T* type) = 0;
};