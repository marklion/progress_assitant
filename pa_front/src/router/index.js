import Vue from 'vue'
import VueRouter from 'vue-router'

Vue.use(VueRouter)

const routes = [
  {
    path: '/',
    name: 'Home',
    component: () => import('../views/Home.vue'),
    meta: {    
        private_title: '流程助手',
        has_go_back:false,
    },
  },
  {
    path: '/login',
    name: 'Login',
    component: () => import('../views/Login.vue'),
    meta: {    
        private_title: '登陆',
        has_go_back:true,
    },
  },
  {
    path: '/application',
    name: 'Application',
    component: () => import('../views/Application.vue'),
    meta: {    
        private_title: '应用',
        has_go_back:false,
    },
  },
  {
    path: '/myself',
    name: 'Myself',
    component: () => import('../views/Myself.vue'),
    meta: {    
        private_title: '我的',
        has_go_back:false,
    },
  },
]

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes
})

export default router
