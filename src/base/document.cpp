/****************************************************************************
** Copyright (c) 2020, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#include "application.h"
#include "document.h"
#include "caf_utils.h"
#include <fougtools/occtools/qt_utils.h>
#include <TDataStd_Name.hxx>
#include <TDF_AttributeIterator.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_Area.hxx>
#include <XCAFDoc_Centroid.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_Volume.hxx>

namespace Mayo {

Document::Document()
    : QObject(nullptr),
      TDocStd_Document(NameFormatBinary)
{
}

void Document::initXCaf()
{
    m_xcaf.m_shapeTool = XCAFDoc_DocumentTool::ShapeTool(this->Main());
    m_xcaf.m_colorTool = XCAFDoc_DocumentTool::ColorTool(this->Main());
}

QString Document::name() const
{
    return m_name;
}

void Document::setName(const QString& v)
{
    m_name = v;
}

QString Document::filePath() const
{
    return m_filePath;
}

void Document::setFilePath(const QString& filepath)
{
    m_filePath = filepath;
}

const char* Document::toNameFormat(Document::Format format)
{
    switch (format) {
    case Format::Binary: return Document::NameFormatBinary;
    case Format::Xml: return Document::NameFormatXml;
    }

    return nullptr;
}

const char Document::NameFormatBinary[] = "BinDocMayo";
const char Document::NameFormatXml[] = "XmlDocMayo";

const char Document::TypeName[] = "Mayo::Document";
const char* Document::dynTypeName() const
{
    return Document::TypeName;
}

QString Document::findLabelName(const TDF_Label& lbl)
{
    QString name = CafUtils::labelAttrStdName(lbl);
    return name;
}

void Document::setLabelName(const TDF_Label &lbl, const QString &name)
{
    TDataStd_Name::Set(lbl, occ::QtUtils::toOccExtendedString(name));
}

bool Document::isXCafDocument() const
{
    return XCAFDoc_DocumentTool::IsXCAFDocument(this);
}

void Document::BeforeClose()
{
    TDocStd_Document::BeforeClose();
    Application::instance()->notifyDocumentAboutToClose(m_identifier);
}

void Document::ChangeStorageFormat(const TCollection_ExtendedString& newStorageFormat)
{
    // TODO: check format
    TDocStd_Document::ChangeStorageFormat(newStorageFormat);
}




void XCaf::rebuildAssemblyTree()
{
    m_asmTree.clear();
    for (const TDF_Label& rootLabel : this->topLevelFreeShapes())
        this->deepBuildAssemblyTree(0, rootLabel);
}

TDF_LabelSequence XCaf::topLevelFreeShapes() const
{
    TDF_LabelSequence seq;
    if (m_shapeTool)
        m_shapeTool->GetFreeShapes(seq);

    return seq;
}

TDF_LabelSequence XCaf::shapeComponents(const TDF_Label& lbl)
{
    TDF_LabelSequence seq;
    XCAFDoc_ShapeTool::GetComponents(lbl, seq);
    return seq;
}

TDF_LabelSequence XCaf::shapeSubs(const TDF_Label& lbl)
{
    TDF_LabelSequence seq;
    XCAFDoc_ShapeTool::GetSubShapes(lbl, seq);
    return seq;
}

bool XCaf::isShape(const TDF_Label& lbl)
{
    return XCAFDoc_ShapeTool::IsShape(lbl);
}

bool XCaf::isShapeFree(const TDF_Label& lbl)
{
    return XCAFDoc_ShapeTool::IsFree(lbl);
}

TopoDS_Shape XCaf::shape(const TDF_Label& lbl)
{
    return XCAFDoc_ShapeTool::GetShape(lbl);
}

//QString XCaf::findLabelName(const TDF_Label& lbl)
//{
//    QString name = CafUtils::labelAttrStdName(lbl);
//    if (name.isEmpty()) {
//        if (XCaf::isShape(lbl)) {
//            const TopoDS_Shape shape = XCaf::shape(lbl);
//            switch (shape.ShapeType()) {
//            case TopAbs_COMPOUND: name = "Compound"; break;
//            case TopAbs_COMPSOLID: name = "CompSolid"; break;
//            case TopAbs_SOLID: name = "Solid"; break;
//            case TopAbs_SHELL: name = "Shell"; break;
//            case TopAbs_FACE: name = "Face"; break;
//            case TopAbs_WIRE: name = "Wire"; break;
//            case TopAbs_EDGE: name = "Edge"; break;
//            case TopAbs_VERTEX: name = "Vertex"; break;
//            case TopAbs_SHAPE: name = "Shape"; break;
//            }
//            name = QString("%1 %2").arg(name).arg(lbl.Tag());
//        }
//        else {
//            name = QString("[[%1]]").arg(CafUtils::labelTag(lbl));
//        }
//    }

//    return name;
//}

//QString XCaf::findLabelName(TreeNodeId nodeId) const
//{
//    return XCaf::findLabelName(m_asmTree.nodeData(nodeId));
//}

//void XCaf::setLabelName(TreeNodeId nodeId, const QString& name)
//{
//    XCaf::setLabelName(m_asmTree.nodeData(nodeId), name);
//}

bool XCaf::isShapeAssembly(const TDF_Label& lbl)
{
    return XCAFDoc_ShapeTool::IsAssembly(lbl);
}

bool XCaf::isShapeReference(const TDF_Label& lbl)
{
    return XCAFDoc_ShapeTool::IsReference(lbl);
}

bool XCaf::isShapeSimple(const TDF_Label& lbl)
{
    return XCAFDoc_ShapeTool::IsSimpleShape(lbl);
}

bool XCaf::isShapeComponent(const TDF_Label& lbl)
{
    return XCAFDoc_ShapeTool::IsComponent(lbl);
}

bool XCaf::isShapeCompound(const TDF_Label& lbl)
{
    return XCAFDoc_ShapeTool::IsCompound(lbl);
}

bool XCaf::isShapeSub(const TDF_Label& lbl)
{
    return XCAFDoc_ShapeTool::IsSubShape(lbl);
}

bool XCaf::hasShapeColor(const TDF_Label& lbl) const
{
    if (!m_colorTool)
        return false;

    return m_colorTool->IsSet(lbl, XCAFDoc_ColorGen)
            || m_colorTool->IsSet(lbl, XCAFDoc_ColorSurf)
            || m_colorTool->IsSet(lbl, XCAFDoc_ColorCurv);
}

Quantity_Color XCaf::shapeColor(const TDF_Label& lbl) const
{
    Quantity_Color color = {};
    if (!m_colorTool)
        return color;

    if (m_colorTool->GetColor(lbl, XCAFDoc_ColorGen, color))
        return color;

    if (m_colorTool->GetColor(lbl, XCAFDoc_ColorSurf, color))
        return color;

    if (m_colorTool->GetColor(lbl, XCAFDoc_ColorCurv, color))
        return color;

    return color;
}

TopLoc_Location XCaf::shapeReferenceLocation(const TDF_Label& lbl)
{
    return XCAFDoc_ShapeTool::GetLocation(lbl);
}

TDF_Label XCaf::shapeReferred(const TDF_Label& lbl)
{
    TDF_Label referred;
    XCAFDoc_ShapeTool::GetReferredShape(lbl, referred);
    return referred;
}

TopLoc_Location XCaf::shapeAbsoluteLocation(TreeNodeId nodeId) const
{
    TopLoc_Location absoluteLoc;
    TreeNodeId it = nodeId;
    while (it != 0) {
        const TDF_Label& nodeLabel = m_asmTree.nodeData(it);
        const TopLoc_Location nodeLoc = XCaf::shapeReferenceLocation(nodeLabel);
        absoluteLoc = nodeLoc * absoluteLoc;
        it = m_asmTree.nodeParent(it);
    }

    return absoluteLoc;
}

XCaf::ValidationProperties XCaf::validationProperties(
        const TDF_Label& lbl)
{
    ValidationProperties props = {};
    for (TDF_AttributeIterator it(lbl); it.More(); it.Next()) {
        const Handle_TDF_Attribute ptrAttr = it.Value();
        const Standard_GUID& attrId = ptrAttr->ID();
        if (&attrId == &XCAFDoc_Centroid::GetID()) {
            const auto& centroid = static_cast<const XCAFDoc_Centroid&>(*ptrAttr);
            props.hasCentroid = true;
            props.centroid = centroid.Get();
        }
        else if (&attrId == &XCAFDoc_Area::GetID()) {
            const auto& area = static_cast<const XCAFDoc_Area&>(*ptrAttr);
            props.hasArea = true;
            props.area = area.Get() * Quantity_SquaredMillimeter;
        }
        else if (&attrId == &XCAFDoc_Volume::GetID()) {
            const auto& volume = static_cast<const XCAFDoc_Volume&>(*ptrAttr);
            props.hasVolume = true;
            props.volume = volume.Get() * Quantity_CubicMillimeter;
        }

        if (props.hasCentroid && props.hasArea && props.hasVolume)
            break;
    }

    return props;
}

} // namespace Mayo
