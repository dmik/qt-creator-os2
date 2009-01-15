/***************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact:  Qt Software Information (qt-info@nokia.com)
**
**
** Non-Open Source Usage
**
** Licensees may use this file in accordance with the Qt Beta Version
** License Agreement, Agreement version 2.2 provided with the Software or,
** alternatively, in accordance with the terms contained in a written
** agreement between you and Nokia.
**
** GNU General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU General
** Public License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the packaging
** of this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
**
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt GPL Exception
** version 1.3, included in the file GPL_EXCEPTION.txt in this package.
**
***************************************************************************/

#ifndef BASEFILEWIZARD_H
#define BASEFILEWIZARD_H

#include "core_global.h"

#include <coreplugin/dialogs/iwizard.h>

#include <QtGui/QIcon>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QMap>
#include <QtCore/QList>

QT_BEGIN_NAMESPACE
class QWizard;
class QWizardPage;
QT_END_NAMESPACE

namespace Core {

class ICore;
class IEditor;
class IFileWizardExtension;

class BaseFileWizardParameterData;
struct BaseFileWizardPrivate;
class GeneratedFilePrivate;

/*!
 * Represents a file generated by a wizard. The Wizard class will check for
 * each file whether it already exists and will report any errors that may
 * occur during creation of the files.
 */
class CORE_EXPORT GeneratedFile
{
public:
    GeneratedFile();
    explicit GeneratedFile(const QString &path);
    GeneratedFile(const GeneratedFile &);
    GeneratedFile &operator=(const GeneratedFile&);
    ~GeneratedFile();

    // Full path of the file should be created, or the suggested file name
    QString path() const;
    void setPath(const QString &p);

    // Contents of the file
    QString contents() const;
    void setContents(const QString &c);

    // Kind of editor to open the file with
    QString editorKind() const;
    void setEditorKind(const QString &k);

    bool write(QString *errorMessage) const;

private:
    QSharedDataPointer<GeneratedFilePrivate> m_d;
};

typedef QList<GeneratedFile> GeneratedFiles;

/* Parameter class for passing parameters to instances of class Wizard
 * containing name, icon and such. */

class CORE_EXPORT BaseFileWizardParameters
{
public:
    explicit BaseFileWizardParameters(IWizard::Kind kind = IWizard::FileWizard);
    BaseFileWizardParameters(const BaseFileWizardParameters &);
    BaseFileWizardParameters &operator=(const BaseFileWizardParameters&);
   ~BaseFileWizardParameters();

    IWizard::Kind kind() const;
    void setKind(IWizard::Kind k);

    QIcon icon() const;
    void setIcon(const QIcon&);

    QString description() const;
    void setDescription(const QString &);

    QString name() const;
    void setName(const QString &);

    QString category() const;
    void setCategory(const QString &);

    QString trCategory() const;
    void setTrCategory(const QString &);

private:
    QSharedDataPointer<BaseFileWizardParameterData> m_d;
};

/* A generic wizard for creating files.
 *
 * The abstract methods:
 *
 * createWizardDialog() : Called to create the QWizard dialog to be shown
 * generateFiles()      : Generate file content
 *
 * must be implemented. The behaviour can be further customized by overwriting
 * the virtual method:
 * postGenerateFiles()  :   Called after generating the files.
 */

class CORE_EXPORT BaseFileWizard : public IWizard
{
    Q_DISABLE_COPY(BaseFileWizard)
    Q_OBJECT

public:
    virtual ~BaseFileWizard();

    // IWizard
    virtual Kind kind() const;
    virtual QIcon icon() const;
    virtual QString description() const;
    virtual QString name() const;

    virtual QString category() const;
    virtual QString trCategory() const;

    virtual QStringList runWizard(const QString &path, QWidget *parent);

    // Utility to find all registered wizards
    static QList<IWizard*> allWizards();
    // Utility to find all registered wizards of a certain kind
    static QList<IWizard*> findWizardsOfKind(Kind kind);

    // Build a file name, adding the extension unless baseName already has one
    static QString buildFileName(const QString &path, const QString &baseName, const QString &extension);

    // Return standard pixmap to be used as watermark
    static QPixmap watermark();
    // Set the standard watermark on a QWizard
    static void setupWizard(QWizard *);

protected:
    typedef QList<QWizardPage *> WizardPageList;

    explicit BaseFileWizard(const BaseFileWizardParameters &parameters, Core::ICore *core, QObject *parent = 0);

    // Overwrite to create the wizard dialog on the parent, adding
    // the extension pages.
    virtual QWizard *createWizardDialog(QWidget *parent,
                                        const QString &defaultPath,
                                        const WizardPageList &extensionPages) const = 0;

    // Overwrite to query the parameters from the dialog and generate the files.
    virtual GeneratedFiles generateFiles(const QWizard *w,
                                         QString *errorMessage) const = 0;

    /* Overwrite to perform steps to be done after files are actually created.
     * The default implementation opens editors with the newly generated files. */
    virtual bool postGenerateFiles(const GeneratedFiles &l, QString *errorMessage);

    // Utility that returns the preferred suffix for a mime type
    QString preferredSuffix(const QString &mimeType) const;

    // Utility that performs an overwrite check on a set of files. It checks if
    // the file exists, can be overwritten at all and prompts the user.
    enum OverwriteResult { OverwriteOk,  OverwriteError,  OverwriteCanceled };
    OverwriteResult promptOverwrite(const QString &location,
                                    const QStringList &files,
                                    QString *errorMessage) const;
    Core::ICore *core() const;

private:
    BaseFileWizardPrivate *m_d;
};

// StandardFileWizard convenience class for creating one file. It uses
// Core::Utils::FileWizardDialog and introduces a new virtual to generate the
// files from path and name.

class CORE_EXPORT StandardFileWizard : public BaseFileWizard
{
    Q_DISABLE_COPY(StandardFileWizard)
    Q_OBJECT

protected:
    explicit StandardFileWizard(const BaseFileWizardParameters &parameters, Core::ICore *core, QObject *parent = 0);

    // Implemented to create a Core::Utils::FileWizardDialog
    virtual QWizard *createWizardDialog(QWidget *parent,
                                        const QString &defaultPath,
                                        const WizardPageList &extensionPages) const;
    // Implemented to retrieve path and name and call generateFilesFromPath()
    virtual GeneratedFiles generateFiles(const QWizard *w,
                                         QString *errorMessage) const;

    // Newly introduced virtual that creates a file from a path
    virtual GeneratedFiles generateFilesFromPath(const QString &path,
                                                 const QString &name,
                                                 QString *errorMessage) const = 0;
};

/* A utility to find all wizards supporting a view mode and matching a predicate */
template <class Predicate>
    QList<IWizard*> findWizards(Predicate predicate)
{
    // Filter all wizards
    const QList<IWizard*> allWizards = BaseFileWizard::allWizards();
    QList<IWizard*> rc;
    const QList<IWizard*>::const_iterator cend = allWizards.constEnd();
    for (QList<IWizard*>::const_iterator it = allWizards.constBegin(); it != cend; ++it)
        if (predicate(*(*it)))
            rc.push_back(*it);
    return rc;
}

} // namespace Core

#endif // BASEFILEWIZARD_H
