#pragma once



template <typename T>
class StateBase
{
public:
	virtual void Start(T* type) = 0;
	virtual void Update(T* type, float dt) = 0;
	virtual void End(T* type) = 0;
	
	virtual bool CanTransitionFrom(T* type, int currentState) { return true; };
};


