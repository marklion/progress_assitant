<template>
<div class="vichele_in_plan_show">
    <van-field readonly clickable :value="my_vichele_info.main_vichele" label="主车" placeholder="点击选择车牌" @click="show_main_vichele_picker = true" :rules="[{ required: true, message: '请指定主车' }]" />
    <van-popup v-model="show_main_vichele_picker" position="bottom" @click="main_vichele_search_input = ''">
        <van-field v-model="main_vichele_search_input" clearable placeholder="输入车牌号过滤或新增(支持模糊查找)">
            <template #button>
                <van-button size="small" type="primary" native-type="button" @click="submit_new_vichele(main_vichele_search_input, true)">新增</van-button>
            </template>
        </van-field>
        <van-picker show-toolbar :columns="main_vichele_pool_filtered" @confirm="confirm_main_vichele" @cancel="show_main_vichele_picker = false">
        </van-picker>
    </van-popup>

    <van-field readonly clickable :value="my_vichele_info.behind_vichele" label="挂车" placeholder="点击选择车牌" @click="show_behind_vichele_picker = true" :rules="[{ required: true, message: '请指定挂车' }]" />
    <van-popup v-model="show_behind_vichele_picker" position="bottom" @click="behind_vichele_search_input = ''">
        <van-field v-model="behind_vichele_search_input" clearable placeholder="输入车牌号过滤或新增(支持模糊查找)">
            <template #button>
                <van-button size="small" type="primary" native-type="button" @click="submit_new_vichele(behind_vichele_search_input, false)">新增</van-button>
            </template>
        </van-field>
        <van-picker show-toolbar :columns="behind_vichele_pool_filtered" @confirm="confirm_behind_vichele" @cancel="show_behind_vichele_picker = false">
        </van-picker>
    </van-popup>

    <van-field readonly clickable :value="my_vichele_info.driver_name" label="司机姓名" placeholder="点击选择司机" @click="show_driver_picker = true" :rules="[{ required: true, message: '请指定司机' }]" />
    <van-field readonly :value="my_vichele_info.driver_phone" label="司机电话" :rules="[{ required: true, message: '请指定司机'}]" />
    <van-popup v-model="show_driver_picker" position="bottom" @click="driver_search_input = ''">
        <van-field v-model="driver_search_input" clearable placeholder="过滤电话/司机姓名/拼音首字母">
        </van-field>
        <van-picker show-toolbar :columns="driver_pool_filtered" @confirm="confirm_driver" @cancel="show_driver_picker = false">
            <template #title>
                <van-button type="info" native-type="button" @click="show_add_driver_diag= true">新增司机</van-button>
            </template>
        </van-picker>
    </van-popup>
    <van-dialog v-model="show_add_driver_diag" close-on-click-overlay title="新增司机" :show-confirm-button="false">
        <van-form @submit="submit_new_driver">
            <van-field v-model="new_driver_name" label="姓名" placeholder="请输入司机姓名" :rules="[{ required: true, message: '请输入正确司机姓名' }]" />
            <van-field v-model="new_driver_phone" type="tel" label="电话" placeholder="请输入司机电话" :rules="[{ required: true, message: '请输入正确手机号', pattern:phone_pattern }]" />
            <van-button round block type="primary">确认添加</van-button>
        </van-form>
    </van-dialog>

    <van-field name="use_for" label="用途" :rules="[{ required: true, message: '请指定用途'}]">
        <template #input>
            <van-radio-group v-model="my_vichele_info.use_for" direction="horizontal" @change="use_for_change">
                <van-radio name="气化">气化</van-radio>
                <van-radio name="气站">气站</van-radio>
            </van-radio-group>
        </template>
    </van-field>

    <van-field readonly clickable name="drop_address" :value="my_vichele_info.drop_address" label="卸车地点" placeholder="点击选择省市区" @click="showArea = true" :rules="[{ required: true, message: '请指定卸车地点'}]" />
    <van-popup v-model="showArea" position="bottom">
        <van-area :area-list="all_area" columns-num="2" @confirm="confirm_area" @cancel="showArea = false" />
    </van-popup>
    <van-field name="comment" v-model="my_vichele_info.comment" label="备注" @input="change_to_parent" />
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
import {
    Button
} from 'vant';
import {
    Stepper
} from 'vant';
import {
    Dialog
} from 'vant';
import {
    RadioGroup,
    Radio
} from 'vant';
import {
    Area
} from 'vant';
import {
    Form
} from 'vant';
import PinyinMatch from 'pinyin-match';
Vue.use(Form);
Vue.use(Area);
Vue.use(Radio);
Vue.use(RadioGroup);
Vue.use(Dialog);
Vue.use(Stepper);
Vue.use(Button);
Vue.use(Popup);
Vue.use(Picker);
Vue.use(Field);
export default {
    name: 'VicheleInPlan',
    props: {
        vichele_info: Object,
    },
    model: {
        prop: 'vichele_info',
        event: 'change',
    },
    watch: {
        vichele_info(_val) {
            this.my_vichele_info = _val;
        },
    },
    computed: {
        main_vichele_pool_filtered: function () {
            var ret = [];
            var vue_this = this;
            this.main_vichele_pool.forEach(element => {
                var regx = new RegExp(vue_this.main_vichele_search_input, 'gi');
                if (element.match(regx)) {
                    ret.push(element);
                }
            });

            return ret;
        },
        behind_vichele_pool_filtered: function () {
            var ret = [];
            var vue_this = this;
            this.behind_vichele_pool.forEach(element => {
                var regx = new RegExp(vue_this.behind_vichele_search_input, 'gi');
                if (element.match(regx)) {
                    ret.push(element);
                }
            });

            return ret;
        },
        driver_pool_filtered: function () {
            var ret = [];
            var vue_this = this;
            if (vue_this.driver_search_input.length <= 0) {
                ret = vue_this.driver_name_pool;
            } else {
                vue_this.driver_name_pool.forEach(element => {
                    if (PinyinMatch.match(element, vue_this.driver_search_input)) {
                        ret.push(element);
                    }
                });
            }
            return ret;
        },
    },
    data: function () {
        return {
            driver_search_input: '',
            main_vichele_search_input: '',
            behind_vichele_search_input: '',
            vichele_number_patten: /^(京[A-HJ-NPQY]|沪[A-HJ-N]|津[A-HJ-NPQR]|渝[A-DFGHN]|冀[A-HJRST]|晋[A-FHJ-M]|蒙[A-HJKLM]|辽[A-HJ-NP]|吉[A-HJK]|黑[A-HJ-NPR]|苏[A-HJ-N]|浙[A-HJKL]|皖[A-HJ-NP-S]|闽[A-HJK]|赣[A-HJKLMS]|鲁[A-HJ-NP-SUVWY]|豫[A-HJ-NP-SU]|鄂[A-HJ-NP-S]|湘[A-HJ-NSU]|粤[A-HJ-NP-Y]|桂[A-HJ-NPR]|琼[A-F]|川[A-HJ-MQ-Z]|贵[A-HJ]|云[AC-HJ-NP-SV]|藏[A-HJ]|陕[A-HJKVU]|甘[A-HJ-NP]|青[A-H]|宁[A-E]|新[A-HJ-NP-S])([0-9A-HJ-NP-Z]{4}[0-9A-HJ-NP-Z挂试]|[0-9]{4}学|[A-D0-9][0-9]{3}警|[DF][0-9A-HJ-NP-Z][0-9]{4}|[0-9]{5}[DF])$|^WJ[京沪津渝冀晋蒙辽吉黑苏浙皖闽赣鲁豫鄂湘粤桂琼川贵云藏陕甘青宁新]?[0-9]{4}[0-9JBXTHSD]$|^(V[A-GKMORTV]|K[A-HJ-NORUZ]|H[A-GLOR]|[BCGJLNS][A-DKMNORVY]|G[JS])[0-9]{5}$|^[0-9]{6}使$|^([沪粤川渝辽云桂鄂湘陕藏黑]A|闽D|鲁B|蒙[AEH])[0-9]{4}领$|^粤Z[0-9A-HJ-NP-Z][0-9]{3}[港澳]$/i,
            phone_pattern: /^(13[0-9]|14[01456879]|15[0-35-9]|16[2567]|17[0-8]|18[0-9]|19[0-35-9])\d{8}$/,
            showArea: false,
            my_vichele_info: this.vichele_info,
            main_vichele_pool: [],
            show_main_vichele_picker: false,
            behind_vichele_pool: [],
            show_behind_vichele_picker: false,
            show_driver_picker: false,
            driver_name_pool: [],
            show_add_driver_diag: false,
            new_driver_name: '',
            new_driver_phone: '',
            all_area: {
                province_list: {
                    110000: '北京市',
                    120000: '天津市',
                    130000: '河北省',
                    140000: '山西省',
                    150000: '内蒙古自治区',
                    210000: '辽宁省',
                    220000: '吉林省',
                    230000: '黑龙江省',
                    310000: '上海市',
                    320000: '江苏省',
                    330000: '浙江省',
                    340000: '安徽省',
                    350000: '福建省',
                    360000: '江西省',
                    370000: '山东省',
                    410000: '河南省',
                    420000: '湖北省',
                    430000: '湖南省',
                    440000: '广东省',
                    450000: '广西壮族自治区',
                    460000: '海南省',
                    500000: '重庆市',
                    510000: '四川省',
                    520000: '贵州省',
                    530000: '云南省',
                    540000: '西藏自治区',
                    610000: '陕西省',
                    620000: '甘肃省',
                    630000: '青海省',
                    640000: '宁夏回族自治区',
                    650000: '新疆维吾尔自治区',
                    710000: '台湾省',
                    810000: '香港特别行政区',
                    820000: '澳门特别行政区',
                },
                city_list: {
                    110100: '北京市',
                    120100: '天津市',
                    130100: '石家庄市',
                    130200: '唐山市',
                    130300: '秦皇岛市',
                    130400: '邯郸市',
                    130500: '邢台市',
                    130600: '保定市',
                    130700: '张家口市',
                    130800: '承德市',
                    130900: '沧州市',
                    131000: '廊坊市',
                    131100: '衡水市',
                    140100: '太原市',
                    140200: '大同市',
                    140300: '阳泉市',
                    140400: '长治市',
                    140500: '晋城市',
                    140600: '朔州市',
                    140700: '晋中市',
                    140800: '运城市',
                    140900: '忻州市',
                    141000: '临汾市',
                    141100: '吕梁市',
                    150100: '呼和浩特市',
                    150200: '包头市',
                    150300: '乌海市',
                    150400: '赤峰市',
                    150500: '通辽市',
                    150600: '鄂尔多斯市',
                    150700: '呼伦贝尔市',
                    150800: '巴彦淖尔市',
                    150900: '乌兰察布市',
                    152200: '兴安盟',
                    152500: '锡林郭勒盟',
                    152900: '阿拉善盟',
                    210100: '沈阳市',
                    210200: '大连市',
                    210300: '鞍山市',
                    210400: '抚顺市',
                    210500: '本溪市',
                    210600: '丹东市',
                    210700: '锦州市',
                    210800: '营口市',
                    210900: '阜新市',
                    211000: '辽阳市',
                    211100: '盘锦市',
                    211200: '铁岭市',
                    211300: '朝阳市',
                    211400: '葫芦岛市',
                    220100: '长春市',
                    220200: '吉林市',
                    220300: '四平市',
                    220400: '辽源市',
                    220500: '通化市',
                    220600: '白山市',
                    220700: '松原市',
                    220800: '白城市',
                    222400: '延边朝鲜族自治州',
                    230100: '哈尔滨市',
                    230200: '齐齐哈尔市',
                    230300: '鸡西市',
                    230400: '鹤岗市',
                    230500: '双鸭山市',
                    230600: '大庆市',
                    230700: '伊春市',
                    230800: '佳木斯市',
                    230900: '七台河市',
                    231000: '牡丹江市',
                    231100: '黑河市',
                    231200: '绥化市',
                    232700: '大兴安岭地区',
                    310100: '上海市',
                    320100: '南京市',
                    320200: '无锡市',
                    320300: '徐州市',
                    320400: '常州市',
                    320500: '苏州市',
                    320600: '南通市',
                    320700: '连云港市',
                    320800: '淮安市',
                    320900: '盐城市',
                    321000: '扬州市',
                    321100: '镇江市',
                    321200: '泰州市',
                    321300: '宿迁市',
                    330100: '杭州市',
                    330200: '宁波市',
                    330300: '温州市',
                    330400: '嘉兴市',
                    330500: '湖州市',
                    330600: '绍兴市',
                    330700: '金华市',
                    330800: '衢州市',
                    330900: '舟山市',
                    331000: '台州市',
                    331100: '丽水市',
                    340100: '合肥市',
                    340200: '芜湖市',
                    340300: '蚌埠市',
                    340400: '淮南市',
                    340500: '马鞍山市',
                    340600: '淮北市',
                    340700: '铜陵市',
                    340800: '安庆市',
                    341000: '黄山市',
                    341100: '滁州市',
                    341200: '阜阳市',
                    341300: '宿州市',
                    341500: '六安市',
                    341600: '亳州市',
                    341700: '池州市',
                    341800: '宣城市',
                    350100: '福州市',
                    350200: '厦门市',
                    350300: '莆田市',
                    350400: '三明市',
                    350500: '泉州市',
                    350600: '漳州市',
                    350700: '南平市',
                    350800: '龙岩市',
                    350900: '宁德市',
                    360100: '南昌市',
                    360200: '景德镇市',
                    360300: '萍乡市',
                    360400: '九江市',
                    360500: '新余市',
                    360600: '鹰潭市',
                    360700: '赣州市',
                    360800: '吉安市',
                    360900: '宜春市',
                    361000: '抚州市',
                    361100: '上饶市',
                    370100: '济南市',
                    370200: '青岛市',
                    370300: '淄博市',
                    370400: '枣庄市',
                    370500: '东营市',
                    370600: '烟台市',
                    370700: '潍坊市',
                    370800: '济宁市',
                    370900: '泰安市',
                    371000: '威海市',
                    371100: '日照市',
                    371300: '临沂市',
                    371400: '德州市',
                    371500: '聊城市',
                    371600: '滨州市',
                    371700: '菏泽市',
                    410100: '郑州市',
                    410200: '开封市',
                    410300: '洛阳市',
                    410400: '平顶山市',
                    410500: '安阳市',
                    410600: '鹤壁市',
                    410700: '新乡市',
                    410800: '焦作市',
                    410900: '濮阳市',
                    411000: '许昌市',
                    411100: '漯河市',
                    411200: '三门峡市',
                    411300: '南阳市',
                    411400: '商丘市',
                    411500: '信阳市',
                    411600: '周口市',
                    411700: '驻马店市',
                    419000: '省直辖县',
                    420100: '武汉市',
                    420200: '黄石市',
                    420300: '十堰市',
                    420500: '宜昌市',
                    420600: '襄阳市',
                    420700: '鄂州市',
                    420800: '荆门市',
                    420900: '孝感市',
                    421000: '荆州市',
                    421100: '黄冈市',
                    421200: '咸宁市',
                    421300: '随州市',
                    422800: '恩施土家族苗族自治州',
                    429000: '省直辖县',
                    430100: '长沙市',
                    430200: '株洲市',
                    430300: '湘潭市',
                    430400: '衡阳市',
                    430500: '邵阳市',
                    430600: '岳阳市',
                    430700: '常德市',
                    430800: '张家界市',
                    430900: '益阳市',
                    431000: '郴州市',
                    431100: '永州市',
                    431200: '怀化市',
                    431300: '娄底市',
                    433100: '湘西土家族苗族自治州',
                    440100: '广州市',
                    440200: '韶关市',
                    440300: '深圳市',
                    440400: '珠海市',
                    440500: '汕头市',
                    440600: '佛山市',
                    440700: '江门市',
                    440800: '湛江市',
                    440900: '茂名市',
                    441200: '肇庆市',
                    441300: '惠州市',
                    441400: '梅州市',
                    441500: '汕尾市',
                    441600: '河源市',
                    441700: '阳江市',
                    441800: '清远市',
                    441900: '东莞市',
                    442000: '中山市',
                    445100: '潮州市',
                    445200: '揭阳市',
                    445300: '云浮市',
                    450100: '南宁市',
                    450200: '柳州市',
                    450300: '桂林市',
                    450400: '梧州市',
                    450500: '北海市',
                    450600: '防城港市',
                    450700: '钦州市',
                    450800: '贵港市',
                    450900: '玉林市',
                    451000: '百色市',
                    451100: '贺州市',
                    451200: '河池市',
                    451300: '来宾市',
                    451400: '崇左市',
                    460100: '海口市',
                    460200: '三亚市',
                    460300: '三沙市',
                    460400: '儋州市',
                    469000: '省直辖县',
                    500100: '重庆市',
                    500200: '县',
                    510100: '成都市',
                    510300: '自贡市',
                    510400: '攀枝花市',
                    510500: '泸州市',
                    510600: '德阳市',
                    510700: '绵阳市',
                    510800: '广元市',
                    510900: '遂宁市',
                    511000: '内江市',
                    511100: '乐山市',
                    511300: '南充市',
                    511400: '眉山市',
                    511500: '宜宾市',
                    511600: '广安市',
                    511700: '达州市',
                    511800: '雅安市',
                    511900: '巴中市',
                    512000: '资阳市',
                    513200: '阿坝藏族羌族自治州',
                    513300: '甘孜藏族自治州',
                    513400: '凉山彝族自治州',
                    520100: '贵阳市',
                    520200: '六盘水市',
                    520300: '遵义市',
                    520400: '安顺市',
                    520500: '毕节市',
                    520600: '铜仁市',
                    522300: '黔西南布依族苗族自治州',
                    522600: '黔东南苗族侗族自治州',
                    522700: '黔南布依族苗族自治州',
                    530100: '昆明市',
                    530300: '曲靖市',
                    530400: '玉溪市',
                    530500: '保山市',
                    530600: '昭通市',
                    530700: '丽江市',
                    530800: '普洱市',
                    530900: '临沧市',
                    532300: '楚雄彝族自治州',
                    532500: '红河哈尼族彝族自治州',
                    532600: '文山壮族苗族自治州',
                    532800: '西双版纳傣族自治州',
                    532900: '大理白族自治州',
                    533100: '德宏傣族景颇族自治州',
                    533300: '怒江傈僳族自治州',
                    533400: '迪庆藏族自治州',
                    540100: '拉萨市',
                    540200: '日喀则市',
                    540300: '昌都市',
                    540400: '林芝市',
                    540500: '山南市',
                    540600: '那曲市',
                    542500: '阿里地区',
                    610100: '西安市',
                    610200: '铜川市',
                    610300: '宝鸡市',
                    610400: '咸阳市',
                    610500: '渭南市',
                    610600: '延安市',
                    610700: '汉中市',
                    610800: '榆林市',
                    610900: '安康市',
                    611000: '商洛市',
                    620100: '兰州市',
                    620200: '嘉峪关市',
                    620300: '金昌市',
                    620400: '白银市',
                    620500: '天水市',
                    620600: '武威市',
                    620700: '张掖市',
                    620800: '平凉市',
                    620900: '酒泉市',
                    621000: '庆阳市',
                    621100: '定西市',
                    621200: '陇南市',
                    622900: '临夏回族自治州',
                    623000: '甘南藏族自治州',
                    630100: '西宁市',
                    630200: '海东市',
                    632200: '海北藏族自治州',
                    632300: '黄南藏族自治州',
                    632500: '海南藏族自治州',
                    632600: '果洛藏族自治州',
                    632700: '玉树藏族自治州',
                    632800: '海西蒙古族藏族自治州',
                    640100: '银川市',
                    640200: '石嘴山市',
                    640300: '吴忠市',
                    640400: '固原市',
                    640500: '中卫市',
                    650100: '乌鲁木齐市',
                    650200: '克拉玛依市',
                    650400: '吐鲁番市',
                    650500: '哈密市',
                    652300: '昌吉回族自治州',
                    652700: '博尔塔拉蒙古自治州',
                    652800: '巴音郭楞蒙古自治州',
                    652900: '阿克苏地区',
                    653000: '克孜勒苏柯尔克孜自治州',
                    653100: '喀什地区',
                    653200: '和田地区',
                    654000: '伊犁哈萨克自治州',
                    654200: '塔城地区',
                    654300: '阿勒泰地区',
                    659000: '自治区直辖县级行政区划',
                    710100: '台北市',
                    710200: '高雄市',
                    710300: '台南市',
                    710400: '台中市',
                    710500: '金门县',
                    710600: '南投县',
                    710700: '基隆市',
                    710800: '新竹市',
                    710900: '嘉义市',
                    711100: '新北市',
                    711200: '宜兰县',
                    711300: '新竹县',
                    711400: '桃园县',
                    711500: '苗栗县',
                    711700: '彰化县',
                    711900: '嘉义县',
                    712100: '云林县',
                    712400: '屏东县',
                    712500: '台东县',
                    712600: '花莲县',
                    712700: '澎湖县',
                    712800: '连江县',
                    810100: '香港岛',
                    810200: '九龙',
                    810300: '新界',
                    820100: '澳门半岛',
                    820200: '离岛',
                },
            },
        };
    },
    methods: {
        convert_bigger: function (_value) {
            return _value.toLocaleUpperCase();
        },
        have_to_have_gua: function (value) {
            var ret = false;
            if (value.split('')[value.length - 1] == "挂") {
                ret = true;
            }
            return ret;
        },
        use_for_change: function () {
            this.change_to_parent();
        },
        count_change: function () {
            this.change_to_parent();
        },
        fetch_current_vichele: function () {
            var vue_this = this;
            vue_this.$call_remote_process("user_management", 'get_bound_vichele', [vue_this.$cookies.get('pa_ssid'), true]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.main_vichele_pool, index, element.number);
                });
            });
            vue_this.$call_remote_process("user_management", 'get_bound_vichele', [vue_this.$cookies.get('pa_ssid'), false]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.behind_vichele_pool, index, element.number);
                });
            });
        },
        fetch_current_driver: function () {
            var vue_this = this;
            vue_this.$call_remote_process("user_management", 'get_bound_driver_info', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.driver_name_pool, index, element.name + '-' + element.phone);
                });
            });
        },
        submit_new_vichele: function (_vichele_number, _main_vichele) {
            var vue_this = this;
            if (!vue_this.vichele_number_patten.test(_vichele_number)) {
                vue_this.$toast('车牌号格式错误');
                return;
            }
            if (!_main_vichele && !vue_this.have_to_have_gua(_vichele_number)) {
                vue_this.$toast('挂车需要以挂结尾');
                return;
            }
            _vichele_number = _vichele_number.toLocaleUpperCase();
            vue_this.$call_remote_process("user_management", 'bind_new_vichele', [vue_this.$cookies.get('pa_ssid'), _vichele_number, _main_vichele]).then(function (resp) {
                if (resp) {
                    vue_this.fetch_current_vichele();
                }
            });
        },
        submit_new_driver: function () {
            var vue_this = this;
            vue_this.$call_remote_process("user_management", 'bind_new_driver', [vue_this.$cookies.get('pa_ssid'), {
                name: vue_this.new_driver_name,
                phone: vue_this.new_driver_phone
            }]).then(function (resp) {
                if (resp) {
                    vue_this.fetch_current_driver();
                    vue_this.driver_search_input = vue_this.new_driver_name;
                }
            }).finally(function () {
                vue_this.show_add_driver_diag = false;
            });
        },
        confirm_area: function (_value) {
            this.my_vichele_info.drop_address = _value.filter((item) => !!item)
                .map((item) => item.name)
                .join('/');
            this.showArea = false;
            this.change_to_parent();
        },
        confirm_driver: function (_val) {
            this.show_driver_picker = false;
            this.my_vichele_info.driver_name = _val.split('-')[0];
            this.my_vichele_info.driver_phone = _val.split('-')[1];
            this.change_to_parent();
        },
        confirm_main_vichele: function (_val) {
            this.show_main_vichele_picker = false;
            this.my_vichele_info.main_vichele = _val;
            this.change_to_parent();
        },
        confirm_behind_vichele: function (_val) {
            this.show_behind_vichele_picker = false;
            this.my_vichele_info.behind_vichele = _val;
            this.change_to_parent();
        },
        change_to_parent: function () {
            this.$emit('change', this.my_vichele_info);
        },
    },
    beforeMount: function () {
        this.fetch_current_vichele();
        this.fetch_current_driver();
    },
}
</script>

<style>

</style>
