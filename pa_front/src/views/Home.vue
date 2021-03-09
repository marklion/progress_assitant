<template>
<div class="home">
    <h1>{{company_name()}}</h1>
    <van-row align="center" type="flex">
        <van-col>
            <van-image round width="60px" height="60px" :src="$store.state.userinfo.logo" />
        </van-col>
        <van-col>
            <div class="name_show">{{$store.state.userinfo.name}}</div>
            <div class="role_show">{{$store.state.userinfo.role}}</div>
        </van-col>
    </van-row>

    <van-tabs v-model="active">
        <van-tab title="待处理">
            <ticket-brief v-for="single_brief in all_tickets.need_i_handle" :key="single_brief.ticket_number" :ticket_number="single_brief.ticket_number" :creator="single_brief.creator" :assignee_role="single_brief.assignee_role" :timestamp="single_brief.timestamp" :app_name="single_brief.app_name" :next_step_name="single_brief.next_step_name"></ticket-brief>
        </van-tab>
        <van-tab title="我创建的">
            <ticket-brief v-for="single_brief in all_tickets.created_by_me" :key="single_brief.ticket_number" :ticket_number="single_brief.ticket_number" :creator="single_brief.creator" :assignee_role="single_brief.assignee_role" :timestamp="single_brief.timestamp" :app_name="single_brief.app_name" :next_step_name="single_brief.next_step_name"></ticket-brief>
        </van-tab>
        <van-tab title="我参与的">
            <ticket-brief v-for="single_brief in all_tickets.operated_by_me" :key="single_brief.ticket_number" :ticket_number="single_brief.ticket_number" :creator="single_brief.creator" :assignee_role="single_brief.assignee_role" :timestamp="single_brief.timestamp" :app_name="single_brief.app_name" :next_step_name="single_brief.next_step_name"></ticket-brief>
        </van-tab>
    </van-tabs>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Button
} from 'vant';
import {
    Image as VanImage
} from 'vant';
import {
    Col,
    Row
} from 'vant';
import TicketBrief from '../components/TicketBrief.vue'
import {
    Tab,
    Tabs
} from 'vant';

Vue.use(Tab);
Vue.use(Tabs);
Vue.use(Col);
Vue.use(Row);

Vue.use(VanImage);
Vue.use(Button);
export default {
    name: 'Home',
    components: {
        "ticket-brief": TicketBrief,
    },
    data: function () {
        return {
            active: 0,
            company_name() {
                var ret = "未绑定公司";
                if (this.$store.state.userinfo.company) {
                    ret = this.$store.state.userinfo.company;
                }

                return ret;
            },
            all_tickets: {
                created_by_me: [],
                operated_by_me: [],
                need_i_handle: [],
            },
        };
    },
    watch: {
        "$store.state.userinfo": function (value) {
            if (!value.company) {
                this.$router.push({
                    name: 'BindCompany',
                    query: {
                        company: this.$route.query.company
                    }
                });
            }
        },
    },
    beforeMount() {
        var vue_this = this;
        vue_this.$axios.get('/tickets_brief/' + vue_this.$cookies.get('pa_ssid')).then(function (resp) {
            resp.data.result.created_by_me.forEach((element, index) => {
                vue_this.$set(vue_this.all_tickets.created_by_me, index, element);
            });
            resp.data.result.operated_by_me.forEach((element, index) => {
                vue_this.$set(vue_this.all_tickets.operated_by_me, index, element);
            });
            resp.data.result.need_i_handle.forEach((element, index) => {
                vue_this.$set(vue_this.all_tickets.need_i_handle, index, element);
            });
        }).catch(function (err) {
            console.log(err);
        });
    },
    methods: {},
}
</script>
