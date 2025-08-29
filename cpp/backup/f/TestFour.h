/******************** Test 4 **********************/

class CTestFour
{
public:
	int GetValue() { return m_nPrivate;}
	void SetValue(int nValue) { m_nPrivate = nValue;}

	friend void BadFunFour(CTestFour& four); 
private:
	int m_nPrivate;
};


class CTestFourPianZi
{
public:
	int m_notPrivate;
};