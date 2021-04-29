<template>
<div class="stuff_plan_show">
    <van-notice-bar v-if="company_notice" left-icon="volume-o" :text="company_notice" />
    <van-dialog v-model="show_notice_diag" v-if="company_notice" title="公告" :message="company_notice">
    </van-dialog>
    <van-cell :value="stuff_brief.name" center title="计划货品" :label="stuff_brief.company">
        <template #right-icon>
            <van-button class="preview_btn_show" size="small" type="info" plain @click="preview_company_attachment">查看卖方资质</van-button>
        </template>
    </van-cell>
    <van-field v-model="stuff_brief.company_address" rows="1" autosize label="公司地址" type="textarea" readonly>
    </van-field>
    <van-field v-model="stuff_brief.company_contact" rows="1" autosize label="联系方式" type="textarea" readonly>
    </van-field>
    <stuff-info-submit :proxy_company="$route.query.proxy_company" :type_id="stuff_brief.type_id" :is_create="true" :min_time="min_time" :orig_name="stuff_brief.name" :orig_price="stuff_brief.price"></stuff-info-submit>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Cell,
    CellGroup
} from 'vant';
import {
    NoticeBar
} from 'vant';
import {
    Dialog
} from 'vant';

import {
    Field
} from 'vant';
import { Button } from 'vant';
import { ImagePreview } from 'vant';
Vue.use(Button);
Vue.use(Field);
Vue.use(Dialog);
Vue.use(NoticeBar);
Vue.use(Cell);
Vue.use(CellGroup);
import StuffInfoSubmit from '../components/StuffInfoSubmit.vue'
export default {
    name: 'StuffPlan',
    components: {
        "stuff-info-submit": StuffInfoSubmit,
    },
    data: function () {
        return {
            stuff_brief: {
                name: '',
                company: '',
                price: 0.0,
                type_id: 0,
                company_address: '',
                company_contact: '',
            },
            min_time: {},
            company_notice: '',
            show_notice_diag:true,
        };
    },
    beforeMount: function () {
        var vue_this = this;
        this.$call_remote_process("stuff_info", 'get_stuff_detail', [parseInt(vue_this.$route.params.type_id)]).then(function (resp) {
            vue_this.stuff_brief.name = resp.name;
            vue_this.stuff_brief.company = resp.company;
            vue_this.stuff_brief.price = resp.price;
            vue_this.stuff_brief.type_id = resp.type_id;
            vue_this.stuff_brief.company_address = resp.company_address;
            vue_this.stuff_brief.company_contact = resp.company_contact;
            vue_this.$call_remote_process("company_management", 'get_notice', [resp.company]).then(function (resp) {
                vue_this.company_notice = resp;
            });
        });
        this.min_time = new Date();
    },
    methods: {
        preview_company_attachment: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "get_attachment", [vue_this.stuff_brief.company]).then(function (resp) {
                if (resp)
                {
                    ImagePreview([vue_this.$remote_url + resp]);
                }
            });
        },
    },
}
</script>

<style scoped>
.preview_btn_show {
    margin-left: 15px;
}
</style>
