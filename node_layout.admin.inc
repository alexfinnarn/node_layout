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
  backdrop_add_js('https://cdnjs.cloudflare.com/ajax/libs/vue/2.5.2/vue.min.js', 'external');
  backdrop_add_js('https://cdn.jsdelivr.net/npm/sortablejs@1.7.0/Sortable.min.js', 'external');
  backdrop_add_js('https://cdnjs.cloudflare.com/ajax/libs/Vue.Draggable/2.15.0/vuedraggable.min.js', 'external');

  $nid = explode('/', current_path())[1];
  $node = node_load($nid);
  $form_state['_nid'] = $nid;

  // Boxes for each region.
  $regions = node_layout_get_active_regions($node->type);
  $layout = node_layout_get_layout($nid);

  global $base_url;

  if ($layout->regions === false) {
    $layout->data = array(
      'baseURL' => $base_url,
      'nid' => $nid,
      'regions' => array_map(function($region) {
        return array(
          'name' => $region,
          'references' => array(
            array(
              'nid' => NULL,
              'title' => 'placeholder',
              'type' => NULL,
            ),
          ),
        );
      }, $regions),
    );
    $data = array(
      'encoded_layout' => json_encode($layout->data),
      'layout' => $layout,
    );
  } else {
    $decoded_data = unserialize($layout->data);
    $decoded_data->baseURL = $base_url;
    $data = array(
      'encoded_layout' => json_encode($decoded_data),
      'layout' => $layout,
    );
  }

  $form_state['_layout'] = $layout;

  $form['regions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Regions:'),
    '#collapsible' => FALSE,
  );

  $form['regions']['vue_form'] = array(
    '#markup' => theme('node_layout_edit_layout', array('data' => $data)),
  );

  $form['_final_layout'] = array(
    '#type' => 'hidden',
    '#default_value' => array(),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

function node_layout_edit_form_submit(&$form, &$form_state) {
  $submitted_layout = json_decode($form_state['values']['_final_layout']);

  // Remove other properties of references leaving only name.
  $arr = (array) $submitted_layout->regions;

  foreach(array_keys($arr) as $value) {
    foreach($submitted_layout->regions->$value->references as $index => $value2) {
      $submitted_layout->regions->$value->references[$index] = array(
        'title' => $value2->name,
        'nid' => $value2->nid,
        'type' => $value2->type,
      );
    }
  }

  if ($form_state['_layout']->regions !== FALSE) {
    $entity = entity_load('node_layout', $form_state['_layout']->id);
    $entity->data = serialize($submitted_layout);
  }
  else {
    $values = array();
    $values['nid'] = $form_state['_nid'];
    $values['layout'] = $submitted_layout;
    $entity = entity_create('node_layout', $values);
  }

  try {
    $entity->save();
  } catch (EntityStorageException $e) {
    watchdog('node_layout', $e->getMessage());
  }
}

function node_layout_edit_access_callback() {
  // @todo Add user access check.

  $nid = explode('/', current_path())[1];
  $node = node_load($nid);
  if (isset($node->type)) {
    return node_layout_is_type_available($node->type);
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
    '#description' => 'Select node types to configure settings for them.',
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
