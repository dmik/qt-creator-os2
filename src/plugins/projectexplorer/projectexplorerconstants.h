/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (info@qt.nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at info@qt.nokia.com.
**
**************************************************************************/

#ifndef PROJECTEXPLORERCONSTANTS_H
#define PROJECTEXPLORERCONSTANTS_H

#include <QtCore/QtGlobal>

namespace ProjectExplorer {
namespace Constants {

// modes and their priorities
const char * const MODE_SESSION         = "Project";
const int          P_MODE_SESSION       = 85;

// actions
const char * const NEWSESSION           = "ProjectExplorer.NewSession";
const char * const NEWPROJECT           = "ProjectExplorer.NewProject";
const char * const LOAD                 = "ProjectExplorer.Load";
const char * const UNLOAD               = "ProjectExplorer.Unload";
const char * const CLEARSESSION         = "ProjectExplorer.ClearSession";
const char * const BUILDPROJECTONLY     = "ProjectExplorer.BuildProjectOnly";
const char * const BUILD                = "ProjectExplorer.Build";
const char * const BUILDCM              = "ProjectExplorer.BuildCM";
const char * const BUILDSESSION         = "ProjectExplorer.BuildSession";
const char * const REBUILDPROJECTONLY   = "ProjectExplorer.RebuildProjectOnly";
const char * const REBUILD              = "ProjectExplorer.Rebuild";
const char * const REBUILDCM            = "ProjectExplorer.RebuildCM";
const char * const REBUILDSESSION       = "ProjectExplorer.RebuildSession";
const char * const DEPLOYPROJECTONLY    = "ProjectExplorer.DeployProjectOnly";
const char * const DEPLOY               = "ProjectExplorer.Deploy";
const char * const DEPLOYCM             = "ProjectExplorer.DeployCM";
const char * const DEPLOYSESSION        = "ProjectExplorer.DeploySession";
const char * const PUBLISH              = "ProjectExplorer.Publish";
const char * const CLEANPROJECTONLY     = "ProjectExplorer.CleanProjectOnly";
const char * const CLEAN                = "ProjectExplorer.Clean";
const char * const CLEANCM              = "ProjectExplorer.CleanCM";
const char * const CLEANSESSION         = "ProjectExplorer.CleanSession";
const char * const CANCELBUILD          = "ProjectExplorer.CancelBuild";
const char * const RUN                  = "ProjectExplorer.Run";
const char * const RUNCONTEXTMENU       = "ProjectExplorer.RunContextMenu";
const char * const STOP                 = "ProjectExplorer.Stop";
const char * const ADDNEWFILE           = "ProjectExplorer.AddNewFile";
const char * const ADDEXISTINGFILES     = "ProjectExplorer.AddExistingFiles";
const char * const ADDNEWSUBPROJECT     = "ProjectExplorer.AddNewSubproject";
const char * const REMOVEPROJECT        = "ProjectExplorer.RemoveProject";
const char * const OPENFILE             = "ProjectExplorer.OpenFile";
const char * const SHOWINGRAPHICALSHELL = "ProjectExplorer.ShowInGraphicalShell";
const char * const OPENTERMIANLHERE     = "ProjectExplorer.OpenTerminalHere";
const char * const REMOVEFILE           = "ProjectExplorer.RemoveFile";
const char * const DELETEFILE           = "ProjectExplorer.DeleteFile";
const char * const RENAMEFILE           = "ProjectExplorer.RenameFile";
const char * const SETSTARTUP           = "ProjectExplorer.SetStartup";
const char * const PROJECTTREE_COLLAPSE_ALL = "ProjectExplorer.CollapseAll";

const char * const SHOW_TASK_IN_EDITOR  = "ProjectExplorer.ShowTaskInEditor";
const char * const VCS_ANNOTATE_TASK    = "ProjectExplorer.VcsAnnotateTask";
const char * const SHOW_TASK_OUTPUT     = "ProjectExplorer.ShowTaskOutput";

// Run modes
const char * const RUNMODE              = "ProjectExplorer.RunMode";

const char * const SELECTTARGET         = "ProjectExplorer.SelectTarget";


// action priorities
const int          P_ACTION_RUN            = 100;
const int          P_ACTION_BUILDPROJECT   = 80;

// context
const char * const C_PROJECTEXPLORER    = "Project Explorer";
const char * const C_PROJECT_TREE       = "ProjectExplorer.ProjectTreeContext";
const char * const C_APP_OUTPUT         = "ProjectExplorer.ApplicationOutput";
const char * const C_COMPILE_OUTPUT     = "ProjectExplorer.CompileOutput";

// languages
const char * const LANG_CXX             = "CXX";
const char * const LANG_QMLJS           = "QMLJS";

// menus
const char * const M_RECENTPROJECTS     = "ProjectExplorer.Menu.Recent";
const char * const M_BUILDPROJECT       = "ProjectExplorer.Menu.Build";
const char * const M_DEBUG              = "ProjectExplorer.Menu.Debug";
const char * const M_DEBUG_STARTDEBUGGING = "ProjectExplorer.Menu.Debug.StartDebugging";
const char * const M_SESSION            = "ProjectExplorer.Menu.Session";

// menu groups
const char * const G_BUILD_SESSION      = "ProjectExplorer.Group.BuildSession";
const char * const G_BUILD_PROJECT      = "ProjectExplorer.Group.Build";
const char * const G_BUILD_OTHER        = "ProjectExplorer.Group.Other";
const char * const G_BUILD_RUN          = "ProjectExplorer.Group.Run";
const char * const G_BUILD_CANCEL       = "ProjectExplorer.Group.BuildCancel";

// context menus
const char * const M_SESSIONCONTEXT     = "Project.Menu.Session";
const char * const M_PROJECTCONTEXT     = "Project.Menu.Project";
const char * const M_SUBPROJECTCONTEXT  = "Project.Menu.SubProject";
const char * const M_FOLDERCONTEXT      = "Project.Menu.Folder";
const char * const M_FILECONTEXT        = "Project.Menu.File";
const char * const M_OPENFILEWITHCONTEXT = "Project.Menu.File.OpenWith";

// context menu groups
const char * const G_SESSION_BUILD      = "Session.Group.Build";
const char * const G_SESSION_FILES      = "Session.Group.Files";
const char * const G_SESSION_OTHER      = "Session.Group.Other";
const char * const G_SESSION_CONFIG     = "Session.Group.Config";

const char * const G_PROJECT_FIRST      = "Project.Group.Open";
const char * const G_PROJECT_BUILD      = "Project.Group.Build";
const char * const G_PROJECT_RUN        = "Project.Group.Run";
const char * const G_PROJECT_FILES      = "Project.Group.Files";
const char * const G_PROJECT_TREE       = "Project.Group.Tree";
const char * const G_PROJECT_LAST       = "Project.Group.Last";

const char * const G_FOLDER_FILES       = "ProjectFolder.Group.Files";
const char * const G_FOLDER_OTHER       = "ProjectFolder.Group.Other";
const char * const G_FOLDER_CONFIG      = "ProjectFolder.Group.Config";

const char * const G_FILE_OPEN          = "ProjectFile.Group.Open";
const char * const G_FILE_OTHER         = "ProjectFile.Group.Other";
const char * const G_FILE_CONFIG        = "ProjectFile.Group.Config";

const char * const RUNMENUCONTEXTMENU   = "Project.RunMenu";

// file id
const char * const FILE_FACTORY_ID      = "ProjectExplorer.FileFactoryId";

// icons
const char * const ICON_BUILD           = ":/projectexplorer/images/build.png";
const char * const ICON_BUILD_SMALL     = ":/projectexplorer/images/build_small.png";
const char * const ICON_CLEAN           = ":/projectexplorer/images/clean.png";
const char * const ICON_CLEAN_SMALL     = ":/projectexplorer/images/clean_small.png";
const char * const ICON_REBUILD         = ":/projectexplorer/images/rebuild.png";
const char * const ICON_REBUILD_SMALL   = ":/projectexplorer/images/rebuild_small.png";
const char * const ICON_RUN             = ":/projectexplorer/images/run.png";
const char * const ICON_RUN_SMALL       = ":/projectexplorer/images/run_small.png";
const char * const ICON_DEBUG_SMALL     = ":/projectexplorer/images/debugger_start_small.png";
const char * const ICON_STOP            = ":/projectexplorer/images/stop.png";
const char * const ICON_STOP_SMALL      = ":/projectexplorer/images/stop_small.png";
const char * const ICON_TOOLCHAIN_SETTINGS_CATEGORY = ":projectexplorer/images/build.png"; // FIXME: Need an icon!
const char * const ICON_WINDOW          = ":/projectexplorer/images/window.png";

const char * const TASK_BUILD           = "ProjectExplorer.Task.Build";

const char * const C_SOURCE_MIMETYPE = "text/x-csrc";
const char * const C_HEADER_MIMETYPE = "text/x-chdr";
const char * const CPP_SOURCE_MIMETYPE = "text/x-c++src";
const char * const CPP_HEADER_MIMETYPE = "text/x-c++hdr";
const char * const FORM_MIMETYPE = "application/x-designer";
const char * const QML_MIMETYPE = "application/x-qml";
const char * const RESOURCE_MIMETYPE = "application/vnd.nokia.xml.qt.resource";

// settings page
const char * const PROJECTEXPLORER_SETTINGS_CATEGORY  = "K.ProjectExplorer";
const char * const PROJECTEXPLORER_SETTINGS_TR_CATEGORY = QT_TRANSLATE_NOOP("ProjectExplorer", "Projects");
const char * const PROJECTEXPLORER_SETTINGS_CATEGORY_ICON  = ":/core/images/category_project.png";
const char * const PROJECTEXPLORER_SETTINGS_ID = "ProjectExplorer.ProjectExplorer";
const char * const TOOLCHAIN_SETTINGS_CATEGORY = "ProjectExplorer.Settings.ToolChains";
const char * const TOOLCHAIN_SETTINGS_PAGE_ID = "M.ProjectExplorer.ToolChainOptions";

// task categories
const char * const TASK_CATEGORY_COMPILE = "Task.Category.Compile";
const char * const TASK_CATEGORY_BUILDSYSTEM = "Task.Category.Buildsystem";

// Wizard category
const char * const PROJECT_WIZARD_CATEGORY = "I.Projects"; // (after Qt)
const char * const PROJECT_WIZARD_TR_SCOPE = "ProjectExplorer";
const char * const PROJECT_WIZARD_TR_CATEGORY = QT_TRANSLATE_NOOP("ProjectExplorer", "Other Project");

// Build step lists ids:
const char * const BUILDSTEPS_CLEAN = "ProjectExplorer.BuildSteps.Clean";
const char * const BUILDSTEPS_BUILD = "ProjectExplorer.BuildSteps.Build";
const char * const BUILDSTEPS_DEPLOY = "ProjectExplorer.BuildSteps.Deploy";

// Deploy Configuration id:
const char * const DEFAULT_DEPLOYCONFIGURATION_ID = "ProjectExplorer.DefaultDeployConfiguration";

// ToolChain Ids
const char * const CLANG_TOOLCHAIN_ID = "ProjectExplorer.ToolChain.Clang";
const char * const GCC_TOOLCHAIN_ID = "ProjectExplorer.ToolChain.Gcc";
const char * const LINUXICC_TOOLCHAIN_ID = "ProjectExplorer.ToolChain.LinuxIcc";
const char * const MINGW_TOOLCHAIN_ID = "ProjectExplorer.ToolChain.Mingw";
const char * const MSVC_TOOLCHAIN_ID = "ProjectExplorer.ToolChain.Msvc";

// Run Configuration defaults:
const int QML_DEFAULT_DEBUG_SERVER_PORT = 3768;

// Default directory to run custom (build) commands in.
const char * const DEFAULT_WORKING_DIR = "%{buildDir}";

} // namespace Constants
} // namespace ProjectExplorer

#endif // PROJECTEXPLORERCONSTANTS_H
