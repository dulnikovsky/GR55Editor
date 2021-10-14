#ifndef PATCHDATAWRAPPERBASE_H
#define PATCHDATAWRAPPERBASE_H

#include <QObject>

class Patch;

class PatchDataWrapperBase : public QObject
{
    Q_OBJECT
public:
    explicit PatchDataWrapperBase(QObject *parent = nullptr);

    void setBaseOffset(unsigned int val) { baseOffset = val; }

signals:

protected slots:
    virtual void onDataChanged(unsigned int offset, int lenght, bool byUserControl) = 0;

protected:
    Patch *patch;
    unsigned int baseOffset{0};
};

#endif // PATCHDATAWRAPPERBASE_H
