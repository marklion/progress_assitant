<template>
<div class="vehicle_que_show">
    <div>
        已叫号{{called_count}}车,未叫号{{wait_for_call_count}}车
    </div>
    <div>
        已进厂{{dropping}}车,可以出厂{{dropped}}车
    </div>
    <van-search v-model="search_key" placeholder="过滤车号" />
    <vehicle-que-node v-for="(single_vqn, index) in vehicle_need_show" :key="index" :vq_node="single_vqn" @refresh="init_vq">
    </vehicle-que-node>
</div>
</template>

<script>
import VehicleQueNode from '../components/VehicleQueNode.vue'
import PinyinMatch from 'pinyin-match';
export default {
    name: "VehicleQue",
    data: function () {
        return {
            all_vq: [],
            search_key: '',
        };
    },
    computed: {
        called_count: function () {
            var ret = 0;
            this.all_vq.forEach(item => {
                if (item.has_called) {
                    ret += 1;
                }
            });
            return ret;
        },
        wait_for_call_count: function () {
            var ret = 0;
            ret = this.all_vq.length - this.called_count;
            return ret;
        },
        dropping: function () {
            var ret = 0;
            this.all_vq.forEach(item => {
                if (item.status_code > 1) {
                    ret += 1;
                }
            });

            return ret;
        },
        dropped: function () {
            var ret = 0;
            this.all_vq.forEach(item => {
                if (!item.need_confirm) {
                    ret += 1;
                }
            });
            return ret;
        },
        vehicle_need_show: function () {
            var ret = [];
            if (this.search_key) {
                this.all_vq.forEach(element => {
                    if (PinyinMatch.match(element.plate, this.search_key)) {
                        ret.push(element);
                    }
                });
            } else {
                ret = this.all_vq;
            }
            return ret;
        },
    },
    components: {
        "vehicle-que-node": VehicleQueNode,
    },
    watch: {
        "$store.state.zc_rpc_url": function () {
            this.init_vq();
        },
        "$store.state.zczh_back_end": function () {
            this.init_vq();
        },
    },
    methods: {
        init_vq: function () {
            var vue_this = this;
            if (vue_this.$store.state.zc_rpc_url) {
                vue_this.$call_remote_process_no_toast("open_api", 'get_queue_node', [vue_this.$store.state.userinfo.phone], vue_this.$store.state.zc_rpc_url).then(function (resp) {
                    vue_this.all_vq = [];
                    resp.forEach((element, index) => {
                        vue_this.$set(vue_this.all_vq, index, element);
                    });
                });
            } else if (vue_this.$store.state.zczh_back_end) {
                vue_this.$axios.post(vue_this.$store.state.zczh_back_end + "/api/order/get_registered_order", {}, {
                    headers: {
                        "Content-Type": "application/json",
                        "token": vue_this.$store.state.zczh_back_token,
                    }
                }).then(function (resp) {
                    vue_this.all_vq = [];
                    resp.data.result.forEach((element, index) => {
                        var tmp_v = {};
                        tmp_v.id = Number(element.order_number);
                        tmp_v.plate = element.plate_number;
                        tmp_v.back_plate = element.back_plate_number;
                        tmp_v.stuff_name = element.stuff_name;
                        tmp_v.driver_name = element.driver_name;
                        tmp_v.driver_phone = element.driver_phone;
                        tmp_v.p_weight = element.p_weight.toFixed(2);
                        tmp_v.m_weight = element.m_weight.toFixed(2);
                        tmp_v.seal_no = element.seal_no;
                        tmp_v.need_confirm = (element.confirm_info.operator_time.length == 0) ? true : false;
                        tmp_v.has_called = (element.call_info.operator_time.length == 0) ? false : true;
                        tmp_v.check_in_time = element.reg_info.operator_time;
                        tmp_v.call_time = element.call_info.operator_time;
                        tmp_v.status_code = element.status;
                        if (element.p_weight > 0) {
                            tmp_v.status_code = 3;
                        }
                        if (element.m_weight > 0) {
                            tmp_v.status_code = 4;
                        }
                        vue_this.$set(vue_this.all_vq, index, tmp_v);
                    });
                });
            }
        },
    },
    beforeMount: function () {
        this.init_vq();
    },
}
</script>

<style>

</style>
