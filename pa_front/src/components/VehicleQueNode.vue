<template>
<div class="vehicle_que_node_show">
    <van-cell :value="vq_node.stuff_name" :label="status_string(vq_node.status_code)" center>
        <template #icon>
            <van-icon name="clock-o" size="20" color="blue" v-if="!vq_node.has_called" />
            <div v-else>
                <van-icon name="pause-circle-o" size="20" color="red" v-if="vq_node.need_confirm" />
                <van-icon name="passed" v-else size="20" color="green" />
            </div>
        </template>
        <template #title>
            <div class="vehicle_plate_show">
                {{vq_node.plate}}
            </div>
            <div class="vehicle_plate_show" v-if="vq_node.back_plate">
                {{vq_node.back_plate}}
            </div>
        </template>
        <template #right-icon>
            <van-button v-if="!vq_node.has_called" size="small" type="info" @click="do_call(true)">叫号</van-button>
            <div v-else>
                <van-button size="small" type="warning" @click="do_call(false)">取消叫号</van-button>
                <van-button size="small" type="danger" @click="do_pass()">过号</van-button>
            </div>
        </template>
    </van-cell>
    <van-cell :title="vq_node.driver_name">

        <a :href="'tel:'+vq_node.driver_phone">{{ vq_node.driver_phone}}</a>
        <template #label>
            <div>{{vq_node.stuff_name}}</div>
            <div>排号时间：{{vq_node.check_in_time}}</div>
            <div v-if="vq_node.has_called">叫号时间：{{vq_node.call_time}}</div>
        </template>
        <template #right-icon v-if="vq_node.has_called && vq_node.status_code <= 3">
            <div v-if="vq_node.need_confirm">
                <van-button size="small" type="primary" @click="open_seal_no_diag(true)">铅封确认</van-button>
                <van-button size="small" type="warning" @click="xy_confirm()">泄压确认</van-button>
            </div>
            <van-button v-else size="small" type="danger" @click="open_seal_no_diag(false)">取消确认</van-button>
        </template>
    </van-cell>
    <van-cell title="一次称重" v-if="vq_node.status_code == 3">{{vq_node.p_weight}}</van-cell>
    <van-dialog v-model="seal_no_diag" title="输入铅封" closeOnClickOverlay @confirm="do_confirm(true)">
        <van-field v-model="seal_no" label="铅封号" />
    </van-dialog>
</div>
</template>

<script>
export default {
    name: 'VehicleQueNode',
    data: function () {
        return {
            status_string: function (status_code) {
                var ret = "未入场";
                switch (status_code) {
                    case 2:
                        ret = "已入场";
                        break;
                    case 3:
                        ret = "正在装卸货";
                        break;
                    case 4:
                        ret = "称重完成，等待出场";
                        break;
                    default:
                        break;
                }

                return ret;
            },
            seal_no: '',
            seal_no_diag: false,
        };
    },
    props: {
        vq_node: Object,
    },
    methods: {
        pub_event: function () {
            this.$emit("refresh");
        },
        do_call: function (is_call) {
            var vue_this = this;
            vue_this.$call_remote_process("open_api", 'field_queue_call', [vue_this.$store.state.userinfo.phone, vue_this.vq_node.id, is_call], vue_this.$store.state.zc_rpc_url).then(function (resp) {
                if (resp) {
                    vue_this.pub_event();
                }
            });
        },
        do_pass: function () {
            var vue_this = this;
            vue_this.$call_remote_process("open_api", 'field_queue_pass', [vue_this.$store.state.userinfo.phone, vue_this.vq_node.id], vue_this.$store.state.zc_rpc_url).then(function (resp) {
                if (resp) {
                    vue_this.pub_event();
                }
            });
        },
        open_seal_no_diag: function (is_confirm) {
            if (is_confirm) {
                this.seal_no_diag = true;
            } else {
                this.do_confirm(is_confirm);
            }
        },
        xy_confirm: function () {
            this.seal_no = "正在泄压";
            this.do_confirm(true);
        },
        do_confirm: async function (is_confirm) {
            var vue_this = this;
            if (!is_confirm) {
                vue_this.seal_no = "";
            }
            try {
                var seal_resp = true;
                if (is_confirm) {
                    seal_resp = await vue_this.$call_remote_process("open_api", 'field_queue_set_seal', [vue_this.$store.state.userinfo.phone, vue_this.vq_node.id, vue_this.seal_no], vue_this.$store.state.zc_rpc_url);
                }
                if (seal_resp) {
                    await vue_this.$call_remote_process("open_api", 'field_queue_confirm', [vue_this.$store.state.userinfo.phone, vue_this.vq_node.id, is_confirm], vue_this.$store.state.zc_rpc_url);
                }
            } catch (error) {
                console.log(error);
            }
            vue_this.seal_no_diag = false;
            vue_this.pub_event();
        },
    },
}
</script>

<style>
.vehicle_que_node_show {
    border: 1px solid rgb(165, 49, 49);
    border-radius: 6px;
}

.vehicle_plate_show {
    height: 20px;
    border: 2px solid black;
    border-radius: 4px;
    display: inline-block;
    font-size: 16px;
    padding-bottom: 3px;
    margin-left: 8px;
    background-color: rgba(245, 199, 34, 0.978);
}
</style>
