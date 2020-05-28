/****************************************************************************
** Copyright (c) 2020, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#include "widget_model_tree_builder_mesh.h"
#include "../base/caf_utils.h"
#include "theme.h"
#include "widget_model_tree.h"

#include <QtWidgets/QTreeWidgetItem>
#include <TDataXtd_Triangulation.hxx>

namespace Mayo {

bool WidgetModelTreeBuilder_Mesh::supportsEntity(const DocumentTreeNode& node) const
{
    return CafUtils::hasAttribute<TDataXtd_Triangulation>(node.label());
}

void WidgetModelTreeBuilder_Mesh::fillTreeItem(QTreeWidgetItem* treeItem, const DocumentTreeNode& node)
{
    WidgetModelTreeBuilder::fillTreeItem(treeItem, node);
    Expects(this->supportsEntity(node));
    treeItem->setIcon(0, mayoTheme()->icon(Theme::Icon::ItemMesh));
}

WidgetModelTreeBuilder* WidgetModelTreeBuilder_Mesh::clone() const
{
    return new WidgetModelTreeBuilder_Mesh;
}

} // namespace Mayo
