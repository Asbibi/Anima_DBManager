#include "sstringhelper.h"

namespace SStringHelper {

QString GetLanguageString(SStringLanguages _language)
{
    switch (_language)
    {
        case SStringLanguages::French:
            return "Français";
        case SStringLanguages::English:
            return "English";
    }
    return "";
}

}
