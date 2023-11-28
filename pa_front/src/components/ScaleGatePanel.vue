<template>
<div class="scale_gate_panel_show">
    <van-swipe-cell>
        <van-card :desc="is_scale?scale_status:''" currency="">
            <template #title>
                <div style="font-size: 20px">
                    {{name}}
                </div>
            </template>
            <template #thumb>
                <div v-if="is_scale">
                    <van-icon size="80" name="https://www.d8sis.cn/pa_web/logo_res/scale_icon.png" />
                </div>
                <div v-else>
                    <van-icon size="80" name="https://www.d8sis.cn/pa_web/logo_res/gate_icon.png" />
                </div>
            </template>
            <template #tags v-if="is_scale">
                <van-tag v-if="!enter_is_close" plain type="danger">入口未关</van-tag>
                <van-tag v-else plain type="success">入口已关</van-tag>
                <van-tag v-if="!exit_is_close" plain type="danger">出口未关</van-tag>
                <van-tag v-else plain type="success">出口已关</van-tag>
            </template>
            <template #price v-if="is_scale">
                <div>
                    {{cur_weight}}吨
                </div>
            </template>
            <template #footer>
                <van-button round type="warning" text="出口拍照" size="mini" @click="take_pic(name, false)" />
                <van-button size="mini" round type="primary" @click="gate_ctrl(name, false, true)">出口开</van-button>
                <van-button size="mini" round type="danger" @click="gate_ctrl(name, false, false)">出口关</van-button>
                <van-button size="mini" round type="primary" @click="gate_ctrl(name, true, true)">入口开</van-button>
                <van-button size="mini" round type="danger" @click="gate_ctrl(name, true, false)">入口关</van-button>
                <van-button round type="warning" text="入口拍照" size="mini" @click="take_pic(name, true)" />
            </template>
            <template #num>
                左右滑动更多操作
            </template>
        </van-card>
        <template #left>
            <van-button square type="info" text="出口触发" class="swip-button" @click="trigger_diag_show=true;focus_is_enter = false" />
            <van-button square type="info" text="入口触发" class="swip-button" @click="trigger_diag_show=true;focus_is_enter = true" />
        </template>
        <template #right>
            <van-button square type="primary" v-if="is_scale" text="确认重量" class="swip-button" @click="confirm_scale(name)" />
            <van-button v-if="is_scale" square type="danger" text="重置" class="swip-button" @click="reset_scale(name)" />
        </template>
    </van-swipe-cell>

    <van-dialog v-model="trigger_diag_show" title="触发识别" closeOnClickOverlay @confirm="trigger_cap(name, focus_is_enter, trigger_vehicle)">
        <van-field v-model="trigger_vehicle" label="车牌" placeholder="不输入车牌则拍照触发" />
    </van-dialog>
</div>
</template>

<script>
import {
    ImagePreview
} from 'vant';
export default {
    name: "ScaleGatePanel",
    props: {
        is_scale: Boolean,
        enter_is_close: Boolean,
        exit_is_close: Boolean,
        scale_status: String,
        cur_weight: String,
        name: String,
    },
    data: function () {
        return {
            trigger_diag_show: false,
            focus_is_enter: false,
            trigger_vehicle: '',
        };
    },
    methods: {
        pub_evt: function () {
            this.$emit("refresh");
        },
        found_device_to_execute: function (find_func, exec_func) {
            var vue_this = this;
            vue_this.$axios.post(vue_this.$store.state.zczh_back_end + "/api/get_scale_sm_info", {}, {
                headers: {
                    "Content-Type": "application/json",
                    "token": vue_this.$store.state.zczh_back_token,
                }
            }).then(function (resp) {
                resp.data.result.forEach((element) => {
                    if (element.set_info.name == vue_this.name) {
                        var id = find_func(element.set_info);
                        exec_func(id);
                    }
                });
            });
        },
        gate_ctrl: function (name, is_enter, is_open) {
            var vue_this = this;
            if (vue_this.$store.state.zczh_back_end) {
                this.found_device_to_execute(function (set_info) {
                    if (is_enter) {
                        return set_info.gate.front.id;
                    } else {
                        return set_info.gate.back.id;
                    }
                }, function (id) {
                    vue_this.$axios.post(vue_this.$store.state.zczh_back_end + "/api/gate_ctrl", {
                        device_id: id,
                        is_open: is_open,
                    }, {
                        headers: {
                            "Content-Type": "application/json",
                            "token": vue_this.$store.state.zczh_back_token,
                        }
                    }).then(function () {
                        vue_this.pub_evt();
                    });
                })
            } else {
                vue_this.$call_remote_process("open_api", 'do_device_opt_gate_control', [vue_this.$store.state.userinfo.phone, name, is_enter, is_open], vue_this.$store.state.zc_rpc_url).then(function () {
                    vue_this.pub_evt();
                });
            }
        },
        confirm_scale: function (name) {
            var vue_this = this;
            if (vue_this.$store.state.zczh_back_end) {
                this.found_device_to_execute(function (set_info) {
                    return set_info.id
                }, function (id) {
                    vue_this.$axios.post(vue_this.$store.state.zczh_back_end + "/api/confirm_scale", {
                        sm_id: id,
                    }, {
                        headers: {
                            "Content-Type": "application/json",
                            "token": vue_this.$store.state.zczh_back_token,
                        }
                    }).then(function () {
                        vue_this.pub_evt();
                    });
                });
            } else {
                vue_this.$call_remote_process("open_api", 'do_device_opt_confirm_scale', [vue_this.$store.state.userinfo.phone, name], vue_this.$store.state.zc_rpc_url).then(function () {
                    vue_this.pub_evt();
                });
            }
        },
        reset_scale: function (name) {
            var vue_this = this;
            if (this.$store.state.zczh_back_end) {
                this.found_device_to_execute(function (set_info) {
                    return set_info.id
                }, function (id) {
                    vue_this.$axios.post(vue_this.$store.state.zczh_back_end + "/api/reset_scale_sm", {
                        sm_id: id,
                    }, {
                        headers: {
                            "Content-Type": "application/json",
                            "token": vue_this.$store.state.zczh_back_token,
                        }
                    }).then(function () {
                        vue_this.pub_evt();
                    });
                });
            } else {
                vue_this.$call_remote_process("open_api", 'do_device_opt_reset_scale', [vue_this.$store.state.userinfo.phone, name], vue_this.$store.state.zc_rpc_url).then(function () {
                    vue_this.pub_evt();
                });
            }
        },
        trigger_cap: function (name, is_enter, vehicle_number) {
            var vue_this = this;
            if (this.$store.state.zczh_back_end) {
                this.found_device_to_execute(function (set_info) {
                    if (is_enter) {
                        return set_info.plate_cam.front.id;
                    } else {
                        return set_info.plate_cam.back.id;
                    }
                }, function (id) {
                    if (vehicle_number) {
                        vue_this.$axios.post(vue_this.$store.state.zczh_back_end + "/api/device_mock/push_plate", {
                            device_id: id,
                            plate_no: vehicle_number,
                        }, {
                            headers: {
                                "Content-Type": "application/json",
                                "token": vue_this.$store.state.zczh_back_token,
                            }
                        }).then(function () {
                            vue_this.trigger_diag_show = false;
                            vue_this.vehicle_number = "";
                            vue_this.pub_evt();
                        });
                    } else {
                        vue_this.$axios.post(vue_this.$store.state.zczh_back_end + "/api/plate_cam_cap", {
                            device_id: id,
                        }, {
                            headers: {
                                "Content-Type": "application/json",
                                "token": vue_this.$store.state.zczh_back_token,
                            }
                        }).then(function () {
                            vue_this.trigger_diag_show = false;
                            vue_this.vehicle_number = "";
                            vue_this.pub_evt();
                        });
                    }
                });
            } else {
                vue_this.$call_remote_process("open_api", 'do_device_opt_trigger_cap', [vue_this.$store.state.userinfo.phone, name, is_enter, vehicle_number], vue_this.$store.state.zc_rpc_url).then(function () {
                    vue_this.trigger_diag_show = false;
                    vue_this.vehicle_number = "";
                    vue_this.pub_evt();
                });
            }
        },
        take_pic: function (name, is_enter) {
            var vue_this = this;
            var pic_call_back = function (resp) {
                if (resp.indexOf("http") != -1) {
                    ImagePreview([resp]);
                } else {
                    var prifix_url = vue_this.$store.state.zc_rpc_url.substr(0, vue_this.$store.state.zc_rpc_url.indexOf("/zh_rpc/"));
                    if (vue_this.$store.state.zczh_back_end) {
                        prifix_url = vue_this.$store.state.zczh_back_end;
                    }
                    var whole_path = prifix_url + resp;
                    ImagePreview([whole_path]);
                }
                vue_this.pub_evt();
            };

            if (this.$store.state.zczh_back_end) {
                this.found_device_to_execute(function (set_info) {
                    if (is_enter) {
                        return set_info.plate_cam.front.id;
                    } else {
                        return set_info.plate_cam.back.id;
                    }
                }, function (id) {
                    vue_this.$axios.post(vue_this.$store.state.zczh_back_end + "/api/cap_picture", {
                        device_id: id,
                    }, {
                        headers: {
                            "Content-Type": "application/json",
                            "token": vue_this.$store.state.zczh_back_token,
                        }
                    }).then(function (resp) {
                        pic_call_back(resp.data.result);
                    });
                });
            } else {
                vue_this.$call_remote_process("open_api", 'do_device_opt_take_pic', [vue_this.$store.state.userinfo.phone, name, is_enter], vue_this.$store.state.zc_rpc_url).then(function (resp) {
                    pic_call_back(resp);
                });
            }
        },
    },
}
</script>

<style scoped>
.swip-button {
    height: 100%;
}
</style>
