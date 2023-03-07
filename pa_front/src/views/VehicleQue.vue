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
