<template>
<div class="plan_brief_show">
    <van-card class="stuff_card_show" :desc="company" :title="name" @click="nav_to_detail">
        <template #num>
            {{plan_count}}吨
        </template>
        <template #tags>
            <div>
                进厂时间：{{plan_time}}
            </div>
            <van-tag plain type="danger">{{cur_status}}</van-tag>
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

Vue.use(Tag);
Vue.use(Card);
export default {
    name: 'PlanBrief',
    props: {
        plan_id: Number,
        company_view: Boolean,
        conflict_reason: String,
    },
    data: function () {
        return {
            name: '',
            company: '',
            plan_count: 0.1,
            plan_time: '',
            vichele_info: [],
            created_time: '',
            status: 0,
        };
    },
    watch: {
        plan_id: function () {
            this.init_brief_data();
        }
    },
    computed: {
        cur_status: function () {
            var ret = "未确认";
            switch (this.status) {
                case 1:
                    ret = "待付款"
                    break;
                case 2:
                    ret = "已付款待确认";
                    break;
                case 3:
                    ret = "已收款待提货";
                    break;
                case 4:
                    ret = "已提货";
                    break;
                case 5:
                    ret = "已撤销";
                    break;
                default:
                    break;
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
                resp.vichele_info.forEach((element, index) => {
                    vue_this.$set(vue_this.vichele_info, index, element);
                });
                vue_this.created_time = vue_this.formatDateTime(new Date(resp.created_time * 1000));
                vue_this.name = resp.name;
                vue_this.status = resp.status;
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
    font-size:18px;
    line-height: 20px;
}
.stuff_card_show /deep/ .van-card__desc {
    font-size: 16px;
    font-weight: bold;
}
.conflict_show {
    color:red;
}
</style>
