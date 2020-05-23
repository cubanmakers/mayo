/****************************************************************************
** Copyright (c) 2020, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once

#include "document.h"
#include <TDocStd_Application.hxx>
#include <QtCore/QFileInfo>
#include <atomic>
#include <unordered_map>

namespace Mayo {

class Application;
DEFINE_STANDARD_HANDLE(Application, TDocStd_Application)

using ApplicationPtr = opencascade::handle<Application>;

class Application : public QObject, public TDocStd_Application {
    Q_OBJECT
public:
    static ApplicationPtr instance();

    int documentCount() const;
    DocumentPtr newDocument(Document::Format docFormat = Document::Format::Binary);
    DocumentPtr openDocument(const QString& filePath, PCDM_ReaderStatus* ptrReadStatus = nullptr);
    DocumentPtr findDocumentByIndex(int docIndex) const;
    DocumentPtr findDocumentByIdentifier(Document::Identifier docIdent) const;
    DocumentPtr findDocumentByLocation(const QFileInfo& loc) const;

public: //  from TDocStd_Application
    void NewDocument(
            const TCollection_ExtendedString& format,
            opencascade::handle<TDocStd_Document>& outDoc) override;
    void InitDocument(const opencascade::handle<TDocStd_Document>& doc) const override;

// TODO: Redefine TDocStd_Document::BeforeClose() to emit signal documentClosed
// class Document : public TDocStd_Document { ... };
// using DocumentPtr = Handle_Document
// -> Can't do because PCDM_RetrievalDriver subclasses create explicitly "new TDocStd_Document(...)"
//    This would break TDocStd_Application::Open(...)

    DEFINE_STANDARD_RTTI_INLINE(Application, TDocStd_Application)

signals:
    void documentAdded(const DocumentPtr& doc);
    void documentAboutToClose(const DocumentPtr& doc);

private: // Implementation
    friend class Document;

    Application();
    void notifyDocumentAboutToClose(Document::Identifier docIdent);
    void addDocument(const DocumentPtr& doc);

    std::atomic<Document::Identifier> m_seqDocumentIdentifier = {};
    std::unordered_map<Document::Identifier, DocumentPtr> m_mapIdentifierDocument;
};

} // namespace Mayo
