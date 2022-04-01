<template>
<div>
    <van-form validate-first @submit="onSubmit" @failed="onFailed">
        <van-field v-model="formData.stuff_name" name="stuff_name" label="竞价商品" placeholder="竞价商品名称" readonly :rules="[{ required: true, message: '请填写竞价商品名称' }]" />
        <van-field v-model="formData.total_count" type="number" name="total_count" label="商品总量" placeholder="竞价商品总供给量" :rules="[{ required: true, message: '请填写竞价商品供给总量' }]" />
        <van-divider content-position="left">竞价区间</van-divider>
        <van-field v-model="formData.max_price" type="number" name="max_price" label="最高价" placeholder="设置竞价上限" :rules="[{ required: true, message: '请填写接受的最高报价' }]" />
        <van-field v-model="formData.min_price" type="number" name="min_price" label="最低价" placeholder="设置竞价下限" :rules="[{ required: true, message: '请填写接受的最低报价' }]" />
        <van-field name="bidding_times" label="竞价轮次">
            <template #input>
                <van-radio-group v-model="formData.bidding_times" direction="horizontal">
                    <van-radio name="1" value="1">1轮</van-radio>
                    <van-radio name="2" value="2">2轮</van-radio>
                </van-radio-group>
            </template>
        </van-field>
        <van-field v-model="formData.deposit" type="number" name="deposit" label="保证金" placeholder="请设置参标保证金" :rules="[{ required: true, message: '不设置可填0' }]" />
        <van-field readonly clickable name="end_time" :value="formData.end_time" label="截止时间" placeholder="点击选择截止时间" @click="showPicker = true" />
        <van-popup v-model="showPicker" position="bottom">
            <van-datetime-picker type="datetime" title="选择截止时间" :min-date="new Date()" :filter="timeFilter" @confirm="onConfirmEndTime" @cancel="showPicker = false" />
        </van-popup>
        <van-field type=""
                   :value="formData.customers && formData.customers.length"
                   readonly is-link clickable
                   label="参与客户"
                   name="customers"
                   @click="showCustomerPicker = true"
                   :rules="[{validator: validCustomer, message: '至少要选定2家公司参与竞价'}]"
                   placeholder="请选择可参与竞价的客户"></van-field>
        <van-popup :value="showCustomerPicker" position="bottom" closeable @click-overlay="onCloseCustomerPicker" @click-close-icon="onCloseCustomerPicker" class="customer_picker">
            <div class="button-ctn">
                <van-button type="primary" size="small" @click="onClickCheckAll">全选</van-button>
                <van-button type="info" size="small" @click="onClickToggleAll">反选</van-button>
                <van-search class="customer_name_search" @input="customerFilter" v-model="searchKey" placeholder="关键词或拼音首字母" />
            </div>
            <div class="list-ctn">
                <van-loading v-if="allCustomers.length === 0" size="24px" vertical>加载中...</van-loading>
                <van-checkbox-group v-model="formData.customers" ref="checkboxGroup">
                    <van-cell-group>
                        <van-cell
                            v-for="(item, index) in filteredCustomers"
                            clickable
                            :key="item.name"
                            :title="item.name"
                            @click="toggle(index)"
                        >
                            <template #right-icon>
                                <van-checkbox :name="item.name" ref="checkboxes" />
                            </template>
                        </van-cell>
                    </van-cell-group>
                </van-checkbox-group>
            </div>
        </van-popup>


        <div style="margin: 16px;">
            <van-button round block type="info" native-type="submit">提交</van-button>
        </div>
    </van-form>
</div>
</template>

<script>
import moment from 'moment'
import PinyinMatch from "pinyin-match";
import {Notify} from "vant";
import {getAllContract} from '@/api/company'
import {createBidding} from '@/api/stuff'

export default {
    data() {
        return {
            ssid : '',
            formData: {
                stuff_name: '',
                min_price: undefined,
                max_price: undefined,
                bidding_times: '1',
                customers: [],
                end_time: moment().format('YYYY-MM-DD HH:mm:00'),
                deposit: 0,
                total_count: undefined,
            },
            showPicker: false,
            showCustomerPicker: false,
            allCustomers: [],
            filteredCustomers: [],
            searchKey : ''
        }
    },
    computed:{
        submitForm(){
            return {
                stuff_name : this.formData.stuff_name,
                min_price: +this.formData.min_price,
                max_price: +this.formData.max_price,
                bidding_times: +this.formData.bidding_times,
                customers: this.formData.customers,
                end_time: this.formData.end_time,
                deposit: +this.formData.deposit,
                total_count: +this.formData.total_count
            }
        }
    },
    mounted() {
        this.formData.stuff_name = this.$route.query.name;
        this.ssid = this.$cookies.get('pa_ssid')
        getAllContract(this.ssid, false).then((resp) => {
            this.allCustomers = resp.map((item) => {return {name : item.a_side_company}})
            this.filteredCustomers = this.allCustomers
        })
    },
    methods: {
        async onSubmit() {
            let isDone = await createBidding(this.ssid, this.submitForm)
            if(isDone){
                await this.$router.push({name : 'home'})
            }
        },
        onFailed() {
            Notify({ type: 'danger', message: '请按照页面提示修改表单' });
        },
        onConfirmEndTime(endTime) {
            this.formData.end_time = moment(endTime).format('YYYY-MM-DD HH:mm:00')
            this.showPicker = false
        },
        timeFilter(type, opt) {
            if (type === 'minute') {
                return opt.filter(option =>
                    option % 10 === 0
                )
            }
            return opt
        },
        onCloseCustomerPicker(){
            this.showCustomerPicker = false
        },
        toggle(index) {
            this.$refs.checkboxes[index].toggle();
        },
        onClickCheckAll(){
            this.$refs.checkboxGroup.toggleAll(true);
        },
        onClickToggleAll(){
            this.$refs.checkboxGroup.toggleAll();
        },
        validCustomer(val){
            return val >= 2
        },
        customerFilter(keyword){
            this.filteredCustomers = this.allCustomers.filter(item => {
                return PinyinMatch.match(item.name, keyword) || item.name.match(new RegExp(keyword))
            })

        }
    }
}
</script>

<style scoped>
.customer_picker{
    height: 50%;
}
.button-ctn{
    vertical-align: baseline;
    line-height: 52px;
    height: 52px;
}
.customer_name_search{
    display: inline-block;
}
.list-ctn{
    height: 100%;
    overflow: scroll;
    padding-bottom: 20px;
}
.customer_picker button{
    vertical-align: super;
    margin: 10px 0 10px 10px;
}
</style>
