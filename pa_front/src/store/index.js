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
      user_id:0
    },
    is_login:false,
    enter_company:false,
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
  },
  actions: {
  },
  modules: {
  }
})
