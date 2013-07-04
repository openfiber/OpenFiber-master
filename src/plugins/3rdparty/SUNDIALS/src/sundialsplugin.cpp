//==============================================================================
// SUNDIALS plugin
//==============================================================================

#include "sundialsplugin.h"

//==============================================================================

namespace SUNDIALS {

//==============================================================================

PLUGININFO_FUNC SUNDIALSPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin to access CVODE, IDA and KINSOL from the <a href=\"http://computation.llnl.gov/casc/sundials/description/description.html\">SUNDIALS</a> library."));
    descriptions.insert("ru", QString::fromUtf8("плагин для доступа к CVODE, IDA and KINSOL от<a href=\"http://computation.llnl.gov/casc/sundials/description/description.html\">SUNDIALS</a>."));

    return new PluginInfo(PluginInfo::InterfaceVersion001,
                          PluginInfo::ThirdParty,
                          false,
                          false,
                          QStringList(),
                          descriptions);
}

//==============================================================================

}   // namespace SUNDIALS

//==============================================================================
// End of file
//==============================================================================
