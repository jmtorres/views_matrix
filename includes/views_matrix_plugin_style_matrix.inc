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
      '#title' => t('Column header'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
    );

    $form['xconfig']['field'] = array(
      '#type' => 'select',
      '#title' => t('Field'),
      '#description' => t('Select the field to display in the column header.'),
      '#options' => $field_options,
      '#default_value' => $this->options['xconfig']['field'],
    );

    $form['xconfig']['sort'] = array(
      '#type' => 'select',
      '#title' => t('Sort'),
      '#description' => t('Select the method to sort the field by.'),
      '#options' => array('asc' => t('Ascending'), 'dsc' => t('Descending')),
      '#default_value' => $this->options['xconfig']['sort'],
    );

    $form['yconfig'] = array(
      '#type' => 'fieldset',
      '#title' => t('Row header'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
    );

    $form['yconfig']['field'] = array(
      '#type' => 'select',
      '#title' => t('Field'),
      '#description' => t('Select the field to display in the row header.'),
      '#options' => $field_options,
      '#default_value' => $this->options['yconfig']['field'],
    );

    $form['yconfig']['sort'] = array(
      '#type' => 'select',
      '#title' => t('Sort'),
      '#description' => t('Select the method to sort the field by.'),
      '#options' => array('asc' => t('Ascending'), 'dsc' => t('Descending')),
      '#default_value' => $this->options['yconfig']['sort'],
    );
  }

  /**
   * Validate row and colum header configuration set. This is only used during
   * runtime. parent::validate() cannot be used because it breaks the Views
   * UI.
   */
  public function canRender() {

    $errors = parent::validate();

    if (empty($this->options['xconfig']['field']) || empty($this->options['yconfig']['field'])) {
      // Do not save without x field or y field.
      $errors[] = t('Style @style requires an x and y field to be defined.', array('@style' => $this->definition['title']));
    }
    else {
      // Check if fields exist still.
      $options = $this->display->handler->get_field_labels(TRUE);

      if (!isset($options[$this->options['xconfig']['field']]) || !isset($options[$this->options['yconfig']['field']])) {
        $errors[] = t('An x or y field is defined, but not does not exist.');
      }
    }

    return $errors;
  }
}
