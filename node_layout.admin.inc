<?php

/**
 * @file
 * Admin forms for the node_layout module.
 */

/**
 * @param array $form
 * @param $form_state
 *
 * @return array
 */
function node_layout_edit_form($form = array(), &$form_state) {

  $nid = explode('/', current_path())[1];
  $form_state['values']['nid'] = $nid;

  $layout_id = db_query('SELECT id FROM {node_layout} WHERE nid = :nid', array(':nid' => $nid))->fetchField();

  if ($layout_id) {
    $layout = entity_load('node_layout', $layout_id);
  }

  $foo = 'bar';

  // We will have many fields with the same name, so we need to be able to
  // access the form hierarchically.
  $form['#tree'] = TRUE;

  // Create block.
  $form['add_block'] = array();

  // Boxes for each region.
  $regions = node_layout_get_active_regions();
  $form['regions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Regions:'),
    '#collapsible' => FALSE,
  );

  foreach ($regions as $region) {
    $form['regions'][$region] = array(
      '#type' => 'fieldset',
      '#title' => t('Region: @region', array('@region' => $region)),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['regions'][$region]['reference'] = array(
      '#type' => 'textfield',
      '#title' => t('Reference'),
      '#description' => t('Enter the box you want to add to the region.'),
      '#autocomplete_path' => 'node_layout_titles/autocomplete',
//      '#size' => 20,
//      '#maxlength' => 20,
      //      '#required' => TRUE,
    );

    // Adds "Add another name" button.
//    $form['add_name'] = array(
//      '#type' => 'submit',
//      '#value' => t('Add another name'),
//      '#submit' => array('form_example_tutorial_9_add_name'),
//    );
    // If we have more than one name, this button allows removal of the
    // last name.
//    if ($form_state['num_names'] > 1) {
//      $form['remove_name'] = array(
//        '#type' => 'submit',
//        '#value' => t('Remove latest name'),
//        '#submit' => array('form_example_tutorial_9_remove_name'),
//        // Since we are removing a name, don't validate until later.
//        '#limit_validation_errors' => array(),
//      );
//    }
  }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Submit',
    );

    return $form;

}

function node_layout_edit_form_submit(&$form, &$form_state) {
  $values = array();

  $values['nid'] = $form_state['values']['nid'];
  $values['layout'] = $form_state['values']['regions'];

  $entity = entity_create('node_layout', $values);

  $foo = 'bar';
  try {
    $entity->save();
  } catch (EntityStorageException $e) {
    watchdog('node_layout', $e->getMessage());
  }
}

function form_example_tutorial_9_add_name($form, &$form_state) {
  // Everything in $form_state is persistent, so we'll just use
  // $form_state['add_name']
  $form_state['num_names']++;
  // Setting $form_state['rebuild'] = TRUE causes the form to be rebuilt again.
  $form_state['rebuild'] = TRUE;
}

function form_example_tutorial_9_remove_name($form, &$form_state) {
  if ($form_state['num_names'] > 1) {
    $form_state['num_names']--;
  }
  // Setting $form_state['rebuild'] = TRUE causes the form to be rebuilt again.
  $form_state['rebuild'] = TRUE;
}

function form_example_tutorial_9_validate($form, &$form_state) {
  for ($i = 1; $i <= $form_state['num_names']; $i++) {
    $year_of_birth = $form_state['values']['name'][$i]['year_of_birth'];
    if ($year_of_birth && ($year_of_birth < 1900 || $year_of_birth > 2000)) {
      form_set_error("name][$i][year_of_birth", t('Enter a year between 1900 and 2000.'));
    }
  }
}
