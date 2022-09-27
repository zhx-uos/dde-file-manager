/*
 * Copyright (C) 2022 Uniontech Software Technology Co., Ltd.
 *
 * Author:     zhangyu<zhangyub@uniontech.com>
 *
 * Maintainer: zhangyu<zhangyub@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "canvasmanagerbroker.h"
#include "canvasmanager.h"
#include "model/fileinfomodel.h"
#include "view/canvasview.h"

#include <dfm-framework/dpf.h>

#include <QAbstractItemView>

using namespace ddplugin_canvas;

#define CanvasManagerSlot(topic, args...) \
            dpfSlotChannel->connect(QT_STRINGIFY(DDP_CANVAS_NAMESPACE), QT_STRINGIFY2(topic), this, ##args)

#define CanvasManagerDisconnect(topic) \
            dpfSlotChannel->disconnect(QT_STRINGIFY(DDP_CANVAS_NAMESPACE), QT_STRINGIFY2(topic))

CanvasManagerBroker::CanvasManagerBroker(CanvasManager *ptr, QObject *parent)
    : QObject(parent)
    , canvas(ptr)
{

}

CanvasManagerBroker::~CanvasManagerBroker()
{
    CanvasManagerDisconnect(slot_CanvasManager_FileInfoModel);
    CanvasManagerDisconnect(slot_CanvasManager_Update);
    CanvasManagerDisconnect(slot_CanvasManager_Edit);
    CanvasManagerDisconnect(slot_CanvasManager_IconLevel);
    CanvasManagerDisconnect(slot_CanvasManager_SetIconLevel);
    CanvasManagerDisconnect(slot_CanvasManager_AutoArrange);
    CanvasManagerDisconnect(slot_CanvasManager_SetAutoArrange);
    CanvasManagerDisconnect(slot_CanvasManager_View);
}

bool CanvasManagerBroker::init()
{
    CanvasManagerSlot(slot_CanvasManager_FileInfoModel, &CanvasManagerBroker::fileInfoModel);
    CanvasManagerSlot(slot_CanvasManager_Update, &CanvasManagerBroker::update);
    CanvasManagerSlot(slot_CanvasManager_Edit, &CanvasManagerBroker::edit);
    CanvasManagerSlot(slot_CanvasManager_IconLevel, &CanvasManagerBroker::iconLevel);
    CanvasManagerSlot(slot_CanvasManager_SetIconLevel, &CanvasManagerBroker::setIconLevel);
    CanvasManagerSlot(slot_CanvasManager_AutoArrange, &CanvasManagerBroker::autoArrange);
    CanvasManagerSlot(slot_CanvasManager_SetAutoArrange, &CanvasManagerBroker::setAutoArrange);
    CanvasManagerSlot(slot_CanvasManager_View, &CanvasManagerBroker::view);
    return true;
}

void CanvasManagerBroker::update()
{
    canvas->update();
}

void CanvasManagerBroker::edit(const QUrl &url)
{
    canvas->openEditor(url);
}

int CanvasManagerBroker::iconLevel()
{
    return canvas->iconLevel();
}

void CanvasManagerBroker::setIconLevel(int lv)
{
    canvas->setIconLevel(lv);
}

bool CanvasManagerBroker::autoArrange()
{
    return canvas->autoArrange();
}

void CanvasManagerBroker::setAutoArrange(bool on)
{
    canvas->setAutoArrange(on);
}

QAbstractItemModel *CanvasManagerBroker::fileInfoModel()
{
    return canvas->fileModel();
}

QAbstractItemView *CanvasManagerBroker::view(int viewIdx)
{
    auto views = canvas->views();
    if (viewIdx > 0 && viewIdx <= views.size())
        return views.at(viewIdx - 1).get();

    return nullptr;
}


