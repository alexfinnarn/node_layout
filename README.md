## node_layout

Layouts per node for the Backdrop CMS. 

## Motivation

In Drupal 7, I was able to have a layout per node that I could edit via "node/%/layout". In Backdrop, I can make layouts for patterns of paths, e.g. "content/foo/*" but not per node. Adding one per node via the layout tool becomes unmanageable. The goal for this module is to provide a way for users to create a layout per node managed by the same tabs they are used to view/edit.

## Installation

1. Install the module like you would any Backdrop module. No content types will be selected on install to inherit node layouts.
2. You will need to go to `admin/config/content/node-layout` and select which node types can have layouts.
3. After you save the form, it will reload and you can select which regions the content type can use for layout placement.
4. **Remove in future** You will need to add the layout blocks to the default layout, `admin/structure/layouts/manage/default`, in order for the placed blocks to show up when viewing nodes. This will be automated in the future with a layout per node type.
5. Create a node that has the node layout functionality enabled and go to the `node/%/layout` path to create your node layout.



