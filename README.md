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

6. Create some content with the "hidden path" content type so you can use it in the layout tool. We'll call them "blocks" from now on.
7. Create a node that has the node layout functionality enabled and go to the `node/%/layout` path to create your node layout.

<img width="1018" alt="screen shot 2018-10-03 at 3 48 24 pm" src="https://user-images.githubusercontent.com/3640707/46443784-cf452c80-c723-11e8-949a-08a781970fbb.png">

## Creating A Block

The Node Layout module allows you to create blocks via the layout interface. It is currently done via an iframe implemention but will be switched to loading the form via AJAX with a smoother UX to navigate away from the created node.

<img width="1062" alt="screen shot 2018-10-04 at 1 43 51 pm" src="https://user-images.githubusercontent.com/3640707/46501889-9ec0c980-c7db-11e8-8495-818f740d67ad.png">

Simply click "Create Block" to show a listing of the block types you can choose to create content for. You will create content with exactly the same form as you see in the normal `node/add` UI, except the modal will only include the form needed and not the theme of the site...eventually.

<img width="1031" alt="screen shot 2018-10-04 at 1 44 57 pm" src="https://user-images.githubusercontent.com/3640707/46501948-c31ca600-c7db-11e8-9f7f-9b59e260ecfe.png">

Once you are done creating content, close the modal and the layout data will be refreshed to allow you to add the newly create block to a region via the search input.

## Editing Blocks

Once you add a block to a region, you might want to edit it and the layout interface allows you to do this without having to switch screens and lose context.

<img width="565" alt="screen shot 2018-10-04 at 1 49 07 pm" src="https://user-images.githubusercontent.com/3640707/46502205-738aaa00-c7dc-11e8-8edf-270fe215b795.png">

Click "Edit" on any block to see the `node/edit` form for that hidden path node. This is loaded in an iframe for now but will use AJAX in the future. Once you save your edit, close the modal to complete the edit. 

## Feature Roadmap

There isn't really a roadmap for this module yet since it is in heavy development, but some features have been discussed in addition to cleaning up the code and removing the iframes for modals.

- **Remove CSS and make Themeable** - Currently, the Bootstap CSS framework is used to provide basic theming, but to fit into existing admin themes, the classes used will need to be configurable. Also, the regions should visually reflect the what the final output looks like at least in rows and column widths.

- **Add A Preview In-line** - Just looking at a title/label of a block might not give you a good idea of what it includes. When adding a block or looking at what's currently in a region, the rendered nodes can be inserted just like they are on `node/%/view` for the layout.

- **Create A Listing Feature** - A user might want to see a list of where a specific type of block is used on any given site. There is a table in the database to store references when editing a layout, but nothing has been implemented yet. Some sort of cache invalidation strategy could be derived from that listing feature.

