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
            <van-datetime-picker type="datetime" title="选择截止时间" :min-date="new Date()" :filter="timefilter" @confirm="onConfirmEndtime" @cancel="showPicker = false" />
        </van-popup>


        <div style="margin: 16px;">
            <van-button round block type="info" native-type="submit">提交</van-button>
        </div>
    </van-form>
</div>
</template>

<script>
import moment from 'moment'

export default {
    data() {
        return {
            formData: {
                stuff_name: '',
                min_price: undefined,
                max_price: undefined,
                bidding_times: '1',
                customers: [],
                end_time: moment().format('YYYY-MM-DD HH:mm:00'),
                deposit: 0,
                total_count: undefined
            },
            showPicker: false,

        }
    },
    mounted() {
        this.formData.stuff_name = this.$route.query.name;
    },
    methods: {
        onSubmit(arg) {
            console.log(arg)
        },
        onFailed() {

        },
        onConfirmEndtime(endtime) {
            this.formData.end_time = moment(endtime).format('YYYY-MM-DD HH:mm:00')
            this.showPicker = false
        },
        timefilter(type, opt) {
            if (type === 'minute') {
                return opt.filter(option =>

                    option % 10 === 0
                )
            }
            return opt
        }
    }
}
</script>

<style scoped>

</style>
