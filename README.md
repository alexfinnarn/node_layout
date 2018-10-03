## node_layout

Layouts per node for the Backdrop CMS. 

## Motivation

In Drupal 7, I was able to have a layout per node that I could edit via "node/%/layout". In Backdrop, I can make layouts for patterns of paths, e.g. "content/foo/*" but not per node. Adding one per node via the layout tool becomes unmanageable. The goal for this module is to provide a way for users to create a layout per node managed by the same tabs they are used to view/edit.

## Installation

1. Install the module like you would any Backdrop module. No content types will be selected on install to inherit node layouts.

2. You will need to go to `admin/config/content/node-layout` and select which node types can have layouts.
<img width="748" alt="screen shot 2018-10-03 at 3 28 24 pm" src="https://user-images.githubusercontent.com/3640707/46443501-b8520a80-c722-11e8-9ead-6168721e5717.png">

3. After you save the form, it will reload and you can select which regions the content type can use for layout placement.

<img width="879" alt="screen shot 2018-10-03 at 3 41 43 pm" src="https://user-images.githubusercontent.com/3640707/46443556-e8011280-c722-11e8-81fc-0da934cff136.png">

4. **Remove in future** You will need to add the layout blocks to the default layout, `admin/structure/layouts/manage/default`, in order for the placed blocks to show up when viewing nodes. This will be automated in the future with a layout per node type.

<img width="978" alt="screen shot 2018-10-03 at 3 43 01 pm" src="https://user-images.githubusercontent.com/3640707/46443594-1252d000-c723-11e8-89ae-615a30453faf.png">

5. Node Layout needs to have "hidden path" content types to be able to create and edit the blocks. You can create them at `admin/structure/types/add` and make sure that "Hide path display" is checked.

<img width="1147" alt="screen shot 2018-10-03 at 3 46 53 pm" src="https://user-images.githubusercontent.com/3640707/46443747-acb31380-c723-11e8-84c0-e5310e96240e.png">

6. Create some content with the "hidden path" content type so you cna use it in the layout tool.
7. Create a node that has the node layout functionality enabled and go to the `node/%/layout` path to create your node layout.

<img width="1018" alt="screen shot 2018-10-03 at 3 48 24 pm" src="https://user-images.githubusercontent.com/3640707/46443784-cf452c80-c723-11e8-949a-08a781970fbb.png">

