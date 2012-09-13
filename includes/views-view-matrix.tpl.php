<?php
/**
 * @file
 *   Views View Matrix Template
 *
 * Variables:
 *   - $header
 *   - $rows
 *   - $row_classes
 */
?>
<table class="views-matrix">
  <?php if (!empty($title)) : ?>
    <caption><?php print $title; ?></caption>
  <?php endif; ?>

  <thead>
    <tr>
      <?php foreach ($header as $col => $col_header): ?>
        <th class="views-matrix-col-header <?php print (!$col) ? 'views-matrix-col-first' : ''?>"><?php print $col_header; ?></th>
      <?php endforeach; ?>
    </tr>
  </thead>

  <tbody>
    <?php foreach ($rows as $row_index => $row): ?>
    <tr class="<?php print implode(' ', $row_classes[$row_index]); ?>">
      <?php foreach ($row as $col_index => $content): ?>
        <t<?php print ($col_index) ? 'd' : 'h'; ?><?php  print drupal_attributes($content['attributes']); ?>>
          <?php print !empty($content) ? $content['data'] : ''; ?>
        </t<?php print ($col_index) ? 'd' : 'h'; ?>>
      <?php endforeach; ?>
    </tr>
    <?php endforeach; ?>
  </tbody>
</table>
