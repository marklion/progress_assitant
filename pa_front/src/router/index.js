import Vue from 'vue'
import VueRouter from 'vue-router'

Vue.use(VueRouter)

const routes = [
  {
    path: '/',
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
  {
    path: '/bind_company',
    name: 'BindCompany',
    component: () => import('../views/BindCompany.vue'),
    meta: {    
        private_title: '绑定公司信息',
        has_go_back:true,
    },
  },
  {
    path: '/create_ticket/:app_id',
    name: 'CreateTicket',
    component: () => import('../views/CreateTicket.vue'),
    meta: {    
        private_title: '创建流程',
        has_go_back:true,
    },
  },
  {
    path: '/ticket/:ticket_number',
    name: 'TicketDetail',
    component: () => import('../views/TicketDetail.vue'),
    meta: {    
        private_title: '流程详情',
        has_go_back:true,
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
]

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes
})

export default router
