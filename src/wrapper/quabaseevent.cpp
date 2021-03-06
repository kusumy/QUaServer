#include "quabaseevent.h"

#ifdef UA_ENABLE_SUBSCRIPTIONS_EVENTS

#include <QUaServer>

const QStringList QUaBaseEvent::DefaultProperties = QStringList()
<< "EventId"
<< "EventType"
<< "SourceNode"
<< "SourceName"
<< "Time"
<< "ReceiveTime"
//<< "LocalTime" // NOTE : removed because is optional and open62541 now does not add it
<< "Message"
<< "Severity";

QUaBaseEvent::QUaBaseEvent(QUaServer *server)
	: QUaBaseObject(server)
{
	// copy temp originator nodeId, this was user can trigger the event in its derived class constructor
	m_nodeIdOriginator = *server->m_newEventOriginatorNodeId;
	this->setTime(QDateTime::currentDateTimeUtc());

	// NOTE : removed because is optional and open62541 now does not add it
	//this->setLocalTime(QTimeZone::systemTimeZone());

	// set a QObject parent, so event is deleted when originator is deleted
	auto nodeOriginator = QUaNode::getNodeContext(m_nodeIdOriginator, server);
	if (nodeOriginator)
	{
		this->setParent(nodeOriginator);
	}
	else
	{
		this->setParent(server);
	}
}

QByteArray QUaBaseEvent::eventId() const
{
	return const_cast<QUaBaseEvent*>(this)->getEventId()->value().toByteArray();
}

QString QUaBaseEvent::eventType() const
{
	return const_cast<QUaBaseEvent*>(this)->getEventType()->value().toString();
}

QString QUaBaseEvent::sourceNode() const
{
	return const_cast<QUaBaseEvent*>(this)->getSourceNode()->value().toString();
}

QString QUaBaseEvent::sourceName() const
{
	return const_cast<QUaBaseEvent*>(this)->getSourceName()->value().toString();
}

void QUaBaseEvent::setSourceName(const QString & strSourceName)
{
	this->getSourceName()->setValue(strSourceName);
}

QDateTime QUaBaseEvent::time() const
{
	return const_cast<QUaBaseEvent*>(this)->getTime()->value().toDateTime().toUTC();
}

void QUaBaseEvent::setTime(const QDateTime & dateTime)
{
	this->getTime()->setValue(dateTime.toUTC());
}

QDateTime QUaBaseEvent::receiveTime() const
{
	return const_cast<QUaBaseEvent*>(this)->getReceiveTime()->value().toDateTime().toUTC();
}

/*
// NOTE : removed because is optional and open62541 now does not add it
QTimeZone QUaBaseEvent::localTime() const
{
	return qvariant_cast<QTimeZone>(const_cast<QUaBaseEvent*>(this)->getLocalTime()->value());
}

void QUaBaseEvent::setLocalTime(const QTimeZone & localTimeZone)
{
	this->getLocalTime()->setValue(QVariant::fromValue(localTimeZone), METATYPE_TIMEZONEDATATYPE);
}
*/

QString QUaBaseEvent::message() const
{
	return const_cast<QUaBaseEvent*>(this)->getMessage()->value().toString();
}

void QUaBaseEvent::setMessage(const QString & strMessage)
{
	this->getMessage()->setValue(strMessage, METATYPE_LOCALIZEDTEXT);
}

quint16 QUaBaseEvent::severity() const
{
	return (quint16)const_cast<QUaBaseEvent*>(this)->getSeverity()->value().toUInt();
}

void QUaBaseEvent::setSeverity(const quint16 & intSeverity)
{
	this->getSeverity()->setValue(intSeverity);
}

void QUaBaseEvent::trigger()
{
	Q_ASSERT(!UA_NodeId_isNull(&m_nodeId));
	Q_ASSERT(!UA_NodeId_isNull(&m_nodeIdOriginator));
	// NOTE : event life-time attached to C++ instance life-time
	auto st = UA_Server_triggerEvent(
		m_qUaServer->m_server,
		m_nodeId,           // nodeId if the event
		m_nodeIdOriginator, // originating node
		NULL,               // the EventId of the new event
		false               // (do not) delete event node
	);
	Q_ASSERT(st == UA_STATUSCODE_GOOD);
	Q_UNUSED(st);
}

QUaProperty * QUaBaseEvent::getEventId() 
{
	return this->findChild<QUaProperty*>("EventId");
}

QUaProperty * QUaBaseEvent::getEventType()
{
	return this->findChild<QUaProperty*>("EventType");
}

QUaProperty * QUaBaseEvent::getSourceNode()
{
	return this->findChild<QUaProperty*>("SourceNode");
}

QUaProperty * QUaBaseEvent::getSourceName()
{
	return this->findChild<QUaProperty*>("SourceName");
}

QUaProperty * QUaBaseEvent::getTime()
{
	return this->findChild<QUaProperty*>("Time");
}

QUaProperty * QUaBaseEvent::getReceiveTime()
{
	return this->findChild<QUaProperty*>("ReceiveTime");
}

QUaProperty * QUaBaseEvent::getLocalTime()
{
	return this->findChild<QUaProperty*>("LocalTime");
}

QUaProperty * QUaBaseEvent::getMessage()
{
	return this->findChild<QUaProperty*>("Message");
}

QUaProperty * QUaBaseEvent::getSeverity()
{
	return this->findChild<QUaProperty*>("Severity");
}

#endif // UA_ENABLE_SUBSCRIPTIONS_EVENTS