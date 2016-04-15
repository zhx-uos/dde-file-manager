#ifndef RECENTHISTORYMANAGER_H
#define RECENTHISTORYMANAGER_H

#include <QList>
#include <QDir>

#include "basemanager.h"
#include "abstractfilecontroller.h"

class AbstractFileInfo;

class RecentHistoryManager : public AbstractFileController, public BaseManager
{
    Q_OBJECT
public:
    explicit RecentHistoryManager(QObject *parent = 0);
    ~RecentHistoryManager();

    void load() Q_DECL_OVERRIDE;
    void save() Q_DECL_OVERRIDE;

    bool openFile(const DUrl &fileUrl, bool &accepted) const Q_DECL_OVERRIDE;
    const QList<AbstractFileInfo*> getChildren(const DUrl &fileUrl, QDir::Filters filter, bool &accepted) const Q_DECL_OVERRIDE;
    AbstractFileInfo *createFileInfo(const DUrl &fileUrl, bool &accepted) const Q_DECL_OVERRIDE;

private:
    void loadJson(const QJsonObject &json);
    void writeJson(QJsonObject &json);

private slots:
    void addOpenedFile(const DUrl &url);

private:
    QList<DUrl> openedFileList;
};

#endif // RECENTHISTORYMANAGER_H
