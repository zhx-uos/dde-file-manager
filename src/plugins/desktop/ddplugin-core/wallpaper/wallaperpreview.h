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
#ifndef WALLAPERPREVIEW_H
#define WALLAPERPREVIEW_H

#include "dfm_desktop_service_global.h"
#include "background/backgrounddefault.h"
#include "screen/screenservice.h"

#include <com_deepin_wm.h>

#include <QObject>
#include <QMap>

DSB_D_BEGIN_NAMESPACE
using WMInter = com::deepin::wm;
class PreviewWidget : public BackgroundDefault
{
    Q_OBJECT
public:
    using BackgroundDefault::BackgroundDefault;
};

typedef QSharedPointer<PreviewWidget> PreviewWidgetPtr;

class WallaperPreview : public QObject
{
    Q_OBJECT
public:
    explicit WallaperPreview(ScreenService *service, QObject *parent = nullptr);
    ~WallaperPreview();
    void init();
    void setVisible(bool visible);
    bool isVisible() const {
        return visible;
    }
    void pullImageSettings();
    void setWallpaper(const QString &screen, const QString &image);
    inline QMap<QString, QString> wallpaper() const {
        return wallpapers;
    }
    inline PreviewWidgetPtr widget(const QString &screen) const {
        return previewWidgets.value(screen);
    }
signals:

public slots:
    void buildWidgets();
    void updateGeometry();
    void updateWallpaper();
protected:
    PreviewWidgetPtr createWidget(dfmbase::ScreenPointer);
    QString getBackgroundFromWm(const QString &screen);
private:
    bool visible = false;
    ScreenService *screenService = nullptr;
    WMInter *wmInter = nullptr;
    QMap<QString, QString> wallpapers;
    QMap<QString, PreviewWidgetPtr> previewWidgets;
};

DSB_D_END_NAMESPACE

#endif // WALLAPERPREVIEW_H
