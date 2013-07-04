#ifndef SOLVERINTERFACE_H
#define SOLVERINTERFACE_H

#include "interface.h"
#include "plugininfo.h"

#include <QList>
#include <QVariant>

namespace Solver {

enum Type {
    Dae,
    Nla,
    Ode
};

enum PropertyType {
    Integer,
    Double
};

class Property
{
public:
    explicit Property(const PropertyType &pType, const QString &pId,
                      const Descriptions &pDescriptions,
                      const QVariant &pDefaultValue,
                      const bool &pHasVoiUnit = false);

    PropertyType type() const;
    QString id() const;
    Descriptions descriptions() const;
    QVariant defaultValue() const;
    bool hasVoiUnit() const;

private:
    PropertyType mType;
    QString mId;
    Descriptions mDescriptions;
    QVariant mDefaultValue;
    bool mHasVoiUnit;
};

typedef QList<Property> Properties;

}   // namespace Solver

class SolverInterface : public Interface
{
public:
    virtual Solver::Type type() const = 0;
    virtual QString name() const = 0;
    virtual Solver::Properties properties() const = 0;

    QString typeAsString() const;

    virtual void * instance() const = 0;
};

typedef QList<SolverInterface *> SolverInterfaces;


Q_DECLARE_INTERFACE(SolverInterface, "SolverInterface")

#endif
