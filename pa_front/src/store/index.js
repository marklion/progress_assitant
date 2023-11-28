import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    userinfo: {
      name:'',
      buyer: true,
      company: '',
      logo: '',
      phone:'',
      user_id:0,
      groupid:0,
    },
    is_login:false,
    enter_company:false,
    verify_count_down:0,
    need_sec_check:false,
    zc_rpc_url:'',
    need_buy_feature:false,
    zczh_back_end:'',
    zczh_back_token:'',
  },
  mutations: {
    set_userinfo(state, userinfo){
      state.userinfo = userinfo;
    },
    set_login(state, _login) {
      state.is_login = _login;
    },
    set_enter(state, _enter) {
      state.enter_company = _enter;
    },
    set_verify_count_down(state, _count_down) {
      state.verify_count_down = _count_down;
    },
    set_sec_check(state, need_sec_check) {
      state.need_sec_check = need_sec_check;
    },
    set_zc_rpc_url(state, zc_rpc_url) {
      state.zc_rpc_url = zc_rpc_url;
    },
    set_need_buy_feature(state, need_buy_feature) {
      state.need_buy_feature = need_buy_feature;
    },
    set_zczh_back_end(state, zczh_back_end) {
      state.zczh_back_end = zczh_back_end;
    },
    set_zczh_back_token(state, zczh_back_token) {
      state.zczh_back_token = zczh_back_token;
    }
  },
  actions: {
  },
  modules: {
  }
})
