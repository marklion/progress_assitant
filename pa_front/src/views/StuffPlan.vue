<template>
<div class="stuff_plan_show">
    <stuff-title :type_id="parseInt($route.params.type_id)" :buy_company="buy_company()"></stuff-title>
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
import {
    Button
} from 'vant';
Vue.use(Button);
Vue.use(Field);
Vue.use(Dialog);
Vue.use(NoticeBar);
Vue.use(Cell);
Vue.use(CellGroup);
import StuffInfoSubmit from '../components/StuffInfoSubmit.vue'
import StuffTitle from '../components/StuffTitle.vue'
export default {
    name: 'StuffPlan',
    components: {
        "stuff-info-submit": StuffInfoSubmit,
        "stuff-title": StuffTitle,
    },
    data: function () {
        return {
            stuff_brief: {
                name: '',
                price: 0.0,
                type_id: 0,
            },
            min_time: new Date(),
            buy_company: function () {
                var ret = this.$store.state.userinfo.company;
                if (this.$route.query.proxy_company) {
                    ret = this.$route.query.proxy_company;
                }

                return ret;
            },
        };
    },
    beforeMount: function () {
        var vue_this = this;
        this.$call_remote_process("stuff_info", 'get_stuff_detail', [parseInt(vue_this.$route.params.type_id)]).then(function (resp) {
            vue_this.stuff_brief.name = resp.name;
            vue_this.stuff_brief.price = resp.price;
            vue_this.stuff_brief.type_id = resp.type_id;
        });
    },
}
</script>

<style scoped>
</style>
