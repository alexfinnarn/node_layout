<div id="edit-layout-draggable-form" class="container-fluid">
  <h3 class="row">Add Block</h3>
  <hr>
  <div id="add-block-form row">
    <div class="row">
      <div class="col">
        <label for="add-block-reference">Block Reference</label>
        <input type="text"
               name="add-block-reference"
               autocomplete="off"
               class="form-control"
               @keyup.delete="hideResults"
               id="add-block-reference"
               v-model="blockReferenceTitle">
        <div v-if="blockReferenceTitle.length >= 2 && hideBox === false">
          <ul class="list-group">
            <li @click="makeSelection(item)"
                class="list-group-item"
                v-for="item in filteredItems">
              {{ item.type }}: {{ item.title }}
            </li>
          </ul>
        </div>
      </div>
      <div class="col">
        <label for="add-block-select-list">Region</label>
        <select name="add-block-select-list"
                class="form-control"
                v-model="blockSelectList"
                id="add-block-select-list">
          <option v-for="region in layout.regions"
                  :value="region.name">
            {{ region.name }}
          </option>
        </select>
      </div>
      <div class="col">
        <button @click.prevent="addBlockToRegion"
                v-if="blockReference"
                class="btn btn-primary">
          Add Block
        </button>
      </div>
    </div>
  </div>
  <h3 class="row">Regions</h3>
  <hr>
  <div class="row">
    <div v-for="region in layout.regions">
      <div class="col-md">
        <h4>{{ region.name }}</h4>
<!--        <pre>{{ region.references }}</pre>-->
        <region v-on:update-list="handleListUpdate"
                :ref="region.name"
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
        {{ element.type }}: {{element.name}}
<!--        <span class="badge">{{element.order}}</span>-->
        <div class="btn btn-primary nl-edit-button" @click="editItem(element.name)">Edit</div>
        <div class="btn btn-danger nl-remove-button" @click="removeItem(element.name)">Remove</div>
      </li>
    </transition-group>
  </draggable>
</script>

<script type="text/javascript">
  const fooLayout = <?php print $data['encoded_layout']; ?>;

  <?php $foo = 'bar'; ?>

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
        list: this.references.map((item, index) => {
          return {
            name: item.title,
            nid:  item.nid,
            type: item.type,
            order: index + 1,
            fixed: false,
          };
        }),
      };
    },
    computed: {
      dragOptions() {
        return {
          animation: 0,
          group: "description",
          disabled: !this.editable,
          ghostClass: "ghost",
        };
      },
    },
    watch: {
      list(newList) {
        var placeholderExists = newList.filter(value => value.name === 'placeholder');

        if (placeholderExists.length >= 1 && newList.length > 1) {
          this.list = this.list.filter(value => value.name !== 'placeholder');
        }

        if (placeholderExists.length === 0 && newList.length === 0) {
          this.list = [{
            fixed: false,
            name: 'placeholder',
            nid: null,
            type: null,
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
      isDragging(newValue) {
        if (newValue) {
          this.delayedDragging = true;
          return;
        }
        this.$nextTick(() => {
          this.delayedDragging = false;
        });
      },
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
      removeItem(itemName) {
        // @todo Remove item by index/key so that you could have the same content
        // repeated.
        this.list = this.list.filter(value => value.name !== itemName);

        var placeholderExists = this.list.filter(value => value.name === 'placeholder');
        if (placeholderExists.length === 0 && this.list.length === 0) {
          this.list = [{
            fixed: false,
            name: 'placeholder',
            nid: null,
            type: null,
            order: 1,
          }];
        }
      },
    },
  });

  let draggableForm = new Vue({
    el: '#edit-layout-draggable-form',
    data() {
      return {
        blockReference: null,
        blockReferenceTitle: '',
        blockSelectList: 'top',
        layout: fooLayout,
        finalLayout: fooLayout,
        nodes: [],
        hideBox: false,
      };
    },
    computed: {
      filteredItems() {
        return this.nodes.filter((item) => {
          return item.title.toLowerCase().indexOf(this.blockReferenceTitle.toLowerCase()) > -1
        })
      },
    },
    methods: {
      initialize() {
        // Grab the node references.
        let that = this;
        const baseURL = Backdrop.settings.node_layout.baseURL;

        let nodes = fetch(baseURL + '/api/node_layouts')
        .then((response) => response.json())
        .then((data) => {
          that.nodes = JSON.parse(data);
        })
        .catch((err) => {
          console.log(err);
        });
      },
      addBlockToRegion() {
        // @todo Add check for when the user doesn't select a choice.
        this.$refs[this.blockSelectList][0].list.push({
          nid: this.blockReference.nid,
          type: this.blockReference.type,
          name: this.blockReference.title,
          order: this.$refs[this.blockSelectList][0].list.length + 1,
          fixed: false,
        });
      },
      handleListUpdate(updatedlist) {
        this.finalLayout.regions[updatedlist.name]['references'] = updatedlist.references;

        // On page load, the references for the finalLayout variable aren't mapped.
        // I think this is also the case since the lists are tracked within region
        // components but not within the main Vue instances.
        Object.keys(this.$refs).forEach((ref) => {
          this.finalLayout.regions[ref]['references'] = this.$refs[ref][0].list;
        });

        // I don't know of a better way to sync the component state and the
        // Backdrop form state.
        // @todo See if "this.layout" can be used instead.
        jQuery('input[name="_final_layout"]').val(JSON.stringify(this.finalLayout));
      },
      makeSelection(item) {
        this.blockReference = item;
        this.blockReferenceTitle = item.title;
        this.hideBox = true;
      },
      hideResults(event) {
        this.hideBox = false;
        this.blockReference = null;
      },
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
  
  .nl-edit-button, .nl-remove-button {
    cursor: pointer;
  }

  .btn-primary {
    background: #0074bd;
  }
</style>
