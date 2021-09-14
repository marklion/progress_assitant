<template>
<div class="history_input_show">
    <van-field readonly clickable :name="search_key" v-model="input_value" :label="search_key_convert(search_key).show_words" placeholder="点击输入或选择内容" @click="showPicker = true" :rules="rules" :formatter="formatter" />
    <van-popup v-model="showPicker" position="bottom" get-container="body">
        <van-field v-model="key_word" :placeholder="'请输入' + search_key_convert(search_key).show_words" />
        <van-picker show-toolbar :columns="item_need_select" @confirm="onConfirm" @cancel="showPicker = false" />
    </van-popup>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Field
} from 'vant';
import {
    Picker
} from 'vant';
import {
    Popup
} from 'vant';

import PinyinMatch from 'pinyin-match';
Vue.use(Popup);
Vue.use(Picker);
Vue.use(Field);
export default {
    name: 'HistoryInput',
    data: function () {
        return {
            items: [],
            showPicker: false,
            key_word: '',
            search_key_convert: function (_value) {
                var ret = {};
                switch (_value) {
                    case 'company_name':
                        ret.show_words = '发货公司';
                        break;
                    case 'destination':
                        ret.show_words = '收货公司';
                        break;
                    case 'main_vichele_number':
                        ret.show_words = '主车车牌';
                        break;
                    case 'behind_vichele_number':
                        ret.show_words = '挂车车牌';
                        break;
                    case 'stuff_name':
                        ret.show_words = '货物名称';
                        break;
                    case 'comment':
                        ret.show_words = '备注';
                        break;
                    case 'transfor_company':
                        ret.show_words = '承运公司';
                        break;
                    case 'driver_id':
                        ret.show_words = '司机身份证';
                        break;
                    case 'driver_name':
                        ret.show_words = '司机姓名';
                        break;
                    case 'driver_phone':
                        ret.show_words = '司机电话';
                        break;
                    default:
                        break;
                }
                ret.search_key = {
                    [_value]: '?'
                };
                return ret;
            },
        };
    },
    props: {
        value: String,
        search_key: String,
        rules: Array,
        formatter: Function,
    },
    model: {
        prop: 'value',
        event: 'change',
    },
    computed: {
        input_value: {
            get() {
                return this.value;
            },
            set(_value) {
                this.$emit('change', _value);
            },
        },
        item_need_select: function () {
            var ret = [];
            var vue_this = this;
            if (vue_this.key_word.length <= 0) {
                ret = vue_this.items;
            } else {
                vue_this.items.forEach(element => {
                    if (PinyinMatch.match(element, vue_this.key_word)) {
                        ret.push(element);
                    }
                });
                ret.push(this.key_word);
            }
            return ret;
        },
    },
    methods: {
        onConfirm: function (_value) {
            this.$emit('change', _value);
            this.showPicker = false;
        },
    },
    beforeMount: function () {
        var vue_this = this;
        vue_this.$call_remote_process_no_toast("vichele_management", 'get_input_history', [vue_this.$cookies.get('silent_id'), vue_this.search_key_convert(vue_this.search_key).search_key]).then(function (resp) {
            vue_this.items = [];
            var not_empty = [];
            resp.forEach(element => {
                if (element) {
                    not_empty.push(element);
                }
            });

            not_empty.forEach((element, index) => {
                vue_this.$set(vue_this.items, index, element);
            });
        });
    },
}
</script>

<style>

</style>
