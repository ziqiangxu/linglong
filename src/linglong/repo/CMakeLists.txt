# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: LGPL-3.0-or-later

linglong_add_library(
  repo
  SOURCES
  ostree_repo.cpp
  ostree_repohelper.cpp
  repo.cpp
  repo_client.cpp
  vfs_repo.cpp
  HEADERS
  ostree_repo.h
  ostree_repohelper.h
  repo.h
  repo_client.h
  vfs_repo.h
  DEPENDENCIES
  Qt::Core
  Qt::DBus
  Qt::Network
  PkgConfig::glib2
  PkgConfig::ostree1
  linglong_util
)
