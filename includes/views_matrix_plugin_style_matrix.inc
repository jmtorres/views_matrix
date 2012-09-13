<?php
/**
 * @file
 *   Class definition for views matrix plugin.
 */

/**
 * @class
 *   Views Plugin Class
 */
class views_matrix_plugin_style_matrix extends views_plugin_style {

  public function option_definition() {
    $options = parent::option_definition();

    $options['sticky'] = array('default' => FALSE);
    $options['xconfig'] = array('default' => array());
    $options['yconfig'] = array('default' => array());

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $field_options = $this->display->handler->get_field_labels(TRUE); 

    $form['xconfig'] = array(
      '#type' => 'fieldset',
      '#title' => t('X Field'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
    );

    $form['xconfig']['xfield'] = array(
      '#type' => 'select',
      '#title' => t('X Field'),
      '#description' => t('Select the field that will group results by X'),
      '#options' => $field_options,
      '#default_value' => $this->options['xconfig']['xfield'],
    );

    $form['xconfig']['xsort'] = array(
      '#type' => 'select',
      '#title' => t('Sort by'),
      '#description' => t('Select the method to sort the field by.'),
      '#options' => array('asc' => t('Ascending'), 'dsc' => t('Descending')),
      '#default_value' => $this->options['xconfig']['xsort'],
    );

    $form['yconfig'] = array(
      '#type' => 'fieldset',
      '#title' => t('Y Field'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
    );

    $form['yconfig']['yfield'] = array(
      '#type' => 'select',
      '#title' => t('Y Field'),
      '#description' => t('Select the field that will group results by Y'),
      '#options' => $field_options,
      '#default_value' => $this->options['yconfig']['yfield'],
    );

    $form['yconfig']['ysort'] = array(
      '#type' => 'select',
      '#title' => t('Sort by'),
      '#description' => t('Select the method to sort the field by.'),
      '#options' => array('asc' => t('Ascending'), 'dsc' => t('Descending')),
      '#default_value' => $this->options['yconfig']['ysort'],
    );
  }

  /**
   * Validate that the view can be saved.
   */
  function validate() {
    $errors = parent::validate();

    if (empty($this->options['xconfig']['xfield']) || empty($this->options['yconfig']['yfield'])) {
      // Do not save without x field or y field.
      $errors[] = t('Style @style requires an x and y field to be defined.', array('@style' => $this->definition['title']));
    }
    else {
      // Check if fields exist still.
      $options = $this->display->handler->get_field_labels(TRUE);

      if (!isset($options[$this->options['xconfig']['xfield']]) || !isset($options[$this->options['yconfig']['yfield']])) {
        $errors[] = t('An x or y field is defined, but not does not exist.');
      }
    }

    return $errors;
  }
}
