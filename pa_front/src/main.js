import Vue from 'vue'
import './plugins/axios'
import App from './App.vue'
import router from './router'
import store from './store'
import cookies from 'vue-cookies'

Vue.prototype.$cookies = cookies;
Vue.prototype.$remote_url = 'https://www.d8sis.cn/pa_web';
Vue.config.productionTip = false

new Vue({
  router,
  store,
  render: h => h(App)
}).$mount('#app')
