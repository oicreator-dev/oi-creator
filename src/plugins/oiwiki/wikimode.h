#pragma once

#include <coreplugin/imode.h>
#include <QMap>
class QStackedWidget;
class QWebEngineView;
class QTreeWidget;
class QHBoxLayout;
class QTreeWidgetItem;
class QString;
class QNetworkAccessManager;
namespace OIWiki {
class WikiMode : public Core::IMode
{
    Q_OBJECT
public:
    WikiMode();
    ~WikiMode();
    static void createModeIfRequired();
    static void destroyModeIfRequired();
private:
    void initIndexTree(QString file);
    QStackedWidget *m_stackWidget;
    QWidget *m_mainWidget;
    QWebEngineView *m_view;
    QTreeWidget *m_indexTree;
    QHBoxLayout *m_layout;
    QMap<QTreeWidgetItem*,QString> m_treeItemMap;
    QNetworkAccessManager *m_networkManager;
};
}
