<template>
<div class="company_home_show">
    <van-row type="flex" align="center" :gutter="10">
        <van-col :offset="2" :span="6">
            <van-image round width="80px" height="80px" fit="cover" :src="$remote_url +  company_logo" />
        </van-col>
        <van-col :span="16">
            <div class="company_name_show ">
                {{$store.state.userinfo.company}}
            </div>
            <van-row type="flex" align="center" :gutter="10">
                <van-col>
                    <van-icon name="setting-o" size="25" color="#1989fa" @click="nav_to_company_data" />
                </van-col>
            </van-row>
        </van-col>
    </van-row>
    <van-grid>
        <van-grid-item v-for="(single_grid, index) in all_grids" :key="index" :class="{grid_is_active: index == active_index, grid_is_not_active: index != active_index}" :icon="single_grid.icon" :text="single_grid.text" @click="active_index = index" />
    </van-grid>
    <div v-if="active_index == 0">
        <van-cell-group>
            <template #title>
                <van-row type="flex" justify="space-between" align="center">
                    <van-col>今日报价</van-col>
                    <van-col>
                        <van-button type="primary" size="small" round icon="plus" @click="open_add_type">增加商品</van-button>
                    </van-col>
                </van-row>
            </template>
            <van-cell v-for="(single_type, index) in all_type" center :key="index" :value="'¥' + single_type.price" :title="single_type.name">
                <template #extra>
                    <van-popover v-model="show_operate[index]" trigger="click" :actions="support_operate" @select="do_operate" @open="open_operate(single_type)" @close="focus_type = 0">
                        <template #reference>
                            <van-button size="small" round plain type="primary" class="opt_btn_show">操作</van-button>
                        </template>
                    </van-popover>
                </template>
                <template #label v-if="single_type.last">
                    <van-tag plain type="danger">{{single_type.last}}</van-tag>
                </template>
                <template #icon v-if="!single_type.saling">
                    <div class="saling_tag_show">
                        已下架
                    </div>
                </template>
            </van-cell>
        </van-cell-group>
        <van-dialog :show-confirm-button="false" close-on-click-overlay v-model="show_add_stuff" title="添加商品">
            <van-form @submit="add_stuff">
                <van-field v-model="add_stuff_name" name="商品名" label="商品名" placeholder="请输入商品名" :rules="[{ required: true, message: '请填写商品名' }]" />
                <van-field v-model="add_stuff_price" type="number" name="价格" label="价格" placeholder="请输入价格" :rules="[{ required: true, message: '请填写价格' }]" />
                <van-field v-model="add_stuff_last" name="存量" label="备注" placeholder="请输入备注信息" />
                <div style="margin: 16px;">
                    <van-button round block type="info" native-type="submit">确认</van-button>
                </div>
            </van-form>
        </van-dialog>
        <van-dialog :show-confirm-button="false" close-on-click-overlay v-model="show_edit_stuff" title="修改商品">
            <van-form @submit="edit_stuff">
                <van-field v-model="stuff_in_edit.name" name="商品名" label="商品名" readonly />
                <van-field v-model="stuff_in_edit.price" name="价格" label="价格" placeholder="请输入价格" :rules="[{ required: true, message: '请填写价格' }]" />
                <van-field v-model="stuff_in_edit.last" name="存量" label="备注" placeholder="请输入备注信息" />
                <div style="margin: 16px;">
                    <van-button round block type="info" native-type="submit">确认</van-button>
                </div>
            </van-form>
        </van-dialog>
        <van-dialog v-model="show_proxy_company_diag" title="代提客户" :showConfirmButton="false" closeOnClickOverlay>
            <van-form @submit="submit_proxy_company">
                <van-field v-model="proxy_company" name="客户" label="客户公司" placeholder="请输入客户名称" :rules="[{ required: true, message: '请填写客户名称' }]" />
                <div style="margin: 16px;">
                    <van-button round block type="info" native-type="submit">提交</van-button>
                </div>
            </van-form>
        </van-dialog>
    </div>
    <div v-else-if="active_index == 1">
        <van-divider>当前公告</van-divider>
        <van-field v-model="notice" rows="2" autosize type="textarea" maxlength="300" placeholder="请输入公告" show-word-limit />
        <van-row type="flex" justify="center" align="center" :gutter="10">
            <van-col :span="10">
                <van-button size="small" round block type="danger" @click="submit_notice(false)">清除</van-button>
            </van-col>
            <van-col :span="10">
                <van-button size="small" round block type="primary" @click="submit_notice(true)">发布</van-button>
            </van-col>
        </van-row>
    </div>
    <div v-else-if="active_index == 2">
        <van-divider>实时数据</van-divider>
        <van-collapse v-model="expend_statistics">
            <van-collapse-item title="今日装车状态" :value="'共' + vichele_statistics.length + '车  出货' + delivered_vichele.length + '车'" name="0">
                <vxe-table size="small" stripe align="center" :data="vichele_statistics">
                    <vxe-table-column field="company" title="公司" width="34%" sortable></vxe-table-column>
                    <vxe-table-column field="main_vichele" title="主车" width="24%"></vxe-table-column>
                    <vxe-table-column field="behind_vichele" title="挂车" width="24%"></vxe-table-column>
                    <vxe-table-column field="delivered" title="状态" width="18%" sortable :formatter="formater_status_vichele"></vxe-table-column>
                </vxe-table>
            </van-collapse-item>
            <van-collapse-item title="明日计划装车" :value="'共' + tomorrow_vichele.length + '车'" name="1">
                <vxe-table size="small" stripe align="center" :data="tomorrow_vichele">
                    <vxe-table-column field="company" title="公司" width="34%" sortable></vxe-table-column>
                    <vxe-table-column field="main_vichele" title="主车" width="24%"></vxe-table-column>
                    <vxe-table-column field="behind_vichele" title="挂车" width="24%"></vxe-table-column>
                    <vxe-table-column field="delivered" title="状态" width="18%" sortable :formatter="formater_status_vichele"></vxe-table-column>
                </vxe-table>
            </van-collapse-item>

        </van-collapse>
    </div>
    <div v-else-if="active_index == 3">
        <van-divider>访客记录</van-divider>
        <van-search v-model="access_search_key" placeholder="公司名\拼音首字母" />
        <van-cell-group :title="'谁报过计划 共' + real_access_users.length + '人'">
            <van-collapse v-model="real_access_users_show">
                <van-collapse-item v-for="(single_record, index) in access_company_after_filter" :key="index" :title="single_record.company_name" :label="single_record.name" :name="index">
                    <template #icon>
                        <van-image round width="40px" height="40px" fit="cover" :src="$remote_url +  single_record.logo" />
                    </template>
                    <van-cell title="联系电话" :value="single_record.phone"></van-cell>
                    <van-cell title="查看资质" is-link @click="show_attachment(single_record.attachment)"></van-cell>
                    <contract-cell :a_side="single_record.company_name" :b_side="$store.state.userinfo.company"></contract-cell>
                </van-collapse-item>
            </van-collapse>
        </van-cell-group>
        <van-cell-group title="谁感兴趣">
            <van-collapse v-model="all_access_users_show">
                <div v-if="all_access_users.length > 0">
                    <van-collapse-item v-for="(single_record, index) in maybe_access_company_after_filter" :key="index" :title="single_record.company_name" :label="single_record.name" :name="index">
                        <template #icon>
                            <van-image round width="40px" height="40px" fit="cover" :src="$remote_url +  single_record.logo" />
                        </template>
                        <van-cell title="联系电话" :value="single_record.phone"></van-cell>
                        <van-cell title="查看资质" is-link @click="show_attachment(single_record.attachment)"></van-cell>
                    </van-collapse-item>
                </div>
                <div v-else>
                    请联系管理员开通查看权限
                </div>
            </van-collapse>
        </van-cell-group>
    </div>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Col,
    Row
} from 'vant';
import {
    Field
} from 'vant';
import {
    Button
} from 'vant';
import {
    Popup
} from 'vant';
import {
    Form
} from 'vant';
import {
    Dialog
} from 'vant';
import {
    NoticeBar
} from 'vant';
import {
    Cell,
    CellGroup
} from 'vant';
import {
    Tag
} from 'vant';
import {
    Icon
} from 'vant';
import {
    Divider
} from 'vant';
import {
    Popover
} from 'vant';
import {
    Image as VanImage
} from 'vant';
import {
    Collapse,
    CollapseItem
} from 'vant';
import {
    Grid,
    GridItem
} from 'vant';
import {
    ImagePreview
} from 'vant';
import ContractCell from '../components/ContractCell.vue';
import {
    Search
} from 'vant';

Vue.use(Search);
import PinyinMatch from 'pinyin-match';
Vue.use(Grid);
Vue.use(GridItem);
Vue.use(Collapse);
Vue.use(CollapseItem);
Vue.use(VanImage);
Vue.use(Popover);
Vue.use(Divider);
Vue.use(Icon);
Vue.use(Tag);
Vue.use(Cell);
Vue.use(CellGroup);
Vue.use(NoticeBar);
Vue.use(Dialog);
Vue.use(Form);
Vue.use(Popup);
Vue.use(Button);
Vue.use(Field);
Vue.use(Col);
Vue.use(Row);
export default {
    name: 'CompanyHome',
    data: function () {
        return {
            access_search_key: '',
            real_access_users_show: [],
            all_access_users_show: [],
            real_access_users: [],
            all_access_users: [],
            active_index: 0,
            all_grids: [{
                icon: 'shop-o',
                text: '今日报价'
            }, {
                icon: 'info-o',
                text: '公告管理'
            }, {
                icon: 'underway-o',
                text: '实时数据'
            }, {
                icon: 'friends-o',
                text: '访客记录'
            }, ],
            expend_statistics: [],
            all_type: [],
            show_add_stuff: false,
            add_stuff_name: '',
            add_stuff_price: '',
            add_stuff_last: '',
            stuff_in_edit: {
                name: '',
                price: 0,
                last: '',
            },
            show_edit_stuff: false,
            show_remove_stuff: false,
            stuff_status: function (_stuff) {
                var ret = 'shop-o';
                if (_stuff.saling == 0) {
                    ret = 'closed-eye';
                }
                return ret;
            },
            notice: '',
            show_operate: [false],
            show_proxy_company_diag: false,
            focus_type: 0,
            proxy_company: '',
            proxy_type_id: 0,
            company_logo: '',
            vichele_statistics: [],
            tomorrow_vichele: [],
            proxy_need_import: false,
        };
    },
    components: {
        "contract-cell": ContractCell,
    },
    computed: {
        access_company_after_filter: function () {
            var ret = [];
            var vue_this = this;
            if (vue_this.access_search_key.length <= 0) {
                ret = vue_this.real_access_users;
            } else {
                vue_this.real_access_users.forEach(element => {
                    if (PinyinMatch.match(element.company_name, vue_this.access_search_key)) {
                        ret.push(element);
                    }
                });
            }

            return ret;
        },
        maybe_access_company_after_filter: function () {
            var ret = [];
            var vue_this = this;
            if (vue_this.access_search_key.length <= 0) {
                ret = vue_this.all_access_users;
            } else {
                vue_this.all_access_users.forEach(element => {
                    if (PinyinMatch.match(element.company_name, vue_this.access_search_key)) {
                        ret.push(element);
                    }
                });
            }

            return ret;
        },
        undelivered_vichele: function () {
            var ret = [];
            this.vichele_statistics.forEach(element => {
                if (!element.delivered) {
                    ret.push(element);
                }
            });
            return ret;
        },
        delivered_vichele: function () {
            var ret = [];
            this.vichele_statistics.forEach(element => {
                if (element.delivered) {
                    ret.push(element);
                }
            });
            return ret;
        },
        enter_company: function () {
            var ret = false;
            if (this.$store.state.userinfo.company && this.$store.state.userinfo.buyer == false) {
                ret = true;
            }
            return ret;
        },
        support_operate: function () {
            var ret = [{
                text: '编辑',
                operate: this.show_out_edit_diag
            }];
            if (this.focus_type.saling) {
                ret.push({
                    text: '下架',
                    operate: this.show_out_remove_diag
                });
            } else {
                ret.push({
                    text: '上架',
                    operate: this.readd_stuff
                })
            }
            ret.push({
                text: '手工提单',
                operate: this.submit_proxy,
            })
            ret.push({
                text: '手工导入',
                operate: this.submit_proxy_import,
            });
            return ret;
        }
    },
    methods: {
        show_attachment: function (_value) {
            if (_value.search(".jpg") != -1) {
                ImagePreview({
                    images: [this.$remote_url + _value],
                    closeable: true
                });
            } else {
                this.$toast("内容不存在");
            }
        },
        formater_status_vichele: function ({
            cellValue
        }) {
            if (cellValue != true) {
                return "未提货";
            } else {
                return "已提货"
            }
        },
        init_vichele_statistices: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "get_today_statistics", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.vichele_statistics, index, element);
                });
            });
        },

        init_tomorrow_vichele: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "get_tomorrow_statistics", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.tomorrow_vichele, index, element);
                });
            });
        },

        nav_to_company_data: function () {
            this.$router.push({
                name: 'BoundInfo'
            });
        },
        open_add_type: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "get_all_attachment", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                if (resp.length > 0) {
                    vue_this.show_add_stuff = true;
                } else {
                    vue_this.$router.push({
                        name: 'BoundInfo'
                    });
                }
            });
        },
        submit_proxy_company: function () {
            this.show_proxy_company_diag = false;
            this.$router.push({
                name: this.proxy_need_import ? 'PlanImport' : 'StuffPlan',
                params: {
                    type_id: this.proxy_type_id
                },
                query: {
                    proxy_company: this.proxy_company
                }
            });
        },
        open_operate: function (_single_type) {
            this.focus_type = _single_type;
        },
        submit_proxy: function (_type) {
            this.show_proxy_company_diag = true;
            this.proxy_type_id = _type.type_id;
        },
        submit_proxy_import: function (_type) {
            this.proxy_need_import = true;
            this.show_proxy_company_diag = true;
            this.proxy_type_id = _type.type_id;
        },
        do_operate: function (_op) {
            _op.operate(this.focus_type);
        },
        submit_notice: function (_val) {
            var vue_this = this;
            if (_val) {
                vue_this.$call_remote_process("company_management", 'set_notice', [vue_this.$cookies.get('pa_ssid'), vue_this.notice]).then(function (resp) {
                    if (resp) {
                        vue_this.init_company_data();
                    }
                });
            } else {
                vue_this.$call_remote_process("company_management", 'clear_notice', [vue_this.$cookies.get('pa_ssid')]).then(function () {
                    vue_this.init_company_data();
                });
            }
        },
        randomString: function (len) {
            len = len || 32;
            var $chars = 'ABCDEFGHJKMNPQRSTWXYZabcdefhijkmnprstwxyz2345678'; /****默认去掉了容易混淆的字符oOLl,9gq,Vv,Uu,I1****/
            var maxPos = $chars.length;
            var pwd = '';
            for (var i = 0; i < len; i++) {
                pwd += $chars.charAt(Math.floor(Math.random() * maxPos));
            }
            return pwd;
        },
        readd_stuff(_stuff) {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", 'readd_type', [_stuff, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                if (resp) {
                    vue_this.init_company_data();
                }
            });
        },

        edit_stuff: function () {
            var vue_this = this;
            var stuff = Object.create(vue_this.stuff_in_edit);
            stuff.price = parseInt(stuff.price);
            vue_this.$call_remote_process("company_management", 'edit_type', [stuff, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                if (resp) {
                    vue_this.init_company_data();
                }
            }).finally(function () {
                vue_this.show_edit_stuff = false;
            });
        },
        show_out_edit_diag: function (_stuff) {
            this.stuff_in_edit = _stuff;
            this.show_edit_stuff = true;
        },
        show_out_remove_diag: function (_stuff) {
            var vue_this = this;
            var stuff = Object.create(_stuff);
            Dialog.confirm({
                    title: '确认下架',
                    message: '确定要下架' + _stuff.name + '吗？',
                })
                .then(() => {
                    stuff.price = parseInt(stuff.price);
                    vue_this.$call_remote_process("company_management", 'remove_type', [stuff, vue_this.$cookies.get('pa_ssid')]).then(function () {
                        vue_this.init_company_data();
                    });
                });
        },
        add_stuff: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", 'add_type', [vue_this.add_stuff_name, parseInt(vue_this.add_stuff_price), vue_this.add_stuff_last, this.$cookies.get("pa_ssid")]).then(function (resp) {
                if (resp) {
                    vue_this.init_company_data();
                }
            }).finally(function () {
                vue_this.show_add_stuff = false;
            });
        },
        get_type_detail: function (_id) {
            var vue_this = this;
            vue_this.all_type = [];
            this.$call_remote_process("stuff_info", 'get_stuff_detail', [_id, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.$set(vue_this.all_type, vue_this.all_type.length, resp);
                vue_this.all_type.sort((a, b) => {
                    return b.saling - a.saling;
                });
            });
        },
        init_company_data: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", 'get_all_type', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.show_operate = [];
                resp.forEach(function (element) {
                    vue_this.get_type_detail(element);
                    vue_this.show_operate.push(false);
                });
            });
            if (vue_this.$store.state.userinfo.company) {
                vue_this.$call_remote_process("company_management", 'get_notice', [vue_this.$store.state.userinfo.company]).then(function (resp) {
                    vue_this.notice = resp;
                });
            }
            vue_this.$call_remote_process("company_management", 'get_real_access', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.real_access_users, index, element);
                });
            });
            vue_this.$call_remote_process("company_management", 'get_all_access', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.all_access_users, index, element);
                });
            });
        }
    },
    beforeMount: function () {
        this.init_company_data();
        var vue_this = this;
        vue_this.$call_remote_process("company_management", 'get_company_logo', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
            vue_this.company_logo = resp;
        });
        vue_this.init_vichele_statistices();
        vue_this.init_tomorrow_vichele();
    },
    watch: {
        "$store.state.userinfo.company": function (_val) {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", 'get_notice', [_val]).then(function (resp) {
                vue_this.notice = resp;
            });
        },
    },
}
</script>

<style scoped>
.scan_button_show {
    position: fixed;
    bottom: 10%;
    left: 10%;
    color: rgb(183, 196, 64);
    background-color: rgb(133, 228, 220);
}

.opt_btn_show {
    margin-left: 15px;
}

.notice_show {
    color: red;
    border-color: red;
    font-size: 16px;
}

.company_name_show {
    font-size: 25px;
    font-weight: bold;
    margin-top: 10px;
    margin-bottom: 10px;
}

.saling_tag_show {
    margin: 0 auto;
    width: 20px;
    line-height: 15px;
    color: red;
}

.grid_is_active {
    color: green;
}

.grid_is_not_active {
    color: red;
}
</style>
