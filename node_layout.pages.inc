<?php

/**
 * @file
 * Page callbacks for adding, editing, deleting, and revisions management for
 * content.
 */

/**
 * Provides a listing of all embed types you can add.
 *
 * @return string
 *   Markup for list of embed types.
 */
function et_add_page() {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);


  // Bypass the listing if only one content type is available.
//  if (count($content) == 1) {
//    $item = array_shift($content);
//    drupal_goto($item['href']);
//  }

  return theme('node_add_list', array('content' => $content));
}
