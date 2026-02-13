#pragma once
#include <vector>
#include "ICondition.h"


template<typename T>
class Transition
{
	int m_targetState;
	std::vector<ICondition<T>*> m_conditions;

public:
	Transition(int targetState)
	{
		m_targetState = targetState;
	}

	void AddCondition(ICondition<T>* condition)
	{
		m_conditions.push_back(condition);
	}

	int TryTransition(T* type)
	{
		for (int i = 0; i < m_conditions.size(); ++i)
		{
			if (m_conditions[i]->Test(type) == false)
				return -1;
		}

		return m_targetState;
	}
};