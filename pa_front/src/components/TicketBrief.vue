<template>
<div class="ticket_brief">
    <div class="ticket_number_show">流程号：{{ticket_number}}</div>
    <van-card :desc="'创建人：' + creator" :title="app_name">
        <template #num>
            <div class="time_stamp_show">{{timestamp}}</div>
        </template>
        <template #price>
            <div class="assignee_role_show">需要 {{assignee_role}} 处理</div>
        </template>
        <template #tags>
            <span>流程状态：</span>
            <span>
                <van-tag plain type="primary">{{next_step_name}}</van-tag>
            </span>
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
    name: 'TicketBrief',
    props: {
        ticket_number: String,
        creator: String,
        timestamp: String,
        assignee_role: String,
        app_name: String,
        next_step_name: String,
    },
}
</script>

<style scoped>
.ticket_number_show {
    background-color: rgb(157, 180, 189);
}
</style>
