<?php


/**
 * NodeLayoutEntity extends Entity.
 *
 * A subclass of Entity is a requirement for creating a custom entity. It is
 * also a minimum requirement to define the methods included here, even as
 * empty functions.
 */
class NodeLayoutEntity extends Entity {

  /**
   * Implements EntityInterface::id().
   */
  public function id() {
  }

  /**
   * Implements EntityInterface::entityType().
   */
  public function entityType() {
    return 'node_layout';
  }

  /**
   * Implements EntityInterface::bundle().
   */
  public function bundle() {
  }

  /**
   * Implements EntityInterface::label().
   */
  public function label() {
  }

  /**
   * Implements EntityInterface::uri().
   */
  public function uri() {
  }
}


/**
 * NodeLayoutEntityControllerInterface definition.
 *
 * We create an interface here because anyone could come along and
 * use hook_entity_info_alter() to change our controller class.
 * We want to let them know what methods our class needs in order
 * to function with the rest of the module, so here's a handy list.
 *
 * @see hook_entity_info_alter()
 */
interface NodeLayoutEntityControllerInterface
  extends EntityControllerInterface {

  /**
   * Create an entity.
   */
  public function create($values);

  /**
   * Save an entity.
   *
   * @param object $entity
   *   The entity to save.
   */
  public function save($entity);

  /**
   * Delete an entity.
   *
   * @param object $entity
   *   The entity to delete.
   */
  public function delete($entity);

}

/**
 * NodeLayoutEntityController extends BackdropDefaultEntityController.
 *
 * Our subclass of BackdropDefaultEntityController lets us add a few
 * important create, update, and delete methods.
 */
class NodeLayoutEntityController
  extends DefaultEntityController
  implements NodeLayoutEntityControllerInterface {

  /**
   * Create an entity.
   */
  public function create($values) {
    // It is important when creating entities in Backdrop to ensure that you
    // create an instance of the core Entity class rather than an instance of
    // stdClass(). This would lead to exceptions on attempting to save your
    // custom entity.
    // See https://github.com/backdrop/backdrop-issues/issues/2558
    $entity = new NodeLayoutEntity();
    $entity->id = NULL;
    $entity->nid = $values['nid'];
    $entity->data = serialize($values['layout']);
    return $entity;
  }

  /**
   * Save an entity.
   *
   * @param object $redirect
   *   The entity to save.
   *
   * @throws \Exception
   */
  public function save($node_layout) {
    $transaction = db_transaction();
    try {
      // Load the stored entity, if any.
      if (!empty($node_layout->id) && !isset($node_layout->original)) {
        $node_layout->original = entity_load_unchanged('node_layout', $node_layout->id);
      }

      // Invoke hook_entity_presave() on other modules.
      module_invoke_all('entity_presave', $node_layout, 'node_layout');

      // Save to the database and fire relevant hooks.
      if (!empty($node_layout->id)) {

        backdrop_write_record('node_layout', $node_layout, array('id'));
        // Invoke hook_entity_update() on other modules.
        module_invoke_all('entity_update', $node_layout, 'node_layout');
      }
      else {
        backdrop_write_record('node_layout', $node_layout);
        // Invoke hook_entity_insert() on other modules.
        module_invoke_all('entity_insert', $node_layout, 'node_layout');
      }
    } catch (Exception $e) {
      $transaction->rollback('node_layout');
      watchdog_exception('node_layout', $e);
      throw $e;
    }
  }

  /**
   * Delete an entity.
   *
   * @param object $entity
   *   The entity to delete.
   *
   * @throws \Exception
   */
  public function delete($ids) {
    $transaction = db_transaction();
    try {
      foreach ($ids as $id) {
        $node_layout = entity_load('node_layout', $id);
        // Invoke hook_entity_delete() on other modules.
        module_invoke_all('entity_delete', $node_layout, 'node_layout');

        db_delete('node_layout')
          ->condition('id', $node_layout->id, '=')
          ->execute();
      }
    } catch (Exception $e) {
      $transaction->rollback('node_layout');
      watchdog_exception('node_layout', $e);
      throw $e;
    }
  }
}
