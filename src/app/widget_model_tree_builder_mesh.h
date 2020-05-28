/****************************************************************************
** Copyright (c) 2020, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once

#include "widget_model_tree_builder.h"

namespace Mayo {

class WidgetModelTreeBuilder_Mesh : public WidgetModelTreeBuilder {
public:
    bool supportsEntity(const DocumentTreeNode& node) const override;
    void fillTreeItem(QTreeWidgetItem* treeItem, const DocumentTreeNode& node) override;

    WidgetModelTreeBuilder* clone() const override;
};

} // namespace Mayo
