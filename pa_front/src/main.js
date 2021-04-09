import Vue from 'vue'
import './plugins/axios'
import App from './App.vue'
import router from './router'
import store from './store'
import cookies from 'vue-cookies'
import {
    get_client, call_remote_process
} from '@/plugins/rpc_helper.js'

Vue.prototype.$cookies = cookies;
Vue.prototype.$get_client= get_client;
Vue.prototype.$call_remote_process = call_remote_process;
Vue.prototype.$remote_url = process.env.VUE_APP_BACK_END_URL;
Vue.config.productionTip = false

new Vue({
  router,
  store,
  render: h => h(App)
}).$mount('#app')
