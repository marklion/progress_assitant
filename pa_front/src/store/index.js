import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    userinfo: {
      name:'',
      role: '',
      company: '',
      logo: '',
      company_logo:''
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
