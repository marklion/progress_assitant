import Vue from 'vue'
import './plugins/axios'
import App from './App.vue'
import router from './router'
import store from './store'
import cookies from 'vue-cookies'
import {
    get_client
} from '@/plugins/rpc_helper.js'

Vue.prototype.$cookies = cookies;
Vue.prototype.$get_client = get_client;
Vue.prototype.$remote_url = 'https://www.d8sis.cn/pa_web';
Vue.config.productionTip = false

new Vue({
  router,
  store,
  render: h => h(App)
}).$mount('#app')
