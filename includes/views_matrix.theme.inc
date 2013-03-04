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
/*function template_preprocess_views_view_matrix(&$vars) {

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

  $errors = $handler->canRender();
  if (!empty($errors)) {
    // Validation of row and column headers during runtime so it doesn't explode.
    $message = theme('item_list', array('items' => $errors));
    drupal_set_message(t('The view could not be rendered due to the following errors: !error', array('!error' => $message)), 'error');
    return;
  }
  
  $xfield = &$fields[$options['xconfig']['field']];
  $yfield = &$fields[$options['yconfig']['field']];
  
  $columns = array();
  $rows = array();
  $row_header = array();
  
  // Render
  $renders = $handler->render_fields($result);
  
  // Storage for where things are.
  // Lookup of serialized values to the header and row header that shows them.
  $value_index_lookup = array();
  // Lookup of coordinates (the values in $value_index_lookup) for result
  // indexes, of the form $index => array(x => $index, y => $index), where the
  // indexes are values in $value_index_lookup.
  $coordinates = array();

  // Populate both the header and row header with unique values from the
  // respective fields. Both these arrays will be keyed by result index, which
  // will therefore not necessarily be sequential. This means we can later use
  // them as co-ordinates.
  foreach ($result as $index => $item) {
    // Get the values from our x and y fields.
    // Note that because of FieldAPI these may be arrays.
    $xvalue = $xfield->get_value($item);
    $yvalue = $yfield->get_value($item);
    
    if (!in_array($xvalue, $columns)) {
      // Add to header array, an array of $index => value.
      $columns[$index] = $xvalue;
      $value_index_lookup['x'][serialize($xvalue)] = $index;
      $columns_renders[$index] = $renders[$index][$options['xconfig']['field']];
    }
    // Whether we've placed this item in the header or not, we can get its
    // coordinate.
    $coordinates[$index]['x'] = $value_index_lookup['x'][serialize($xvalue)];

    if (!in_array($yvalue, $rows)) {
      // Add to row header array.
      $row_header[$index] = array(
        'data' => $renders[$index][$options['yconfig']['field']],
        'attributes' => array(
          'class' => array('views-matrix-row-header'),
        ),
      );
      $rows[$index] = $yvalue;
      $value_index_lookup['y'][serialize($yvalue)] = $index;
      $rows_renders[$index] = $renders[$index][$options['yconfig']['field']];
    }
    $coordinates[$index]['y'] = $value_index_lookup['y'][serialize($yvalue)];

    $vars['row_classes'][$index] = array(
      ($index % 2 == 0) ? 'even' : 'odd',
    );

    if (!empty($options['row class'])) {
      $vars['row_classes'][$index][] = views_css_safe($options['row class']);
    }
  }

  // Sorting.
  // This may not always make sense with FieldAPI fields, eg image fields.
  if ($options['xconfig']['sort'] == 'asc') {
    asort($columns_renders);
  }
  else {
    arsort($columns_renders);
  }

  if ($options['yconfig']['sort'] == 'asc') {
    asort($rows_renders);
  }
  else {
    arsort($rows_renders);
  }

  // Create the header and rows arrays for theme_table(), populating the header
  // and row header cells, and padding out the table with empty cells.
  $vars['header'] = array();
  $vars['header'][] = '';
  // Use the rendered column array, as it is sorted.
  foreach (array_keys($columns_renders) as $xindex) {
    // Place the rendered field in the table header.
    $vars['header'][] = $renders[$xindex][$options['xconfig']['field']];
  }
  // Fill up the table with the first column of each row from the row header,
  // and the rest of each row with blanks, using the index values from both the
  // header arrays so we can use our coordinates later.
  $xparity = $yparity = 0;
  foreach (array_keys($rows_renders) as $yindex) {
    // Set the row header.
    $vars['rows'][$yindex]['header'] = $row_header[$yindex];
    
    $yparity = 0;
    // Fill in blank cells.
    foreach (array_keys($columns_renders) as $xindex) {
      $classes = array('views-matrix-cell', 'views-matrix-empty');
      $classes[] = (($xparity + $yparity) %2 === 0) ? 'even' : 'odd';
      $vars['rows'][$yindex][$xindex] = array(
        'data' => '',
        'attributes' => array('class' => $classes),
      );
      $yparity++;
    }
    $xparity++;
  }
  
  foreach ($result as $index => $item) {
    // Go through each view row and repack the item after markup.
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
      $field_output .= $renders[$index][$field_name];
      $field_output .= '</' . $field_type . '>';

      $field_output .= '</' . $wrapper_type . '>';
      $rendered_item .= $field_output;
    }
    
    // Get the coordinates out of the array.
    $x_coordinate = $coordinates[$index]['x'];
    $y_coordinate = $coordinates[$index]['y'];
    // The row number is y, the column number is x.
    $vars['rows'][$y_coordinate][$x_coordinate]['data'] .= $rendered_item;

    // Remove class views-matrix-empty
    $vars['rows'][$y_coordinate][$x_coordinate]['attributes']['class'] = array_diff($vars['rows'][$y_coordinate][$x_coordinate]['attributes']['class'], array('views-matrix-empty'));
  }
}*/

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

  $errors = $handler->canRender();
  if (!empty($errors)) {
    // Validation of row and column headers during runtime so it doesn't explode.
    $message = theme('item_list', array('items' => $errors));
    drupal_set_message(t('The view could not be rendered due to the following errors: !error', array('!error' => $message)), 'error');
    return;
  }


  $column_field = array('field' => 'field_size', 'sort' => 'asc');

  $row_fields = array(
    array('field' => 'title', 'sort' => 'asc'),
    array('field' => 'field_color_term', 'sort' => 'asc')
  );

  $data_fields = array();
  foreach($row_fields as $row_field){
    $data_fields[] = $row_field['field'];
  }
  $data_fields[] = $column_field['field'];

  /*$data = array(
    'product_1' => array(
      'header' => 'Product 1 Name',
      'data' => array(
        'color_1' => array(
          'header' => 'Color 1 Name',
          'data' => array(
            'size_1' => array(
              'data' => CELL_DATA,
            )
            'size_2' => array(
              'data' => CELL_DATA,
            )
          ),
        ),
      ),
    ),
  );*/

  $renders = $handler->render_fields($result);
  $data = array();
  $column_headers = array();
//dpm(get_defined_vars());return;

  foreach ($result as $row_index => $row){
    $cell = &$data;
    foreach($data_fields as $field){
      $field_value = $field_index = $fields[$field]->get_value($row);
      if(!is_scalar($field_index)){
        $field_index = md5(serialize($field_index));
      }
      if(empty($cell[$field_index])){
        $cell[$field_index] = array();
        $cell[$field_index]['header'] = $field_value;
        $cell[$field_index]['data'] = NULL;
      }
      $cell = &$cell[$field_index]['data'];
    }
    if(empty($column_headers[$field_index])){
      $column_headers[$field_index] = $field_value;
    }
    $cell = "CELL_DATA";
  }

  /*$column_headers = array(
    'size_1' => 'S',
    'size_2' => 'M',
    'size_3' => 'L',
  );*/
//dpm(get_defined_vars());return;
  $rows = _views_matrix_rows($data, $data_fields, $column_headers);
  dpm(get_defined_vars());
}

function _views_matrix_rows(&$data, $data_fields, $column_headers, $level = 1){
  $cells = array();
  if(count($data_fields) < $level){
    foreach($column_headers as $column_index => $column){
      $cell = array(
        'data' => $data['data'][$column_index],
        'attributes' => array('class' => array('views-matrix-cell')),
      );
dpm($level);dpm($column_headers);dpm($data);dpm($column);
      if(empty($data['data'][$column_index])){
        $cell['attributes']['class'][] = 'views-matrix-empty';
      }
      $cells[] = $cell;
    }
    return array($cells);
  }
  $rows = array();
dpm($level);
  foreach($data as $index => $value){
    $sub_rows = _views_matrix_rows($value, $data_fields, $column_headers, $level + 1);
    $row_count = count($value['data']);
    $row_header = array(
      'data' => $value['header'],
      'attributes' => array(
        'class' => array('views-matrix-row-header'),
      ),
    );
    array_unshift($sub_rows[0], $row_header);
    $rows = array_merge($rows, $sub_rows);
  }
  return $rows;
}
