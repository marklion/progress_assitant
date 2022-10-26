<template>
<div class="admin_show">
    <van-tabs v-model="active">
        <van-tab title="待审批">
            <van-cell v-for="(single_apply, index) in all_apply " :key="index" :label="'手机号:' + single_apply.phone" :title="single_apply.name + ' 加入公司申请'">
                <template #right-icon>
                    <van-row v-if="single_apply.status == 0" type="flex" justify="center" align="center" :gutter="5">
                        <van-col>
                            <van-button type="primary" @click="submit_approve(single_apply, true)">批准</van-button>
                        </van-col>
                        <van-col>
                            <van-button type="danger" @click="submit_approve(single_apply, false)">驳回</van-button>
                        </van-col>
                    </van-row>
                    <div v-else-if="single_apply.status == 1">
                        已批准
                    </div>
                    <div v-else>
                        已驳回
                    </div>
                </template>
                <template #icon>
                    <van-image round width="40px" height="40px" :src="$remote_url + single_apply.logo" />
                </template>
            </van-cell>
        </van-tab>
        <van-tab title="所有员工">
            <van-cell v-for="(single_user, index) in all_user" :key="index" :label="'手机号' + single_user.phone">
                <template #title>
                    <span>{{single_user.name}}</span>
                    <van-tag v-if="single_user.is_read_only" type="warning">只读</van-tag>
                </template>
                <template #right-icon>
                    <van-row :gutter="10" type="flex" align="center">
                        <van-col>
                            <van-button type="warning" plain size="small" @click="set_user_read_only(single_user)">更改只读</van-button>
                        </van-col>
                        <van-col>
                            <van-popover v-model="showPopover[index]" trigger="click" :actions="actions" @select="onSelect" @open="focus_user = single_user.user_id" @close="focus_user = 0">
                                <template #reference>
                                    <van-button type="primary" plain size="small">{{group_name(single_user.groupid)}}</van-button>
                                </template>
                            </van-popover>
                        </van-col>
                        <van-col>
                            <van-button type="danger" plain size="small" @click="remove_user(single_user)">移除用户</van-button>
                        </van-col>
                    </van-row>
                </template>
            </van-cell>
        </van-tab>
        <van-tab title="营业时间">
            <van-field readonly clickable :value="work_start_time" label="上班时间" placeholder="点击选择时间" @click="show_start_Picker = true" />
            <van-popup v-model="show_start_Picker" position="bottom">
                <van-datetime-picker type="time" @confirm="on_start_Confirm" @cancel="show_start_Picker = false" />
            </van-popup>
            <van-field readonly clickable :value="work_end_time" label="下班时间" placeholder="点击选择时间" @click="show_end_Picker = true" />
            <van-popup v-model="show_end_Picker" position="bottom">
                <van-datetime-picker type="time" @confirm="on_end_Confirm" @cancel="show_end_Picker = false" />
            </van-popup>
        </van-tab>
        <van-tab title="磅单印章">
            <van-image width="300" height="300" :src="$remote_url + stamp_pic">
                <template v-slot:error>
                    <div>无印章</div>
                    <div>
                        <van-uploader :after-read="upload_stamp" accept="image/*">
                            <van-button icon="plus" size="small" type="primary">上传</van-button>
                        </van-uploader>
                    </div>
                </template>
            </van-image>
            <van-button v-if="stamp_pic" type="danger" block size="small" @click="del_stamp">删除</van-button>
        </van-tab>
    </van-tabs>

</div>
</template>

<script>
import Vue from 'vue';
import {
    Cell,
    CellGroup
} from 'vant';
import {
    Image as VanImage
} from 'vant';
import {
    Button
} from 'vant';
import {
    Col,
    Row
} from 'vant';
import {
    Tab,
    Tabs
} from 'vant';
import {
    Dialog
} from 'vant';
import {
    Field
} from 'vant';
import {
    Popup
} from 'vant';
import {
    DatetimePicker
} from 'vant';
import {
    Popover
} from 'vant';
import {
    Uploader
} from 'vant';
import {
    compressAccurately
} from 'image-conversion';

Vue.use(Uploader);
Vue.use(Popover);
Vue.use(DatetimePicker);
Vue.use(Popup);
Vue.use(Field);
Vue.use(Tab);
Vue.use(Tabs);
Vue.use(Col);
Vue.use(Row);
Vue.use(Button);
Vue.use(VanImage);
Vue.use(Cell);
Vue.use(CellGroup);
export default {
    name: 'Admin',
    data: function () {
        return {
            stamp_pic: '',
            all_apply: [],
            active: 0,
            all_user: [],
            work_start_time: 0,
            show_start_Picker: false,
            work_end_time: 0,
            show_end_Picker: false,
            showPopover: [],
            group_name(_id) {
                var ret = "未分组";
                switch (_id) {
                    case 1:
                        ret = "销售";
                        break;
                    case 2:
                        ret = "采购";
                        break;
                    default:
                        break;
                }

                return ret;
            },
            actions: [{
                text: '未分组',
                groupid: 0
            }, {
                text: '销售',
                groupid: 1
            }, {
                text: '采购',
                groupid: 2
            }],
            focus_user: 0,
        };
    },
    methods: {
        set_user_read_only: function (_user) {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "change_user_read_only", [vue_this.$cookies.get("pa_ssid"), _user.user_id]).then(function (resp) {
                if (resp) {
                    vue_this.init_all_user();
                }
            });
        },
        del_stamp: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "del_stamp_pic", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                if (resp) {
                    vue_this.init_stamp();
                }
            });
        },
        init_stamp: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "get_stamp_pic", [vue_this.$store.state.userinfo.company]).then(function (resp) {
                vue_this.stamp_pic = resp;
            });
        },
        convert_2_base64_send: function (_file) {
            var vue_this = this;
            var reader = new FileReader();
            reader.readAsDataURL(_file);
            reader.onloadend = function () {
                var result = this.result;
                var file_content = result.split(';base64,')[1];
                vue_this.$call_remote_process("company_management", "add_stamp_pic", [vue_this.$cookies.get('pa_ssid'), file_content]).then(function (resp) {
                    if (resp) {
                        vue_this.init_stamp();
                    }
                });
            };
        },
        upload_stamp: function (_file) {
            var vue_this = this;

            compressAccurately(_file.file, 400).then(function (res) {
                vue_this.convert_2_base64_send(res, false);
            });

        },
        onSelect: function (_item) {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "set_user_group", [vue_this.$cookies.get('pa_ssid'), vue_this.focus_user, _item.groupid]).then(function (resp) {
                if (resp) {
                    vue_this.init_all_user();
                }
            });
        },
        on_start_Confirm: function (_time) {
            var start_time = parseInt(_time.split(":")[0]) * 60 + parseInt(_time.split(":")[1]);
            var end_time = parseInt(this.work_end_time.split(":")[0]) * 60 + parseInt(this.work_end_time.split(":")[1]);
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "set_work_time", [vue_this.$cookies.get('pa_ssid'), start_time, end_time]).then(function (resp) {
                if (resp) {
                    vue_this.init_work_time();
                    vue_this.show_start_Picker = false;
                }
            })
        },
        on_end_Confirm: function (_time) {
            var start_time = parseInt(this.work_start_time.split(":")[0]) * 60 + parseInt(this.work_start_time.split(":")[1]);
            var end_time = parseInt(_time.split(":")[0]) * 60 + parseInt(_time.split(":")[1]);
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "set_work_time", [vue_this.$cookies.get('pa_ssid'), start_time, end_time]).then(function (resp) {
                if (resp) {
                    vue_this.init_work_time();
                    vue_this.show_end_Picker = false;
                }
            })
        },
        remove_user: function (_user) {
            var vue_this = this;
            Dialog.confirm({
                message: '确定要移除' + _user.name + '吗',
            }).then(() => {
                vue_this.$call_remote_process("company_management", "remove_user_from_company", [vue_this.$cookies.get('pa_ssid'), _user.user_id]).then(function (resp) {
                    if (resp) {
                        vue_this.init_all_user();
                    }
                });
            });
        },
        submit_approve: function (_apply, approve) {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", 'approve_apply', [_apply.apply_id, vue_this.$cookies.get('pa_ssid'), approve]).then(function (resp) {
                if (resp) {
                    vue_this.init_apply_info();
                }
            });
        },
        init_apply_info: function () {
            var vue_this = this;
            vue_this.all_apply = [];
            vue_this.$call_remote_process("company_management", 'get_all_apply', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.all_apply, index, element);
                });
            });
        },
        init_all_user: function () {
            var vue_this = this;
            vue_this.all_user = [];
            vue_this.showPopover = [];
            vue_this.$call_remote_process("company_management", "get_all_compay_user", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.all_user, index, element);
                    vue_this.$set(vue_this.showPopover, index, false);
                });
            });
        },
        init_work_time: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "get_work_time", [vue_this.$store.state.userinfo.company]).then(function (resp) {
                vue_this.work_start_time = parseInt(resp.start_time / 60) + ':' + resp.start_time % 60;
                vue_this.work_end_time = parseInt(resp.end_time / 60) + ':' + resp.end_time % 60;
            });
        },
    },
    beforeMount: function () {
        this.init_apply_info();
        this.init_all_user();
        this.init_work_time();
        this.init_stamp();
    },
}
</script>

<style>

</style>
