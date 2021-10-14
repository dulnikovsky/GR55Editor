#ifndef PATCHNAMEWRAPPER_H
#define PATCHNAMEWRAPPER_H

#include "patchdatawrapperbase.h"

class PatchNameWrapper : public PatchDataWrapperBase
{
    Q_OBJECT

    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)

public:

    enum Offsets
    {
        Name1  = 1,
        Name16 = Name1 + 16
    };

    explicit PatchNameWrapper(QObject *parent = nullptr) : PatchDataWrapperBase(parent) {}

    QString getName() const;
    void setName(const QString &newName);

signals:
    void nameChanged();

private slots:
    void onDataChanged(unsigned int offset, int lenght, bool byUserControl) override;

private:
    QString tmpName;

};

#endif // PATCHNAMEWRAPPER_H
