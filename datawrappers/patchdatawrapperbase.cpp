#include "patchdatawrapperbase.h"

#include "patch.h"

PatchDataWrapperBase::PatchDataWrapperBase(QObject *prnt) : QObject(prnt)
{
    patch = dynamic_cast<Patch *>(parent());
    if(patch != nullptr)
    {
        connect(patch, &Patch::dataChanged, this, &PatchDataWrapperBase::onDataChanged);
    }
}
