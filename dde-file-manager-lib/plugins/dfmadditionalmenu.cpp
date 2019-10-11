/*
 * Copyright (C) 2019 Deepin Technology Co., Ltd.
 *
 * Author:     Mike Chen <kegechen@gmail.com>
 *
 * Maintainer: Mike Chen <chenke_cm@deepin.com>
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
#include "dfmadditionalmenu.h"
#include "dfileservices.h"

#include <QDir>
#include <QMenu>
#include <QMimeDatabase>
#include <XdgDesktopFile>
DFM_BEGIN_NAMESPACE

class DFMAdditionalMenuPrivate : public QSharedData{
    Q_DECLARE_PUBLIC(DFMAdditionalMenu)
public:
    const QStringList AllMenuTypes {
        "SingleFile",
        "SingleDir",
        "MultiFileDirs",
        "EmptyArea"
    };

    const QLatin1String MENU_TYPE_KEY {"X-DFM-MenuTypes"};
    const QLatin1String MIME_TYPE_KEY {"X-DFM-MimeTypes"};
    const QLatin1String MIMETYPE_EXCLUDE_KEY {"X-DFM-ExcludeMimeTypes"};
    const QLatin1String MENU_HIDDEN_KEY {"X-DFM-NotShowIn"};     // "Desktop", "Filemanager"

    DFMAdditionalMenuPrivate(DFMAdditionalMenu *qq);

    QStringList getValues(XdgDesktopFile &file, const QLatin1String &key, const QStringList &whiteList={});
    bool isMimeTypeSupport(const QString &mt, const QStringList &fileMimeTypes);
    bool isMimeTypeMatch(const QStringList &fileMimeTypes, const QStringList &supportMimeTypes);
    bool isActionShouldShow(QAction *action, bool onDesktop);
    QList<QAction *> emptyAreaActoins(const QString &currentDir, bool onDesktop);
private:
    QList<QAction *> actionList;
    QMap<QString, QList<QAction *> > actionListByType;

    DFMAdditionalMenu *q_ptr;
};

DFMAdditionalMenuPrivate::DFMAdditionalMenuPrivate(DFMAdditionalMenu *qq)
    :q_ptr(qq)
{

}

QStringList DFMAdditionalMenuPrivate::getValues(XdgDesktopFile &file, const QLatin1String &key, const QStringList &whiteList)
{
    QStringList values;
    if (file.contains(key)) {
        values = file.value(key).toString().split(';', QString::SkipEmptyParts);
        for (const QString &one : values) {
            if (whiteList.size() > 0 && !whiteList.contains(one)) {
                values.removeAll(one);
            }
        }
    } else {
        values = whiteList;
    }
    return values;
}

bool DFMAdditionalMenuPrivate::isMimeTypeSupport(const QString &mt, const QStringList &fileMimeTypes)
{
    foreach(const QString &fmt, fileMimeTypes){
        if (fmt.contains(mt, Qt::CaseInsensitive)) {
            return true;
        }
    }
    return false;
}

bool DFMAdditionalMenuPrivate::isMimeTypeMatch(const QStringList &fileMimeTypes, const QStringList &supportMimeTypes)
{
    bool match = false;
    for (QString mt : supportMimeTypes) {
        if (fileMimeTypes.contains(mt, Qt::CaseInsensitive)) {
            match = true;
            break;
        }

        int starPos = mt.indexOf("*");
        if (starPos >=0 && isMimeTypeSupport(mt.left(starPos), fileMimeTypes)) {
            match = true;
            break;
        }
    }
    return match;
}

bool DFMAdditionalMenuPrivate::isActionShouldShow(QAction *action, bool onDesktop)
{
    if (!action) {
        return false;
    }

    // X-DFM-NotShowIn not exist
    if (action->property(MENU_HIDDEN_KEY.data()).isNull()) {
        return true;
    }

    // is menu triggered on desktop
    QStringList notShowInList =  action->property(MENU_HIDDEN_KEY.data()).toStringList();
    return (onDesktop  && !notShowInList.contains("Desktop", Qt::CaseInsensitive)) ||
            (!onDesktop && !notShowInList.contains("Filemanager", Qt::CaseInsensitive));
}

QList<QAction *>DFMAdditionalMenuPrivate::emptyAreaActoins(const QString &currentDir, bool onDesktop)
{
    QString menuType = "EmptyArea";
    QList<QAction *> actions = actionListByType[menuType];
    for (auto it = actions.begin(); it != actions.end(); ) {
        QAction * action = *it;
        if(!action || !isActionShouldShow(action, onDesktop)) {
            it = actions.erase(it);
            continue;
        }

        // Add file list data.
        action->setData(currentDir);
        if (action->menu()) {
            for (QAction * subAction : action->menu()->actions()) {
                subAction->setData(currentDir);
            }
        }

        ++it;
    }

    return actions;
}

DFMAdditionalMenu::DFMAdditionalMenu(QObject *parent)
    : QObject(parent)
    , d_private(new DFMAdditionalMenuPrivate(this))
{
    Q_D(DFMAdditionalMenu);
    QDir oemPath("/usr/share/deepin/dde-file-manager/oem-menuextensions/");
    if (oemPath.exists()) {
        for (const QFileInfo &fileInfo : oemPath.entryInfoList({"*.desktop"})) {
            XdgDesktopFile file;
            file.load(fileInfo.filePath());
            if (!file.isValid()) {
                continue;
            }

            QStringList menuTypes = d->getValues(file, d->MENU_TYPE_KEY, d->AllMenuTypes);
            // MIMETYPE_EXCLUDE_KEY  "X-DFM-ExcludeMimeTypes"
            QStringList excludeMimeTypes = d->getValues(file, d->MIMETYPE_EXCLUDE_KEY);
            // MENU_HIDDEN_KEY  = "X-DFM-NotShowIn"
            QStringList notShowInList = d->getValues(file, d->MENU_HIDDEN_KEY);

            if (menuTypes.isEmpty()) {
                qDebug() << "[OEM Menu Support] Entry will probably not be shown due to empty or have no valid"
                         << d->MENU_TYPE_KEY << "key in the desktop file.";
                qDebug() << "[OEM Menu Support] Details:" << fileInfo.filePath() << "with entry name" << file.name();
            }

            // the XdgDesktopFile::icon() empty fallback is not really an empty fallback, so we need to check it manually.
            QString iconStr = file.localizedValue("Icon").toString();
            QAction *action = new QAction(iconStr.isEmpty() ? QIcon() : file.icon(), file.name());
            QStringList entryActionList = file.actions();
            if (!entryActionList.isEmpty()) {
                QMenu *menu = new QMenu();
                for (const QString &actionName : entryActionList) {
                    QAction *subAction = new QAction(file.actionIcon(actionName), file.actionName(actionName));
                    connect(subAction, &QAction::triggered, this, [subAction, actionName, file](){
                        QStringList files = subAction->data().toStringList();
                        file.actionActivate(actionName, files);
                    });
                    menu->addAction(subAction);
                }
                action->setMenu(menu);
            }

            connect(action, &QAction::triggered, this, [action, file](){
                QStringList files = action->data().toStringList();
                file.startDetached(files);
            });

            d->actionList.append(action);
            if (file.contains("MimeType")) {
                action->setProperty(d->MIME_TYPE_KEY.data(), file.mimeTypes());
            }
            if (file.contains(d->MIMETYPE_EXCLUDE_KEY)) {
                action->setProperty(d->MIMETYPE_EXCLUDE_KEY.data(), excludeMimeTypes);
            }
            if (file.contains(d->MENU_HIDDEN_KEY)) {
                action->setProperty(d->MENU_HIDDEN_KEY.data(), notShowInList);
            }

            for (const QString &oneType : menuTypes) {
                d->actionListByType[oneType].append(action);
            }
        }
    }
}

DFMAdditionalMenu::~DFMAdditionalMenu()
{

}

void DFMAdditionalMenu::appendParentMineType(const QStringList &parentmimeTypes,  QStringList& mimeTypes)
{
    if (parentmimeTypes.size()==0)
        return;

    for (const QString &mtName : parentmimeTypes) {
        QMimeDatabase db;
        QMimeType mt = db.mimeTypeForName(mtName);
        mimeTypes.append(mt.name());
        mimeTypes.append(mt.aliases());
        QStringList pmts = mt.parentMimeTypes();
        appendParentMineType(pmts, mimeTypes);
    }
}

QList<QAction *> DFMAdditionalMenu::actions(const QStringList &files, const QString &currentDir, bool onDesktop/* = false*/)
{
    Q_UNUSED(currentDir);
    Q_D(DFMAdditionalMenu);
    QString menuType = "EmptyArea";
    if (files.size() == 0) {
        return d->emptyAreaActoins(currentDir, onDesktop);
    }

    if (files.count() == 1) {
        menuType = QFileInfo(DUrl(files.first()).toLocalFile()).isDir() ? "SingleDir" : "SingleFile";
    } else {
        menuType = "MultiFileDirs";
    }
    QList<QAction *> actions = d->actionListByType[menuType];

    for (const QString &f : files) {
        const DAbstractFileInfoPointer &fileInfo = DFileService::instance()->createFileInfo(this, DUrl(f));
        if (!fileInfo) {
            qWarning() << "createFileInfo failed: " << f;
            continue;
        }

        if (actions.size()==0) {
            break;
        }

        QStringList fileMimeTypes;
        fileMimeTypes.append(fileInfo->mimeType().name());
        fileMimeTypes.append(fileInfo->mimeType().aliases());
        const QMimeType &mt = fileInfo->mimeType();
        appendParentMineType(mt.parentMimeTypes(), fileMimeTypes);
        fileMimeTypes.removeAll({});

        for (auto it = actions.begin(); it != actions.end(); ) {
            QAction * action = *it;
            if(!action || !d->isActionShouldShow(action, onDesktop)) {
                it = actions.erase(it);
                continue;
            }

            // match exclude mime types
            QStringList excludeMimeTypes = action->property(d->MIMETYPE_EXCLUDE_KEY.data()).toStringList();
            excludeMimeTypes.removeAll({});
            bool match = d->isMimeTypeMatch(fileMimeTypes, excludeMimeTypes);
            if (match) {
                it = actions.erase(it);
                continue;
            }

            // MimeType not exist == MimeType=*
            if (action->property(d->MIME_TYPE_KEY.data()).isNull()) {
                ++it;
                continue;
            }

            // match support mime types
            QStringList supportMimeTypes =  action->property(d->MIME_TYPE_KEY.data()).toStringList();
            supportMimeTypes.removeAll({});
            match = d->isMimeTypeMatch(fileMimeTypes, supportMimeTypes);
            if (!match) {
                it = actions.erase(it);
                continue;
            }

            ++it;
        }
    }

    // Add file list data.
    for (QAction * action : actions) {
        action->setData(files);
        if (action->menu()) {
            for (QAction * subAction : action->menu()->actions()) {
                subAction->setData(files);
            }
        }
    }

    return actions;
}


DFM_END_NAMESPACE
