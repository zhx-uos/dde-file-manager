/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -i ../../plugins/server/dfmplugin-interfaces/devicemanagerdbus.h -c DeviceManagerAdaptor -l DeviceManagerDBus -a ../../plugins/server/dfmplugin-interfaces/dbus_adaptor/devicemanagerdbus_adaptor devicemanagerdbus.xml
 *
 * qdbusxml2cpp is Copyright (C) 2017 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "../../plugins/server/dfmplugin-interfaces/dbus_adaptor/devicemanagerdbus_adaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class DeviceManagerAdaptor
 */

DeviceManagerAdaptor::DeviceManagerAdaptor(DeviceManagerDBus *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

DeviceManagerAdaptor::~DeviceManagerAdaptor()
{
    // destructor
}

void DeviceManagerAdaptor::DetachAllMountedDevices()
{
    // handle method call com.deepin.filemanager.service.DeviceManager.DetachAllMountedDevices
    parent()->DetachAllMountedDevices();
}

void DeviceManagerAdaptor::DetachBlockDevice(const QString &id)
{
    // handle method call com.deepin.filemanager.service.DeviceManager.DetachBlockDevice
    parent()->DetachBlockDevice(id);
}

void DeviceManagerAdaptor::DetachProtocolDevice(const QString &id)
{
    // handle method call com.deepin.filemanager.service.DeviceManager.DetachProtocolDevice
    parent()->DetachProtocolDevice(id);
}

void DeviceManagerAdaptor::EjectBlockDevice(const QString &id)
{
    // handle method call com.deepin.filemanager.service.DeviceManager.EjectBlockDevice
    parent()->EjectBlockDevice(id);
}

QStringList DeviceManagerAdaptor::GetBlockDevicesIdList(const QVariantMap &opts)
{
    // handle method call com.deepin.filemanager.service.DeviceManager.GetBlockDevicesIdList
    return parent()->GetBlockDevicesIdList(opts);
}

QStringList DeviceManagerAdaptor::GetProtolcolDevicesIdList()
{
    // handle method call com.deepin.filemanager.service.DeviceManager.GetProtolcolDevicesIdList
    return parent()->GetProtolcolDevicesIdList();
}

bool DeviceManagerAdaptor::IsMonotorWorking()
{
    // handle method call com.deepin.filemanager.service.DeviceManager.IsMonotorWorking
    return parent()->IsMonotorWorking();
}

void DeviceManagerAdaptor::MountBlockDevice(const QString &id)
{
    // handle method call com.deepin.filemanager.service.DeviceManager.MountBlockDevice
    parent()->MountBlockDevice(id);
}

void DeviceManagerAdaptor::MountProtocolDevice(const QString &id)
{
    // handle method call com.deepin.filemanager.service.DeviceManager.MountProtocolDevice
    parent()->MountProtocolDevice(id);
}

void DeviceManagerAdaptor::PoweroffBlockDevice(const QString &id)
{
    // handle method call com.deepin.filemanager.service.DeviceManager.PoweroffBlockDevice
    parent()->PoweroffBlockDevice(id);
}

QVariantMap DeviceManagerAdaptor::QueryBlockDeviceInfo(const QString &id)
{
    // handle method call com.deepin.filemanager.service.DeviceManager.QueryBlockDeviceInfo
    return parent()->QueryBlockDeviceInfo(id);
}

QVariantMap DeviceManagerAdaptor::QueryProtocolDeviceInfo(const QString &id)
{
    // handle method call com.deepin.filemanager.service.DeviceManager.QueryProtocolDeviceInfo
    return parent()->QueryProtocolDeviceInfo(id);
}

void DeviceManagerAdaptor::UnmountBlockDevice(const QString &id)
{
    // handle method call com.deepin.filemanager.service.DeviceManager.UnmountBlockDevice
    parent()->UnmountBlockDevice(id);
}

void DeviceManagerAdaptor::UnmountProtocolDevice(const QString &id)
{
    // handle method call com.deepin.filemanager.service.DeviceManager.UnmountProtocolDevice
    parent()->UnmountProtocolDevice(id);
}

