<template>
<div class="plan_import_show">
    <stuff-title :type_id="parseInt($route.params.type_id)"></stuff-title>
    <van-dialog v-model="import_diag" title="导入计划" :showConfirmButton="false" closeOnClickOverlay>
        <div class="info_show">
            点击提交按钮后，计划表中的车辆司机信息将自动导入，请注意检查，货品名称、进厂时间等信息
        </div>
        <van-button size="small" plain @click="preview_format_sample">点击查看导入表格格式</van-button>
        <van-form @submit="import_plan_table">
            <van-field name="uploader">
                <template #label v-if="table_name">
                    {{table_name}}
                </template>
                <template #input>
                    <van-button :disabled="!table_from_user" size="small" plain type="info" native-type="button" @click="show_vichele_table = true">预览</van-button>
                    <van-popup v-model="show_vichele_table" position="bottom" get-container="body">
                        <vxe-table size="small" stripe align="center" max-height="500" :data="table_from_user">
                            <vxe-table-column field="main_vichele" title="主车"></vxe-table-column>
                            <vxe-table-column field="behind_vichele" title="挂车"></vxe-table-column>
                            <vxe-table-column field="driver_name" title="司机"></vxe-table-column>
                            <vxe-table-column field="driver_phone" title="电话"></vxe-table-column>
                            <vxe-table-column field="drop_address" title="卸车地址"></vxe-table-column>
                            <vxe-table-column field="use_for" title="用途"></vxe-table-column>
                        </vxe-table>
                    </van-popup>
                </template>
                <template #button>
                    <van-uploader :after-read="prepare_table" accept=".xls,.xlsx">
                        <van-button native-type="button" type="primary" size="small">上传计划表</van-button>
                    </van-uploader>
                </template>
            </van-field>

            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">提交</van-button>
            </div>
        </van-form>
    </van-dialog>
    <van-popup v-model="sheet_seletct_picker" round position="bottom">
        <div>请指定计划内容在那张工作表</div>
        <van-image :src="$remote_url + '/logo_res/sheet_select_prompt.jpg'"></van-image>
        <van-picker show-toolbar :columns="sheet_in_wb" title="请选择工作表" @cancel="sheet_seletct_picker = false" @confirm="confirm_sheet" />
    </van-popup>
    <stuff-info-submit :is_create="true" :min_time="new Date()" :orig_name="stuff_name" :type_id="parseInt($route.params.type_id)" :orig_price="price" :orig_vichele_info="vichele_info"></stuff-info-submit>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Dialog
} from 'vant';
import {
    Uploader
} from 'vant';
import {
    Form
} from 'vant';
import {
    Field
} from 'vant';
import {
    Popup
} from 'vant';
import XLSX from 'xlsx'
import {
    Picker
} from 'vant';
import {
    Image as VanImage
} from 'vant';

Vue.use(VanImage);
Vue.use(Picker);
Vue.use(Popup);
Vue.use(Form);
Vue.use(Field);
Vue.use(Uploader);
Vue.use(Dialog);
import { ImagePreview } from 'vant';
import StuffTitle from '../components/StuffTitle.vue'
import StuffInfoSubmit from '../components/StuffInfoSubmit.vue'
export default {
    name: 'PlanImport',
    data: function () {
        return {
            import_diag: true,
            table_from_user: [],
            table_name: '',
            show_vichele_table: false,
            stuff_name: '',
            price: 0,
            vichele_info: [],
            sheet_seletct_picker: false,
            sheet_in_wb: [],
            wb: '',
        };
    },
    components: {
        "stuff-title": StuffTitle,
        "stuff-info-submit": StuffInfoSubmit,
    },
    methods: {
        preview_format_sample: function () {
            ImagePreview([this.$remote_url + '/logo_res/plan_sheet_sample.jpg']);
        },
        confirm_sheet: function (_value) {
            this.table_from_user = this.convert_wb_2_array(this.wb.Sheets[_value]);
            this.sheet_seletct_picker = false;
        },
        convert_wb_2_array: function (current_sheetes) {
            var ret = [];
            var ancher_x = '';
            var ancher_y = '';
            for (var position in current_sheetes) {
                if (current_sheetes[position].w == "序号") {
                    ancher_x = position.match(/[A-Z]/g).join('');
                    ancher_y = parseInt(position.match(/\d/g).join('')) + 1;
                    break;
                }
            }
            var last_y = parseInt(current_sheetes['!ref'].split(':')[1].match(/\d/g).join(''));
            var main_vichele_x = String.fromCharCode(ancher_x.charCodeAt() + 4);
            var behind_vichele_x = String.fromCharCode(ancher_x.charCodeAt() + 5);
            var driver_name_x = String.fromCharCode(ancher_x.charCodeAt() + 6);
            var driver_phone_x = String.fromCharCode(ancher_x.charCodeAt() + 7);
            var drop_address_x = String.fromCharCode(ancher_x.charCodeAt() + 9);
            var use_for_x = String.fromCharCode(ancher_x.charCodeAt() + 10);
            for (var i = ancher_y; i <= last_y; i++) {
                if (current_sheetes[main_vichele_x + i] &&
                    current_sheetes[behind_vichele_x + i] &&
                    current_sheetes[driver_name_x + i] &&
                    current_sheetes[driver_phone_x + i] &&
                    current_sheetes[drop_address_x + i] &&
                    current_sheetes[use_for_x + i]) {
                    var one_plan = {
                        main_vichele: current_sheetes[main_vichele_x + i].w,
                        behind_vichele: current_sheetes[behind_vichele_x + i].w,
                        driver_name: current_sheetes[driver_name_x + i].w,
                        driver_phone: current_sheetes[driver_phone_x + i].w,
                        drop_address: current_sheetes[drop_address_x + i].w,
                        use_for: current_sheetes[use_for_x + i].w,
                    };
                    ret.push(one_plan);
                }
            }

            return ret;

        },
        prepare_table: function (_file) {
            var reader = new FileReader();
            this.table_name = _file.file.name;
            var vue_this = this;
            reader.onload = function (e) {
                var data = new Uint8Array(e.target.result);
                var workbook = XLSX.read(data, {
                    type: 'array'
                });
                vue_this.sheet_in_wb = [];
                workbook.SheetNames.forEach((element, index) => {
                    vue_this.$set(vue_this.sheet_in_wb, index, element);
                });
                vue_this.wb = workbook;
                vue_this.sheet_seletct_picker = true;
                vue_this.table_from_user = [];
            };
            reader.readAsArrayBuffer(_file.file);
        },
        import_plan_table: function () {
            var vue_this = this;
            vue_this.table_from_user.forEach((element, index) => {
                vue_this.$call_remote_process_no_toast("user_management", "bind_new_vichele", [vue_this.$cookies.get('pa_ssid'), element.main_vichele, true]);
                vue_this.$call_remote_process_no_toast("user_management", "bind_new_vichele", [vue_this.$cookies.get('pa_ssid'), element.behind_vichele, false]);
                vue_this.$call_remote_process_no_toast("user_management", "bind_new_driver", [vue_this.$cookies.get('pa_ssid'), {
                    name: element.driver_name,
                    phone: element.driver_phone
                }]);
                vue_this.$set(vue_this.vichele_info, index, element);
            });
            this.import_diag = false;
        },
    },
    beforeMount: function () {
        var vue_this = this;
        this.$call_remote_process("stuff_info", 'get_stuff_detail', [parseInt(vue_this.$route.params.type_id)]).then(function (resp) {
            vue_this.stuff_name = resp.name;
            vue_this.price = resp.price;
        });
    },
}
</script>

<style scoped>
.info_show {
    padding: 20px 10px;
    color: red;
}
</style>
