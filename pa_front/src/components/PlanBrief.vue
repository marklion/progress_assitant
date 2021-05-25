<template>
<div class="plan_brief_show">
    <div class="order_number_show">
        <van-row type="flex" align="center" justify="space-between">
            <van-col>
                计划单号：{{order_number}}
            </van-col>
            <van-col v-if="can_be_copy">
                <van-button size="small" plain type="info" :to="{name:'PlanCopy', params:{plan_id:plan_id}}">复制</van-button>
            </van-col>
        </van-row>
    </div>
    <van-card class="stuff_card_show" :desc="company" :title="name" @click="nav_to_detail">
        <template #num v-if="plan_count != 0">
            {{plan_count}}吨
        </template>
        <template #tags>
            <div>
                计划进厂时间：{{plan_time}}
            </div>
            <van-tag plain type="danger">{{status_prompt}}</van-tag>
            <div v-if="need_show_deliver_time">
                实际出货时间：{{status_in_plan[3].timestamp}}
            </div>
        </template>
        <template #footer>
            创建时间:{{created_time}}
        </template>
        <template #bottom v-if="conflict_reason">
            <p class="conflict_show">{{conflict_reason}}</p>
        </template>
    </van-card>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Card
} from 'vant';
import {
    Tag
} from 'vant';
import {
    Col,
    Row
} from 'vant';
import {
    Button
} from 'vant';

Vue.use(Button);
Vue.use(Col);
Vue.use(Row);
Vue.use(Tag);
Vue.use(Card);
export default {
    name: 'PlanBrief',
    props: {
        plan_id: Number,
        company_view: Boolean,
        conflict_reason: String,
        status_prompt: String,
    },
    data: function () {
        return {
            name: '',
            company: '',
            plan_count: 0.1,
            plan_time: '',
            created_time: '',
            status: 0,
            status_in_plan: [],
            is_cancel: false,
            order_number: '',
            is_proxy: false,
        };
    },
    watch: {
        plan_id: function () {
            this.init_brief_data();
        }
    },
    computed: {
        can_be_copy: function () {
            var ret = false;
            if (this.$store.state.userinfo.buyer || this.is_proxy) {
                ret = true;
            }

            return ret;
        },
        need_show_deliver_time: function () {
            var ret = false;
            if (this.status_in_plan.length > 3 && this.status_in_plan[3].timestamp) {
                ret = true;
            }
            return ret;
        },
    },
    methods: {
        nav_to_detail: function () {
            this.$router.push({
                name: 'PlanDetail',
                params: {
                    plan_id: this.plan_id
                }
            });
        },
        formatDateTime: function (date) {
            var y = date.getFullYear();
            var m = date.getMonth() + 1;
            m = m < 10 ? ('0' + m) : m;
            var d = date.getDate();
            d = d < 10 ? ('0' + d) : d;
            var h = date.getHours();
            h = h < 10 ? ('0' + h) : h;
            var minute = date.getMinutes();
            minute = minute < 10 ? ('0' + minute) : minute;
            var second = date.getSeconds();
            second = second < 10 ? ('0' + second) : second;
            return y + '-' + m + '-' + d + ' ' + h + ':' + minute + ':' + second;
        },
        init_brief_data: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", 'get_plan', [vue_this.plan_id]).then(function (resp) {
                vue_this.plan_count = resp.count;
                vue_this.plan_time = resp.plan_time;
                vue_this.created_time = vue_this.formatDateTime(new Date(resp.created_time * 1000));
                vue_this.name = resp.name;
                vue_this.status = resp.status;
                vue_this.is_cancel = resp.is_cancel;
                if (resp.proxy_company.length > 0) {
                    vue_this.is_proxy = true;
                }
                if (false == vue_this.company_view) {
                    vue_this.company = resp.sale_company;
                } else {
                    vue_this.company = resp.buy_company;
                    if (resp.proxy_company.length > 0) {
                        vue_this.company = vue_this.company + '(手工)';
                    } else {
                        vue_this.company = vue_this.company + '(' + resp.created_user_name + ')';
                    }
                }
                vue_this.order_number = resp.created_time.toString() + vue_this.plan_id.toString();
                vue_this.get_status_in_plan();
            });
        },
        get_status_in_plan: function () {
            var vue_this = this;
            vue_this.status_in_plan = [];
            vue_this.$call_remote_process("stuff_plan_management", "get_status_rule", [vue_this.plan_id]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.status_in_plan, index, element);
                });
            });
        },

    },
    beforeMount: function () {
        this.init_brief_data();
    },
}
</script>

<style scoped>
.stuff_card_show /deep/ .van-card__title {
    font-size: 18px;
    line-height: 20px;
}

.stuff_card_show /deep/ .van-card__desc {
    font-size: 16px;
    font-weight: bold;
}

.conflict_show {
    color: red;
}

.plan_brief_show {
    border: 1px solid rgb(165, 49, 49);
    border-radius: 6px;
    margin-bottom: 2px;
}

.order_number_show {
    padding-left: 12px;
    margin-top: 3px;
    margin-bottom: 3px;
    margin-left: 5px;
    margin-right: 5px;
    background-color: rgb(187, 187, 187);
    color: rgb(0, 5, 75);
}
</style>
