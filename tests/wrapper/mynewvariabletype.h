#ifndef MYNEWVARIABLETYPE_H
#define MYNEWVARIABLETYPE_H

#include <QOpcUaBaseDataVariable>

class MyNewVariableType : public QOpcUaBaseDataVariable
{
    Q_OBJECT

public:
	explicit MyNewVariableType(QOpcUaServerNode *parent);

private:

	
};

class MyOtherNewVariableType : public QOpcUaBaseDataVariable
{
	Q_OBJECT

public:
	explicit MyOtherNewVariableType(QOpcUaServerNode *parent);

private:


};


#endif // MYNEWVARIABLETYPE_H

