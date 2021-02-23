#pragma once
class CProblem
{
public:
	~CProblem();
	virtual bool bConstraintsSatisfied() = 0;
	virtual double dGetQuality() = 0;
};

