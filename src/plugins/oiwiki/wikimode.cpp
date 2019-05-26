#include "wikimode.h"

#include <extensionsystem/pluginmanager.h>

#include <QStackedWidget>
#include <QWebEngineView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QWebEngineProfile>
#include <QWebEngineScriptCollection>
#include <yaml-cpp/yaml.h>

namespace OIWiki {
static WikiMode *m_instance = nullptr;

WikiMode::WikiMode()
{
    setObjectName(QLatin1String("WikiMode"));
    setEnabled(true);
    setContext(Core::Context("WikiMode"));
    m_stackWidget = new QStackedWidget();
    m_mainWidget = new QWidget();
    setWidget(m_mainWidget);
    setDisplayName(tr("Wiki"));
    setPriority(88);
    setId("Wiki");

    m_layout = new QHBoxLayout(m_mainWidget);

    m_mainWidget->setLayout(m_layout);

    m_indexTree = new QTreeWidget();
    m_layout->addWidget(m_indexTree, 2);

    m_view = new QWebEngineView();
    m_layout->addWidget(m_view, 12);
    m_view->setUrl(QUrl("https://oi-wiki.org/"));

    QFile jsFile(":/js/modifypage.js");
    jsFile.open(QIODevice::ReadOnly);
    QWebEngineScript script;
    script.setSourceCode(QString::fromUtf8(jsFile.readAll()));

    QWebEngineProfile *profile = QWebEngineProfile::defaultProfile();
    profile->scripts()->insert(script);

    connect(m_indexTree, &QTreeWidget::itemDoubleClicked, this,
            [this](QTreeWidgetItem *item, int column) {
                Q_UNUSED(column)
                if (m_treeItemMap.count(item) > 0) {
                    QString url = m_treeItemMap[item];
                    if (url != "null") {
                        if (url.contains("index.md")) {
                            url = url.left(url.size() - 8);
                        } else {
                            url = url.left(url.size() - 3);
                        }
                        qDebug() << url;
                        m_view->setUrl(QUrl(QString("https://oi-wiki.org/") + url));
                    }
                }
            });
    m_networkManager = new QNetworkAccessManager(this);
    QUrl url("https://dev.tencent.com/u/scaffrey/p/OI-wiki/git/raw/master/mkdocs.yml");
    QNetworkReply *reply = m_networkManager->get(QNetworkRequest(QUrl(url)));
    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        auto error = reply->error();
        if (error != QNetworkReply::NoError) {
            qDebug() << error;
            return;
        }
        QString text = QString::fromUtf8(reply->readAll());
        this->initIndexTree(text);
    });
}

WikiMode::~WikiMode()
{
    delete m_stackWidget;
    delete m_mainWidget;
}

void WikiMode::createModeIfRequired()
{
    if (m_instance == nullptr) {
        m_instance = new WikiMode();
        ExtensionSystem::PluginManager::addObject(m_instance);
    }
}

void WikiMode::destroyModeIfRequired()
{
    if (m_instance != nullptr) {
        ExtensionSystem::PluginManager::removeObject(m_instance);
        delete m_instance;
        m_instance = nullptr;
    }
}

void WikiMode::initIndexTree(QString file)
{
    m_treeItemMap.clear();
    using namespace YAML;
    Node root = Load(file.toStdString())["nav"];
    if (!root.IsSequence()) {
        return;
    }
    auto index = new QTreeWidgetItem(m_indexTree);
    index->setText(0, tr("Index"));
    m_treeItemMap.insert(index, "null");
    auto f = [this](auto &&self, QTreeWidgetItem *parent, Node node) -> void {
        for (auto i : node) {
            auto it = i.begin();
            std::string key = it->first.as<std::string>();
            auto item = new QTreeWidgetItem(parent);
            item->setText(0, QString::fromStdString(key));
            Node value = it->second;
            if (value.IsScalar()) {
                auto url = value.as<std::string>();
                m_treeItemMap.insert(item, QString::fromStdString(url));
            } else if (value.IsSequence()) {
                item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                m_treeItemMap.insert(item, "null");
                self(self, item, value);
            }
        }
    };
    f(f, index, root);
}
}
