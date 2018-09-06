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
  $node = node_load($nid);
  $form_state['values']['nid'] = $nid;

  $layout_id = db_query('SELECT id FROM {node_layout} WHERE nid = :nid', array(':nid' => $nid))->fetchField();

  if ($layout_id) {
    $layout = entity_load('node_layout', $layout_id);
  }

  // We will have many fields with the same name, so we need to be able to
  // access the form hierarchically.
  $form['#tree'] = TRUE;

  // Create block.
  $form['add_block'] = array();

  // Boxes for each region.
  $regions = node_layout_get_active_regions($node->type);
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
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

function node_layout_edit_form_submit(&$form, &$form_state) {
  if (isset($form_state['_layout'])) {
    $entity = entity_load('node_layout', $form_state['_layout']->id);
    $entity->data = serialize($form_state['values']['regions']);
  }
  else {
    $values = array();
    $values['nid'] = $form_state['_nid'];
    $values['layout'] = $form_state['values']['regions'];
    $entity = entity_create('node_layout', $values);
  }

  try {
    $entity->save();
  } catch (EntityStorageException $e) {
    watchdog('node_layout', $e->getMessage());
  }
}


function node_layout_settings_form($form = array(), &$form_state) {
  $form['#config'] = 'node_layout.settings';
  $config = config_get('node_layout.settings');

  // Get all node types excluding pageless ones.
  $available_node_types = array_filter(node_type_get_types(), function ($val) {
    return $val->settings['hidden_path'] === FALSE;
  });

  if (!$available_node_types) {
    return 'No available node types to select from.';
  }

  $form['node_types'] = array(
    '#type' => 'fieldset',
    '#title' => 'Available Node Types',
    '#description' => 'Select node types to configure settings for them.'
  );

  $form['node_types']['available_node_types'] = array(
    '#type' => 'checkboxes',
    '#options' => array_combine(
      array_keys($available_node_types),
      array_keys($available_node_types)
    ),
    '#default_value' => $config['available_node_types'],
  );

  // Get the node layout.
  $node_layout = layout_load('default');

  // Get the template associated with the node layout.
  // the template has the regions that can be selected.
  $template = layout_get_layout_template_info($node_layout->layout_template);
  $template_regions = $template['regions'];

  foreach ($config['available_node_types'] as $type) {
    if ($type === 0) {
      continue;
    }

    $form[$type] = array(
      '#type' => 'fieldset',
      '#title' => $type,
    );
    $form[$type][$type . '_regions'] = array(
      '#type' => 'checkboxes',
      '#options' => $template_regions,
      '#default_value' => $config[$type . '_regions'],
    );
  }

  return system_settings_form($form);
}
