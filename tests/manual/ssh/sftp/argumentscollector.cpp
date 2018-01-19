/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "argumentscollector.h"

#include <iostream>

using namespace QSsh;

using namespace std;

ArgumentsCollector::ArgumentsCollector(const QStringList &args)
    : m_arguments(args)
{
}

Parameters ArgumentsCollector::collect(bool &success) const
{
    Parameters parameters;
    parameters.sshParams.options &= ~SshIgnoreDefaultProxy;
    try {
        bool authTypeGiven = false;
        bool portGiven = false;
        bool timeoutGiven = false;
        bool smallFileCountGiven = false;
        bool bigFileSizeGiven = false;
        bool proxySettingGiven = false;
        int pos;
        int port;
        for (pos = 1; pos < m_arguments.count() - 1; ++pos) {
            QString host;
            QString user;
            if (checkAndSetStringArg(pos, host, "-h") || checkAndSetStringArg(pos, user, "-u")) {
                parameters.sshParams.setHost(host);
                parameters.sshParams.setUserName(user);
                continue;
            }
            if (checkAndSetIntArg(pos, port, portGiven, "-p")
                || checkAndSetIntArg(pos, parameters.sshParams.timeout, timeoutGiven, "-t")
                || checkAndSetIntArg(pos, parameters.smallFileCount, smallFileCountGiven, "-c")
                || checkAndSetIntArg(pos, parameters.bigFileSize, bigFileSizeGiven, "-s"))
                continue;
            QString pass;
            if (checkAndSetStringArg(pos, pass, "-pwd")) {
                parameters.sshParams.setPassword(pass);
                if (!parameters.sshParams.privateKeyFile.isEmpty())
                    throw ArgumentErrorException(QLatin1String("-pwd and -k are mutually exclusive."));
                parameters.sshParams.authenticationType
                    = SshConnectionParameters::AuthenticationTypeTryAllPasswordBasedMethods;
                authTypeGiven = true;
                continue;
            }
            if (checkAndSetStringArg(pos, parameters.sshParams.privateKeyFile, "-k")) {
                if (!parameters.sshParams.password().isEmpty())
                    throw ArgumentErrorException(QLatin1String("-pwd and -k are mutually exclusive."));
                parameters.sshParams.authenticationType
                    = SshConnectionParameters::AuthenticationTypePublicKey;
                authTypeGiven = true;
                continue;
            }
            if (!checkForNoProxy(pos, parameters.sshParams.options, proxySettingGiven))
                throw ArgumentErrorException(QLatin1String("unknown option ") + m_arguments.at(pos));
        }

        Q_ASSERT(pos <= m_arguments.count());
        if (pos == m_arguments.count() - 1) {
            if (!checkForNoProxy(pos, parameters.sshParams.options, proxySettingGiven))
                throw ArgumentErrorException(QLatin1String("unknown option ") + m_arguments.at(pos));
        }

        if (!authTypeGiven)
            throw ArgumentErrorException(QLatin1String("No authentication argument given."));
        if (parameters.sshParams.host().isEmpty())
            throw ArgumentErrorException(QLatin1String("No host given."));
        if (parameters.sshParams.userName().isEmpty())
            throw ArgumentErrorException(QLatin1String("No user name given."));

        parameters.sshParams.setPort(portGiven ? port : 22);
        if (!timeoutGiven)
            parameters.sshParams.timeout = 30;
        if (!smallFileCountGiven)
            parameters.smallFileCount = 1000;
        if (!bigFileSizeGiven)
            parameters.bigFileSize = 1024;
        success = true;
    } catch (ArgumentErrorException &ex) {
        cerr << "Error: " << qPrintable(ex.error) << endl;
        printUsage();
        success = false;
    }
    return parameters;
}

void ArgumentsCollector::printUsage() const
{
    cerr << "Usage: " << qPrintable(m_arguments.first())
        << " -h <host> -u <user> "
        << "-pwd <password> | -k <private key file> [ -p <port> ] "
        << "[ -t <timeout> ] [ -c <small file count> ] "
        << "[ -s <big file size in MB> ] [ -no-proxy ]" << endl;
}

bool ArgumentsCollector::checkAndSetStringArg(int &pos, QString &arg, const char *opt) const
{
    if (m_arguments.at(pos) == QLatin1String(opt)) {
        if (!arg.isEmpty()) {
            throw ArgumentErrorException(QLatin1String("option ") + QLatin1String(opt)
                + QLatin1String(" was given twice."));
        }
        arg = m_arguments.at(++pos);
        if (arg.isEmpty() && QLatin1String(opt) != QLatin1String("-pwd"))
            throw ArgumentErrorException(QLatin1String("empty argument not allowed here."));
        return true;
    }
    return false;
}

bool ArgumentsCollector::checkAndSetIntArg(int &pos, int &val,
    bool &alreadyGiven, const char *opt) const
{
    if (m_arguments.at(pos) == QLatin1String(opt)) {
        if (alreadyGiven) {
            throw ArgumentErrorException(QLatin1String("option ") + QLatin1String(opt)
                + QLatin1String(" was given twice."));
        }
        bool isNumber;
        val = m_arguments.at(++pos).toInt(&isNumber);
        if (!isNumber) {
            throw ArgumentErrorException(QLatin1String("option ") + QLatin1String(opt)
                 + QLatin1String(" needs integer argument"));
        }
        alreadyGiven = true;
        return true;
    }
    return false;
}

bool ArgumentsCollector::checkForNoProxy(int &pos, SshConnectionOptions &options,
                                         bool &alreadyGiven) const
{
    if (m_arguments.at(pos) == QLatin1String("-no-proxy")) {
        if (alreadyGiven)
            throw ArgumentErrorException(QLatin1String("proxy setting given twice."));
        options |= SshIgnoreDefaultProxy;
        alreadyGiven = true;
        return true;
    }
    return false;
}
