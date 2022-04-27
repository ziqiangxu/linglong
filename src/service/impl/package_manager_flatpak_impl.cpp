/*
 * Copyright (c) 2020-2021. Uniontech Software Ltd. All rights reserved.
 *
 * Author:     huqinghong <huqinghong@uniontech.com>
 *
 * Maintainer: huqinghong <huqinghong@uniontech.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "package_manager_flatpak_impl.h"

linglong::service::Reply
PackageManagerFlatpakImpl::Install(const linglong::service::InstallParamOption &installParamOption)
{
    linglong::service::Reply reply;
    QString appId = installParamOption.appId.trimmed();
    QStringList argStrList;
    argStrList << "install"
               << "--user"
               << "-y" << appId;
    auto ret = linglong::runner::Runner("flatpak", argStrList, 1000 * 60 * 30);
    if (!ret) {
        reply.code = STATUS_CODE(pkg_install_failed);
        reply.message = "install " + appId + " failed";
    } else {
        reply.code = STATUS_CODE(pkg_install_success);
        reply.message = "install " + appId + " success";
    }
    qInfo() << "flatpak Install " << appId << ", ret:" << ret;
    return reply;
}

/*
 * 查询软件包
 *
 * @param paramOption: 查询参数
 *
 * @return QueryReply dbus方法调用应答
 */
linglong::service::QueryReply PackageManagerFlatpakImpl::Query(const linglong::service::QueryParamOption &paramOption)
{
    linglong::service::QueryReply reply;
    QString appId = paramOption.appId.trimmed();
    if (appId.isNull() || appId.isEmpty()) {
        reply.code = STATUS_CODE(user_input_param_err);
        reply.message = "appId input err";
        qCritical() << reply.message;
        return reply;
    }

    QProcess proc;
    QStringList argStrList;
    QJsonObject appItem;
    if ("installed" == appId) {
        argStrList << "list";
        appItem["appId"] = "flatpaklist";
    } else {
        argStrList << "search" << appId;
        appItem["appId"] = "flatpakquery";
    }
    proc.start("flatpak", argStrList);
    if (!proc.waitForStarted()) {
        reply.code = STATUS_CODE(ErrorPkgQueryFailed);
        reply.message = "flatpak not installed";
        qCritical() << reply.message;
        return reply;
    }
    proc.waitForFinished(1000 * 60 * 5);
    QString ret = proc.readAllStandardOutput();
    auto retCode = proc.exitCode();
    qInfo() << "flatpak query " << appId << ", exitStatus:" << proc.exitStatus() << ", retCode:" << retCode;
    QJsonArray appList;
    appItem["description"] = ret;
    appList.append(appItem);
    QJsonDocument document = QJsonDocument(appList);
    reply.code = STATUS_CODE(ErrorPkgQuerySuccess);
    reply.message = "flatpak query " + appId + " success";
    reply.result = QString(document.toJson());
    return reply;
}

/*
 * 卸载软件包
 *
 * @param paramOption: 卸载命令参数
 *
 * @return linglong::service::Reply 卸载结果信息
 */
linglong::service::Reply
PackageManagerFlatpakImpl::Uninstall(const linglong::service::UninstallParamOption &paramOption)
{
    linglong::service::Reply reply;
    QString appId = paramOption.appId.trimmed();
    QStringList argStrList;
    argStrList << "uninstall"
               << "--user"
               << "-y" << appId;
    auto ret = linglong::runner::Runner("flatpak", argStrList, 1000 * 60 * 30);
    if (!ret) {
        reply.code = STATUS_CODE(pkg_uninstall_failed);
        reply.message = "uninstall " + appId + " failed";
    } else {
        reply.code = STATUS_CODE(pkg_uninstall_success);
        reply.message = "uninstall " + appId + " success";
    }
    qInfo() << "flatpak Uninstall " << appId << ", ret:" << ret;
    return reply;
}