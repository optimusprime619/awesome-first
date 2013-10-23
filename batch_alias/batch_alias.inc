<?php

/**
 * @file
 * Additional functions for Admin configuration form for the Batch Alias.
 */

/**
 * Form builder for the alias settings option form
 */
function batch_alias_settings_form() {
  $aliasable_content_types = get_aliasable_content_type();
  $form = array();
  $form['batch_settings'] = array(
    '#title' => t('Batch Alias Settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#description' => t('Choose the desired batch alias settings for being used in Batch Aliasing'),
  );
  $alias_settings = &$form['batch_settings'];
  $alias_settings['node_settings'] = array(
      '#title' => t('Node Settings'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#description' => t('Choose the content types and settings for the batch URL aliasing process. 
                            If a content type is disabled it implies there are no nodes to alias in it.'),
  );
  if (count($aliasable_content_types)) {
    $content_types = array();
    foreach ($aliasable_content_types as $content_type => $content_name) {
      $content_types[] = $content_type;
      $alias_settings['node_settings'][$content_type] = array(
        '#type' => 'checkbox',
        '#title' => t('%type', array('%type' => $content_name)),
        '#disabled' => !(node_exists_for_type($content_type)),
        '#default_value' => variable_get('batch_alias_' . $content_type, 0),
      );
    }
  }
  else {
    $alias_settings['node_settings']['#collapsible'] = FALSE;
    $alias_settings['node_settings']['node_display_message'] = array(
      '#markup' => '<b>' . t('No Node settings available') . '</b>',
    );
    drupal_set_message(t('It appears that your content types do not have an alias pattern set or there might be no content types at all. 
                    Please check your content type settings in !content_type_link and URL alias settings in !alias_settings_link', 
                    array('!content_type_link' => l(t('Content Types'), 'admin/structure/types') , 
                    '!alias_settings_link' => l(t('URL Patterns'), 'admin/config/search/path/patterns'))), 
                    'warning');
    return $form;
  }

  $form['batch_settings']['all_content_types'] = array(
    '#type' => 'hidden',
    '#value' => $content_types,
  );
  
  $alias_settings['user_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Alias Settings'),
    '#collapsible' => TRUE,
    '#description' => t('Configure settings for User Aliasing'),
  );
  $alias_settings['user_settings']['user_alias_option'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Batch aliasing of users'),
    '#default_value' => variable_get('batch_alias_users', 0),
  );
  $form['#submit'][] = 'batch_alias_settings_form_submit';
  return system_settings_form($form);
}

/**
 * Submission handler for the batch alias settings form
 * @see batch_alias_settings_form().
 */
function batch_alias_settings_form_submit($form, &$form_state) {
  $submitted_settings = $form_state['values'];
  $content_type_info = $submitted_settings['all_content_types'];
  if (count($content_type_info)) {
    foreach ($content_type_info as $content_type_name) {
      variable_set('batch_alias_' . $content_type_name, $submitted_settings[$content_type_name]);
    }
  }
  variable_set('batch_alias_users', $submitted_settings['user_alias_option']);
}

/**
 * Form builder to get user confirmation to start running the node aliasing with Batch API
 */
function run_batch_aliasing_form() {
  $form = array();
  $form['#submit'][] = 'run_batch_alias';
  $question = t("Are you sure you want to run Batch aliasing of content?");
  $description = t("This might take long and depends on the quantity of content in your site and your URL alias settings. This action cannot be undone.");
  $path = "admin/config/development/batch_alias";
  return confirm_form($form, $question, $path, $description, $yes = "Confirm", $no = "Cancel", $name = "batch_alias_confirm");
}

/**
 * Function to trigger the batch aliasing process
 */
function run_batch_alias() {
  $aliasible_node_types = get_aliasable_content_type();
  $operations = array();
  foreach ($aliasible_node_types as $type => $name) {
    if (variable_get('batch_alias_' . $type, 0)) {
      $total = db_query("SELECT COUNT(nid) FROM {node} WHERE `type` = :type AND status = :status", array(":type" => $type, ":status" => 1))->fetchField();
      if ($total) {
        $operations[] = array('batch_alias_generate_node_alias', array($type, $name, $total));
      }
    }
  }
  if (variable_get('pathauto_user_pattern', '')) {
    $users_count = db_query("SELECT COUNT(uid) FROM {users} WHERE `uid` > 0")->fetchField();
    $operations[] = array('batch_alias_generate_user_alias', array($users_count));
  }
  
  $batch = array(
    'init_message' => t('Starting alias generation'),
    'title' => t('Content Aliasing'),
    'operations' => $operations,
    'finished' => 'batch_alias_complete',
  );
  batch_set($batch);
}
