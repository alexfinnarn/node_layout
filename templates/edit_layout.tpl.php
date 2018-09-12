<?php
$foo = 'bar';
?>

<div id="edit-layout-draggable-form">
  <div id="add-block-form" class="row">
    <div class="panel panel-default">
      <div class="panel-heading">
        <h3>Add Block</h3>
      </div>
      <div class="panel-body">
        <input type="text"
               name="add-block-reference"
               id="add-block-reference"
               v-model="addBlockReference">
        <select name="add-block-select-list"
                class="form-control"
                v-model="blockSelectList"
                id="add-block-select-list">
          <option v-for="region in layout.regions"
                  :value="region.name">
            {{ region.name }}
          </option>
        </select>
        <button @click.prevent="addBlockToRegion()"
                class="btn btn-primary">
          Add Block
        </button>
      </div>
    </div>
  </div>
  <div class="row">
    <div v-for="region in layout.regions">
      <div class="col-md">
        <h3>{{ region.name }}:</h3>
        <region v-on:update-list="handleListUpdate"
                :region-name="region.name"
                :references="region.references">
        </region>
      </div>
    </div>
  </div>
</div>

<script type="text/x-template" id="region-template">
  <draggable class="list-group"
             element="ul"
             v-model="list"
             :options="dragOptions"
             :move="onMove"
             @start="isDragging=true"
             @end="isDragging = false">
    <transition-group type="transition" :name="'flip-list'">
      <li class="list-group-item" v-for="element in list" :key="element.order">
        <i :class="element.fixed? 'fa fa-anchor' : 'glyphicon glyphicon-pushpin'" @click=" element.fixed=! element.fixed" aria-hidden="true"></i>
        {{element.name}}
        <span class="badge">{{element.order}}</span>
      </li>
    </transition-group>
  </draggable>
</script>

<script type="text/javascript">
  jQuery(document).ready(function () {
    draggableForm.initialize();
  });

  Vue.component('region', {
    template: '#region-template',
    props: {
      references: Array,
      regionName: String,
    },
    data() {
      return {
        editable: true,
        isDragging: false,
        delayedDragging: false,
        list: this.references.map((name, index) => {
          return { name, order: index + 1, fixed: false };
        }),
      };
    },
    created() {
      // console.log(this);
    },
    beforeUpdate() {
      // console.log(this);
    },
    methods: {
      // orderList() {
      //   this.list = this.list.sort((one, two) => {
      //     return one.order - two.order;
      //   });
      // },
      onMove({ relatedContext, draggedContext }) {
        const relatedElement = relatedContext.element;
        const draggedElement = draggedContext.element;
        return (
          (!relatedElement || !relatedElement.fixed) && !draggedElement.fixed
        );
      },
    },
    computed: {
      dragOptions() {
        return {
          animation: 0,
          group: "description",
          disabled: !this.editable,
          ghostClass: "ghost"
        };
      },
    },
    watch: {
      isDragging(newValue) {
        if (newValue) {
          this.delayedDragging = true;
          return;
        }
        this.$nextTick(() => {
          this.delayedDragging = false;
        });
      },
      list(newValue, oldValue) {
        var placeholderExists = newValue.filter(value => value.name === 'placeholder');

        if (placeholderExists.length === 1 && newValue.length > 1) {
          this.list = this.list.filter(value => value.name !== 'placeholder');
        }

        if (placeholderExists.length === 0 && newValue.length === 0) {
          this.list = [{
            fixed: false,
            name: 'placeholder',
            order: 1,
          }];
        }

        // Emit event with new values.
        if (this.list.filter(value => value.name !== 'placeholder')) {
          this.$emit('update-list', {
            name: this.regionName,
            references: this.list,
          });
        }
      },
    },
  });

  const fooLayout = <?php print $data['encoded_layout']; ?>;

  let draggableForm = new Vue({
    el: '#edit-layout-draggable-form',
    // components: {
    //   region: Region,
    // },
    data() {
      return {
        addBlockReference: '',
        blockSelectList: 'top',
        layout: fooLayout,
        finalLayout: fooLayout,
      };
    },
    updated() {
      console.log(this);
    },
    methods: {
      initialize() {
        // console.log(this.layout);
      },
      addBlockToRegion() {
        console.log(this.layout.regions[this.blockSelectList]);

        this.layout.regions[this.blockSelectList].references = [];
        // this[this.blockSelectList + 'List'].push({
        //   name: this.addBlockReference,
        //   order: this[this.blockSelectList + 'List'].length + 1,
        //   fixed: false,
        // });
      },
      handleListUpdate(updatedlist) {
        // console.log(updatedlist);
        // this.finalRegions
        // console.log(this.finalLayout);

        this.finalLayout.regions[updatedlist.name]['references'] = updatedlist.references;

        console.log(this.finalLayout, 'final layout');
        jQuery('input[name="_final_layout"]').val(JSON.stringify(this.finalLayout));
      }
    },
  });
</script>

<style>
  .flip-list-move {
    transition: transform 0.5s;
  }
  .no-move {
    transition: transform 0s;
  }
  .ghost {
    opacity: 0.5;
    background: #c8ebfb;
  }
  .list-group {
    min-height: 20px;
  }
  .list-group-item {
    cursor: move;
  }
  .list-group-item i {
    cursor: pointer;
  }
</style>
