/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/

#ifndef S60CREATEPACKAGESTEP_H
#define S60CREATEPACKAGESTEP_H

#include "ui_s60createpackagestep.h"

#include <projectexplorer/buildstep.h>
#include <qt4projectmanager/makestep.h>

#include <QMutex>
#include <QWaitCondition>

class QSettings;

namespace Qt4ProjectManager {
namespace Internal {

class S60CreatePackageStepFactory : public ProjectExplorer::IBuildStepFactory
{
    Q_OBJECT
public:
    explicit S60CreatePackageStepFactory(QObject *parent = 0);
    ~S60CreatePackageStepFactory();

    // used to show the list of possible additons to a target, returns a list of types
    QStringList availableCreationIds(ProjectExplorer::BuildConfiguration *parent, ProjectExplorer::BuildStep::Type type) const;
    // used to translate the types to names to display to the user
    QString displayNameForId(const QString &id) const;

    bool canCreate(ProjectExplorer::BuildConfiguration *parent, ProjectExplorer::BuildStep::Type type, const QString &id) const;
    ProjectExplorer::BuildStep *create(ProjectExplorer::BuildConfiguration *parent, ProjectExplorer::BuildStep::Type type, const QString &id);
    // used to recreate the runConfigurations when restoring settings
    bool canRestore(ProjectExplorer::BuildConfiguration *parent, ProjectExplorer::BuildStep::Type type, const QVariantMap &map) const;
    ProjectExplorer::BuildStep *restore(ProjectExplorer::BuildConfiguration *parent, ProjectExplorer::BuildStep::Type type, const QVariantMap &map);
    bool canClone(ProjectExplorer::BuildConfiguration *parent, ProjectExplorer::BuildStep::Type type, ProjectExplorer::BuildStep *product) const;
    ProjectExplorer::BuildStep *clone(ProjectExplorer::BuildConfiguration *parent, ProjectExplorer::BuildStep::Type type, ProjectExplorer::BuildStep *product);
};


class S60CreatePackageStep : public ProjectExplorer::BuildStep
{
    Q_OBJECT
    friend class S60CreatePackageStepFactory;
public:
    enum SigningMode {
        SignSelf = 0,
        SignCustom = 1
    };

    explicit S60CreatePackageStep(ProjectExplorer::BuildConfiguration *bc);
    virtual ~S60CreatePackageStep();

    virtual bool init();
    virtual void run(QFutureInterface<bool> &fi);
    virtual ProjectExplorer::BuildStepConfigWidget *createConfigWidget();
    virtual bool immutable() const;

    QVariantMap toMap() const;

    SigningMode signingMode() const;
    void setSigningMode(SigningMode mode);
    QString customSignaturePath() const;
    void setCustomSignaturePath(const QString &path);
    QString customKeyPath() const;
    void setCustomKeyPath(const QString &path);
    QString passphrase() const   ;
    void setPassphrase(const QString &passphrase);
    QString keyId() const;
    void setKeyId(const QString &keyId);
    bool createsSmartInstaller() const;
    void setCreatesSmartInstaller(bool value);

    void resetPassphrases();

signals:
    void badPassphrase();

protected:
    S60CreatePackageStep(ProjectExplorer::BuildConfiguration *bc, S60CreatePackageStep *bs);
    S60CreatePackageStep(ProjectExplorer::BuildConfiguration *bc, const QString &id);
    bool fromMap(const QVariantMap &map);

    Qt4BuildConfiguration *qt4BuildConfiguration() const;

private slots:
    void slotProcessFinished(int, QProcess::ExitStatus);
    void processReadyReadStdOutput();
    void processReadyReadStdError();
    void taskAdded(const ProjectExplorer::Task &task);
    void outputAdded(const QString &string, ProjectExplorer::BuildStep::OutputFormat format);
    void checkForCancel();
    void definePassphrase();

private:
    enum ErrorType {
        ErrorNone = 0,
        ErrorUndefined,
        ErrorBadPassphrase
    };

    void stdOutput(const QString &line);
    void stdError(const QString &line);
    bool startProcess();

    QString generateKeyId(const QString &keyPath) const;
    QString loadPassphraseForKey(const QString &keyId);
    void savePassphraseForKey(const QString &keyId, const QString &passphrase);
    QString elucidatePassphrase(QByteArray obfuscatedPassphrase, const QString &key) const;
    QByteArray obfuscatePassphrase(const QString &passphrase, const QString &key) const;

    QStringList m_workingDirectories;

    QString m_makeCmd;
    ProjectExplorer::Environment m_environment;
    QStringList m_args;

    void ctor_package();

    SigningMode m_signingMode;
    QString m_customSignaturePath;
    QString m_customKeyPath;
    QString m_passphrase;
    QString m_keyId;
    bool m_createSmartInstaller;
    ProjectExplorer::IOutputParser *m_outputParserChain;

    QProcess *m_process;
    QTimer *m_timer;
    QEventLoop *m_eventLoop;
    QFutureInterface<bool> *m_futureInterface;
    ErrorType m_errorType;

    QWaitCondition m_waitCondition;
    QMutex m_mutex;

    QSettings *m_settings;
};

class S60CreatePackageStepConfigWidget : public ProjectExplorer::BuildStepConfigWidget
{
    Q_OBJECT
public:
    S60CreatePackageStepConfigWidget(S60CreatePackageStep *signStep);
    QString displayName() const;
    void init();
    QString summaryText() const;

private slots:
    void updateUi();
    void updateFromUi();
    void resetPassphrases();

private:
    S60CreatePackageStep *m_signStep;

    Ui::S60CreatePackageStepWidget m_ui;
};

} // Internal
} // Qt4ProjectManager

#endif // S60CREATEPACKAGESTEP_H
