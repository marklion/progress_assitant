<template>
<div class="supplier_info_show">
    <van-cell-group>
        <template #title>
            <van-row type="flex" justify="space-between" align="center">
                <van-col>所有供应商</van-col>
                <van-col>
                    <van-button type="primary" size="small" @click="cur_opt_add=true;edit_supplier_show=true;">添加</van-button>
                </van-col>
            </van-row>
        </template>
        <van-cell v-for="(single_supplier, index) in all_suppliers" :key="index" center :title="single_supplier.name" :value="'最多派车:' + single_supplier.max_vichele" :label="'储量' + single_supplier.reserves">
            <template #right-icon>
                <div style="margin-left:5px;">
                    <van-button type="warning" size="small" @click="trigger_update(single_supplier)">修改</van-button>
                    <van-button type="danger" size="small" @click="trigger_delete(single_supplier)">删除</van-button>
                </div>
            </template>
        </van-cell>
    </van-cell-group>
    <van-dialog v-model="edit_supplier_show" title="新增供应商" :showConfirmButton="false" closeOnClickOverlay>
        <van-form @submit="edit_supplier">
            <van-field v-model="focus_supplier.name" :disabled="!cur_opt_add" label="公司名称" placeholder="请输入公司名" :rules="[{ required: true, message: '请填写公司名' }]" />
            <van-field v-model="focus_supplier.reserves" label="预计储量" placeholder="请输入预计储量" :rules="[{ required: true, message: '请填写预计储量' }]" />
            <van-field v-model="focus_supplier.max_vichele" label="最多派车数" placeholder="请输入最多派车数" :rules="[{ required: true, message: '请填写最多派车数' }]" />
            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">提交</van-button>
            </div>
        </van-form>
    </van-dialog>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Cell,
    CellGroup
} from 'vant';
import {
    Col,
    Row
} from 'vant';
import {
    Button
} from 'vant';
import {
    Dialog
} from 'vant';
import {
    Form
} from 'vant';
import {
    Field
} from 'vant';

Vue.use(Form);
Vue.use(Field);
Vue.use(Dialog);
Vue.use(Button);
Vue.use(Col);
Vue.use(Row);
Vue.use(Cell);
Vue.use(CellGroup);
export default {
    name: 'SupplierInfo',
    data: function () {
        return {
            cur_opt_add: true,
            focus_supplier: {
                name: '',
                reserves: '',
                max_vichele: '',
            },
            edit_supplier_show: false,
            all_suppliers: [],
        };
    },
    methods: {
        trigger_update: function (_supplier) {
            this.focus_supplier = {
                ..._supplier
            };
            this.cur_opt_add = false;
            this.edit_supplier_show = true;
        },
        trigger_delete: function (_supplier) {
            var vue_this = this;
            Dialog.confirm({
                    title: '删除供应商',
                    message: '确定要删除 ' + _supplier.name + " 吗？",
                })
                .then(() => {
                    vue_this.$call_remote_process("vichele_management", "del_supplier", [vue_this.$cookies.get('pa_ssid'), _supplier.id]).then(function (resp) {
                        if (resp) {
                            vue_this.init_supplier();
                        }
                    });
                })

        },
        init_supplier: function () {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", "get_all_supplier", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.all_suppliers = [];
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.all_suppliers, index, element);
                });
            });
        },
        edit_supplier: function () {
            var vue_this = this;
            var func_name = 'add_supplier';
            if (vue_this.cur_opt_add == false) {
                func_name = 'update_supplier';
            }
            vue_this.focus_supplier.reserves = parseFloat(vue_this.focus_supplier.reserves);
            vue_this.focus_supplier.max_vichele = parseInt(vue_this.focus_supplier.max_vichele);
            vue_this.$call_remote_process("vichele_management", func_name, [vue_this.$cookies.get('pa_ssid'), vue_this.focus_supplier]).then(function (resp) {
                if (resp) {
                    vue_this.init_supplier();
                    vue_this.edit_supplier_show = false;
                    vue_this.focus_supplier = {
                        name: '',
                        reserves: '',
                        max_vichele: '',
                    };
                }
            });
        },
    },
    beforeMount: function () {
        this.init_supplier();
    }
}
</script>

<style>

</style>
