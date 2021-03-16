import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    userinfo: {
      is_login:false,
      name:'',
      buyer: true,
      company: '',
      logo: '',
      phone:''
    },
  },
  mutations: {
    set_userinfo(state, userinfo){
      state.userinfo = userinfo;
    },
  },
  actions: {
  },
  modules: {
  }
})
