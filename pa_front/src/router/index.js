import Vue from 'vue'
import VueRouter from 'vue-router'

Vue.use(VueRouter)

const routes = [
  {
    path: '/home',
    name: 'Home',
    component: () => import('../views/Home.vue'),
    meta: {    
        private_title: '掌易',
        has_go_back:false,
    },
  },
  {
    path: '/login_mp',
    name: 'Login',
    component: () => import('../views/Login.vue'),
    meta: {    
        private_title: '登陆中',
        has_go_back:true,
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
  {
    path: '/stuff_plan/:type_id',
    name: 'StuffPlan',
    component: () => import('../views/StuffPlan.vue'),
    meta: {    
        private_title: '计划上报',
        has_go_back:true,
    },
  },
  {
    path: '/plan_detail/:plan_id',
    name: 'PlanDetail',
    component: () => import('../views/PlanDetail.vue'),
    meta: {    
        private_title: '计划详情',
        has_go_back:true,
    },
  },
  {
    path: '/order',
    name: 'Order',
    component: () => import('../views/Order.vue'),
    meta: {    
        private_title: '订单',
        has_go_back:false,
    },
  },
  {
    path: '/company_home',
    name: 'CompanyHome',
    component: () => import('../views/CompanyHome.vue'),
    meta: {    
        private_title: '公司首页',
        has_go_back:false,
    },
  },
  {
    path: '/self_info',
    name: 'SelfInfo',
    component: () => import('../views/SelfInfo.vue'),
    meta: {    
        private_title: '个人信息',
        has_go_back:true,
    },
  },
]

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes
})

export default router
