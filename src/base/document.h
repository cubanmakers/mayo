/****************************************************************************
** Copyright (c) 2020, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once

#include "libtree.h"
#include "quantity.h"
#include <QtCore/QObject>
#include <TDocStd_Document.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>

namespace Mayo {

class XCaf {
public:
    struct ValidationProperties {
        bool hasCentroid;
        bool hasArea;
        bool hasVolume;
        gp_Pnt centroid;
        QuantityArea area;
        QuantityVolume volume;
    };

    bool isNull() const { return m_shapeTool.IsNull(); }

    const Handle_XCAFDoc_ShapeTool& shapeTool() const { return m_shapeTool; }
    const Handle_XCAFDoc_ColorTool& colorTool() const { return m_colorTool; }

    void rebuildAssemblyTree();
    const Tree<TDF_Label>& assemblyTree() const { return m_asmTree; }

    TDF_LabelSequence topLevelFreeShapes() const;
    static TDF_LabelSequence shapeComponents(const TDF_Label& lbl);
    static TDF_LabelSequence shapeSubs(const TDF_Label& lbl);

    static TopoDS_Shape shape(const TDF_Label& lbl);
    static bool isShape(const TDF_Label& lbl);
    static bool isShapeFree(const TDF_Label& lbl);
    static bool isShapeAssembly(const TDF_Label& lbl);
    static bool isShapeReference(const TDF_Label& lbl);
    static bool isShapeSimple(const TDF_Label& lbl);
    static bool isShapeComponent(const TDF_Label& lbl);
    static bool isShapeCompound(const TDF_Label& lbl);
    static bool isShapeSub(const TDF_Label& lbl);

    bool hasShapeColor(const TDF_Label& lbl) const;
    Quantity_Color shapeColor(const TDF_Label& lbl) const;

    TopLoc_Location shapeAbsoluteLocation(TreeNodeId nodeId) const;
    static TopLoc_Location shapeReferenceLocation(const TDF_Label& lbl);
    static TDF_Label shapeReferred(const TDF_Label& lbl);

    static ValidationProperties validationProperties(const TDF_Label& lbl);

private:
    void deepBuildAssemblyTree(TreeNodeId parentNode, const TDF_Label& label);

    friend class Document;
    Handle_XCAFDoc_ShapeTool m_shapeTool;
    Handle_XCAFDoc_ColorTool m_colorTool;
    Tree<TDF_Label> m_asmTree;
};


class Application;
class Document;
DEFINE_STANDARD_HANDLE(Document, TDocStd_Document)

using DocumentPtr = opencascade::handle<Document>;

class Document : public QObject, public TDocStd_Document {
    Q_OBJECT
    Q_PROPERTY(int identifier READ identifier)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(bool isXCafDocument READ isXCafDocument)
public:
    using Identifier = int;
    enum class Format {
        Binary,
        Xml
    };

    Identifier identifier() const { return m_identifier; }

    QString name() const;
    void setName(const QString& v);

    QString filePath() const;
    void setFilePath(const QString& filepath);

    static const char NameFormatBinary[];
    static const char NameFormatXml[];
    static const char* toNameFormat(Format format);

    static const char TypeName[];
    virtual const char* dynTypeName() const;

    static QString findLabelName(const TDF_Label& lbl);
    static void setLabelName(const TDF_Label& lbl, const QString& name);

    bool isXCafDocument() const;
    XCaf& xcaf() { return m_xcaf; }
    const XCaf& xcaf() const { return m_xcaf; }

    static DocumentPtr findFrom(const TDF_Label& label);

public: // -- from TDocStd_Document
    void BeforeClose() override;
    void ChangeStorageFormat(const TCollection_ExtendedString& newStorageFormat) override;

    DEFINE_STANDARD_RTTI_INLINE(Document, TDocStd_Document)

signals:
    void entityAdded(const TDF_Label& labelEntity);
//    void itemAdded(DocumentItem* docItem);
//    void itemErased(const DocumentItem* docItem);
//    void itemPropertyChanged(DocumentItem* docItem, Property* prop);

private:
    friend class Application;
    class FormatBinaryRetrievalDriver;
    class FormatXmlRetrievalDriver;

    Document();
    void initXCaf();
    void setIdentifier(Identifier ident) { m_identifier = ident; }

    Identifier m_identifier = -1;
    QString m_name;
    QString m_filePath;

    XCaf m_xcaf;
};

struct DocumentTreeNode {
    DocumentTreeNode() = default;
    DocumentTreeNode(const DocumentPtr& docPtr, TreeNodeId nodeId);

    bool isValid() const;
    static const DocumentTreeNode& null();

    TDF_Label label() const;

    DocumentPtr document;
    TreeNodeId id = 0;
};

} // namespace Mayo
