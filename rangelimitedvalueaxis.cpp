#include "rangelimitedvalueaxis.h"

RangeLimitedValueAxis::RangeLimitedValueAxis(QObject *parent) :
    QValueAxis(parent)
{
    setProperty("rangeLimited", true); // This is a hack
}

void RangeLimitedValueAxis::setLowerLimit(qreal value)
{
    m_lowLimit = value;
    m_limitLowerRange = true;
}

void RangeLimitedValueAxis::setUpperLimit(qreal value)
{
    m_upLimit = value;
    m_limitUpperRange = true;
}

void RangeLimitedValueAxis::disableLowerLimit()
{
    m_limitLowerRange = false;
}

void RangeLimitedValueAxis::disableUpperLimit()
{
    m_limitUpperRange = false;
}

bool RangeLimitedValueAxis::lowerRangeLimited() const
{
    return m_limitLowerRange;
}

bool RangeLimitedValueAxis::upperRangeLimited() const
{
    return m_limitUpperRange;
}

qreal RangeLimitedValueAxis::lowerLimit() const
{
    return m_lowLimit;
}

qreal RangeLimitedValueAxis::upperLimit() const
{
    return m_upLimit;
}
