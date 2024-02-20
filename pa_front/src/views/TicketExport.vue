<template>
<div>
    <div v-if="all_tickets.length > 0">
        <van-button type="primary" @click="downloadImages">单张下载</van-button>
        <van-button type="info" @click="downloadPageAsImage">整张下载</van-button>
    </div>
    <div class="ticket_show_window" v-for="(single_ticket, index) in all_tickets" :key="index">
        <ticket-content :ticket_id="single_ticket + 'S'" no_need_prompt></ticket-content>
    </div>
</div>
</template>

<script>
import TicketContent from '../components/TicketContent.vue'
import html2canvas from 'html2canvas';
export default {
    name: 'TicketExport',
    components: {
        'ticket-content': TicketContent,
    },
    data: function () {
        return {
            date_begin: '',
            date_end: '',
            ssid: '',
            all_tickets: []
        };
    },
    methods: {
        get_tickets: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "get_single_vid", [vue_this.ssid, vue_this.date_begin, vue_this.date_end]).then(function (resp) {
                if (resp) {
                    resp.forEach((element, index) => {
                        vue_this.$set(vue_this.all_tickets, index, element)
                    });
                } else {
                    vue_this.$toast("无信息");
                }
            });
        },
        downloadImages: function () {
            var elements = document.querySelectorAll('#ticket_picture > img');
            elements.forEach((element, index) => {
                var imgSrc = element.src;
                var link = document.createElement('a');
                link.href = imgSrc;
                link.download = 'image' + index + '.jpg';
                link.click();
            });
        },
        downloadPageAsImage: function () {
            html2canvas(document.body).then(canvas => {
                let imgData = canvas.toDataURL();
                let link = document.createElement('a');
                link.href = imgData;
                link.download = 'page.png';
                link.click();
            });
        },

    },
    beforeMount: function () {
        this.date_begin = this.$route.query.date_begin;
        this.date_end = this.$route.query.date_end;
        this.ssid = this.$route.query.ssid;
        this.get_tickets();
    },
}
</script>

<style scoped>
.ticket_show_window {
    display: inline-block;
    width: 350px;
    height: 700px;
    background-color: #f8bbbb;
    z-index: 999;
}
</style>
