<?php
/**
 * @file
 *   Theme definition.
 */


/**
 * Template preprocess views view matrix
 * @param &$vars an associative array with the following keys:
 *   view: the view object
 *   options: the options
 *   rows: the rendered rows
 *   title: the title of the view
 */
function template_preprocess_views_view_matrix(&$vars) {

  drupal_add_css(drupal_get_path('module', 'views_matrix') . '/views_matrix.css');

  // @see template_preprocess_views_view_table
  $result = $vars['result'] = $vars['rows'];
  $vars['rows'] = array();
  $vars['header'] = array('');
  $vars['row_classes'] = array();

  $view = &$vars['view'];
  $options = $view->style_plugin->options;
  $handler = $view->style_plugin;
  $fields = &$view->field;
  
  $xfield = &$fields[$options['xconfig']['field']];
  $yfield = &$fields[$options['yconfig']['field']];

  $columns = array();
  $rows = array();
  $row_header = array();

  foreach ($result as $num => $item) {
    if (!in_array($item->{$xfield->field_alias}, $columns)) {
      // Add to header array.
      $columns[] = $item->{$xfield->field_alias};
    }

    if (!in_array($item->{$yfield->field_alias}, $rows)) {
      $row_header[] = array(
        'data' => $item->{$yfield->field_alias},
        'attributes' => array(
          'class' => array('views-matrix-row-header'),
        ),
      );
      $rows[] = $item->{$yfield->field_alias};
    }

    $vars['row_classes'][$num] = array(
      ($num % 2 == 0) ? 'even' : 'odd',
    );

    if (!empty($options['row class'])) {
      $vars['row_classes'][$num][] = views_css_safe($options['row class']);
    }
  }

  // Sorting @todo
  if ($options['xconfig']['sort'] == 'asc') {
    sort($columns);
  }
  else {
    rsort($columns);
  }

  if ($options['yconfig']['sort'] == 'asc') {
    sort($row_header);
    sort($rows);
  }
  else {
    rsort($row_header);
    rsort($rows);
  }

  // Array padding and merging.
  $vars['header'] = array_merge(array(''), $columns);
  $padded = array_pad(array(), count($columns)+1, array('data' => '', 'attributes' => array('class' => array('views-matrix-cell', 'views-matrix-empty'))));
  $vars['rows'] = array_pad($vars['rows'], count($rows), $padded);

  foreach (element_children($vars['rows']) as $y) {
    foreach (element_children($vars['rows'][$y]) as $x) {
      if ($x > 0) {
        // Checkerboard striping on the actual cells only.
        $vars['rows'][$y][$x]['attributes']['class'][] = (($x-1+$y) %2 == 0) ? 'even' : 'odd';
      }
    }
  }

  foreach ($row_header as $n => $row_head) {
    $vars['rows'][$n][0] = $row_head;
  }

  // Render
  $renders = $handler->render_fields($result);

  foreach ($result as $num => $item) {
    // Go through each view row and repack the item after markup.
    $xkey = array_search($item->{$xfield->field_alias}, $columns);
    $ykey = array_search($item->{$yfield->field_alias}, $rows);
    $rendered_item = '';

    foreach (element_children($fields) as $field_name) {
      // Create the markup for the field.
      $field_handler = &$fields[$field_name];
      $alias = $field_handler->field_alias;

      if ($field_handler->options['exclude']) {
        // Don't render the field.
        continue;
      }

      // Field wrapper
      $wrapper_type = $field_handler->element_wrapper_type();
      $field_output = '<' . $wrapper_type;
      if ($field_handler->options['element_default_classes']) {
        $wrapper_classes = 'views-field views-field-' . $field_name;
      }
      $wrapper_classes .= $field_handler->element_wrapper_classes();
      $field_output .= (!empty($wrapper_classes)) ? ' class="' . $wrapper_classes . '">' : '>';

      if ($label = $field_handler->label()) {
        // Field label
        $label_type = $field_handler->element_label_type();
        $field_output .= '<' . $label_type;
        $label_classes = $field_handler->element_label_classes();
        $field_output .= (!empty($label_clases)) ? ' class="' . $label_classes . '">' : '>';
        $field_output .= '</' . $label_type . '>';
      }

      // Field
      $field_type = $field_handler->element_type();
      $field_output .= '<' . $field_type;
      $field_classes = $field_handler->element_classes();
      $field_output .= (!empty($field_classes)) ? ' class="' . $field_classes . '">' : '>';
      $field_output .= $renders[$num][$field_name];
      $field_output .= '</' . $field_type . '>';

      $field_output .= '</' . $wrapper_type . '>';
      $rendered_item .= $field_output;
    }

    if ($vars['rows'][$ykey][$xkey+1]['data'] <> '') {
      // Pack into existing cell.
      $vars['rows'][$ykey][$xkey+1]['data'] .= $rendered_item;
    }
    else {
      // Pack into new cell and add even/odd class.
      $vars['rows'][$ykey][$xkey+1]['data'] = $rendered_item;
    }
  }
}

