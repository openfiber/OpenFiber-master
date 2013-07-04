#include "solverinterface.h"

#include <QObject>

namespace Solver {

Property::Property(const PropertyType &pType, const QString &pId,
                   const Descriptions &pDescriptions,
                   const QVariant &pDefaultValue,
                   const bool &pHasVoiUnit) :
    mType(pType),
    mId(pId),
    mDescriptions(pDescriptions),
    mDefaultValue(pDefaultValue),
    mHasVoiUnit(pHasVoiUnit)
{
}

PropertyType Property::type() const
{
    // Return our type

    return mType;
}

QString Property::id() const
{
    // Return our id

    return mId;
}

Descriptions Property::descriptions() const
{
    // Return our descriptions

    return mDescriptions;
}

QVariant Property::defaultValue() const
{
    // Return our default value

    return mDefaultValue;
}

bool Property::hasVoiUnit() const
{
    // Return whether we have a VOI unit

    return mHasVoiUnit;
}

}   // namespace Solver

QString SolverInterface::typeAsString() const
{
    // Return the type of the solver as a string

    switch (type()) {
    case Solver::Ode:
        return QObject::tr("ODE");
    case Solver::Dae:
        return QObject::tr("DAE");
    case Solver::Nla:
        return QObject::tr("NLA");
    default:
        return "???";
    }
}
