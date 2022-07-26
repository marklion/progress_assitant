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
  },
  actions: {
  },
  modules: {
  }
})
