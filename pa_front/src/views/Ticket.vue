<template>
<div class="ticket_sh">
    <div id="ticket_content" v-if="!whole_pic">
        <h3 style="text-align: center;">{{ticket_detail.title.split('\n')[0]}}</h3>
        <h3 style="text-align: center;">{{ticket_detail.title.split('\n')[1]}}</h3>
        <div class="ticket_body">
            <ticket-item item_name="磅单号" :item_value="ticket_detail.ticket_no"></ticket-item>
            <ticket-item item_name="客户" :item_value="ticket_detail.customer_name"></ticket-item>
            <ticket-item item_name="供应商" :item_value="ticket_detail.supplier_name"></ticket-item>
            <ticket-item item_name="车牌号" :item_value="ticket_detail.main_vichele_number"></ticket-item>
            <ticket-item item_name="挂车号" :item_value="ticket_detail.behind_vichele_number"></ticket-item>
            <ticket-item item_name="承运商" :item_value="ticket_detail.transfor_company"></ticket-item>
            <ticket-item item_name="铅封号" :item_value="ticket_detail.seal_no"></ticket-item>
            <ticket-item item_name="产品名称" :item_value="ticket_detail.stuff_name"></ticket-item>
            <ticket-item item_name="皮重" :item_value="ticket_detail.p_weight"></ticket-item>
            <ticket-item item_name="毛重" :item_value="ticket_detail.m_weight"></ticket-item>
            <ticket-item item_name="净重" :item_value="ticket_detail.j_weight"></ticket-item>
            <ticket-item item_name="过皮时间" :item_value="ticket_detail.p_date"></ticket-item>
            <ticket-item item_name="过毛时间" :item_value="ticket_detail.m_date"></ticket-item>
            <ticket-item item_name="联系方式" :item_value="company_contact"></ticket-item>
        </div>
        <van-row>
            <van-col>
                <vue-qr :text="qr_url()" :size="150"></vue-qr>
            </van-col>
            <van-col>
                <div class="ticket_stamp_show" v-if="ticket_stamp">
                    <img width="180" height="180" :src="$remote_url + ticket_stamp" crossorigin="anonymous" />
                </div>
            </van-col>
        </van-row>
        <div>关注“掌易助理”公众号</div>
        <div>发送电子磅单图片可以验真</div>
    </div>
    <div v-else>
        <van-divider>长按磅单，保存到手机相册</van-divider>
        <van-image id="ticket_picture" :src="whole_pic"></van-image>
    </div>
    <div v-if="$store.state.is_login && !$store.state.userinfo.buyer && $store.state.userinfo.company == ticket_detail.title.split('\n')[0]">
        <van-button round block type="warning" @click="modify_ticket_show = true">修改</van-button>
    </div>
    <van-dialog v-model="modify_ticket_show" title="修改磅单" :showConfirmButton="false" closeOnClickOverlay>
        <van-form @submit="update_ticket">
            <van-field v-model="submit_ticket.supplier_name" name="供应商" label="供应商" placeholder="请输入供应商名称" :rules="[{ required:true, message:'请输入供应商名称'}]" />
            <van-field v-model="submit_ticket.transfor_company" name="承运商" label="承运商" placeholder="请输入承运商名称" :rules="[{ required:true, message:'请输入承运商名称'}]" />
            <van-field v-model="submit_ticket.stuff_name" name="货种" label="货种" placeholder="请输入货种" :rules="[{ required:true, message:'请输入货种'}]" />
            <van-field v-model="submit_ticket.p_weight" name="皮重" label="皮重" placeholder="请输入皮重" :rules="[{ required:true, message:'请输入皮重'}]" />
            <van-field v-model="submit_ticket.m_weight" name="毛重" label="毛重" placeholder="请输入毛重" :rules="[{ required:true, message:'请输入毛重'}]" />
            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">提交</van-button>
            </div>
        </van-form>
    </van-dialog>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Image as VanImage
} from 'vant';
import VueQr from 'vue-qr'
import {
    Button
} from 'vant';
import html2canvas from 'html2canvas'
import {
    Divider
} from 'vant';
import {
    Dialog
} from 'vant';
import {
    Form
} from 'vant';
import {
    Field
} from 'vant';

Vue.use(Form);
Vue.use(Field);
// 全局注册
Vue.use(Dialog);

Vue.use(Divider);
Vue.use(Button);
Vue.use(VanImage);
import TicketItem from "../components/TicketItem.vue"
export default {
    name: 'Ticket',
    components: {
        "ticket-item": TicketItem,
        VueQr,
    },
    data: function () {
        return {
            qr_url: function () {
                return window.location.href;
            },
            modify_ticket_show: false,
            submit_ticket: {},
            whole_pic: '',
            ticket_stamp: '',
            ticket_detail: {
                title: '',
                ticket_no: '',
                customer_name: '',
                supplier_name: '',
                transfor_company: '',
                main_vichele_number: '',
                behind_vichele_number: '',
                seal_no: '',
                stuff_name: '',
                p_weight: '',
                m_weight: '',
                j_weight: '',
                m_data: '',
                p_date: '',
            },
            company_contact: '',
        };
    },
    methods: {
        update_ticket: function () {
            var vue_this = this;
            vue_this.$call_remote_process("open_api_management", "modify_vehicle_info_from_ticket", [vue_this.$cookies.get('pa_ssid'), vue_this.submit_ticket]).then(function (resp) {
                if (resp) {
                    vue_this.$router.go(0);
                    vue_this.modify_ticket_show = false;
                }
            });
        },
        make_pic: function () {
            var vue_this = this;
            var ticket_pic = document.querySelector('#ticket_content');
            html2canvas(ticket_pic, {
                useCORS: true
            }).then(function (canvas) {
                var img = canvas.toDataURL('image/jpg');
                vue_this.whole_pic = img;
            });
        },
        init_ticket: function () {
            var vue_this = this;
            vue_this.$call_remote_process("open_api_management", "get_vehicle_info_by_id", [vue_this.$route.params.id]).then(function (resp) {
                vue_this.ticket_detail = resp;
                vue_this.$call_remote_process("company_management", "get_stamp_pic", [resp.title.split('\n')[0]]).then(function (stamp_path) {
                    vue_this.ticket_stamp = stamp_path;
                    vue_this.$call_remote_process("company_management", "get_address_contact", [resp.title.split('\n')[0]]).then(function (address_contact) {
                        vue_this.company_contact = address_contact.contact;
                    }).finally(function () {
                        vue_this.make_pic();
                        vue_this.submit_ticket = {
                            ...vue_this.ticket_detail
                        };
                    });
                });
            });
        },
    },
    beforeMount: function () {
        this.init_ticket();
    }
}
</script>

<style scoped>
.ticket_body {
    padding: 20px;
}

.ticket_stamp_show {
    text-align: right;
    padding-right: 40px;
}
</style>
