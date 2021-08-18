import Vue from 'vue'
import VueRouter from 'vue-router'

Vue.use(VueRouter)

const routes = [
  {
    path: '/home',
    name: 'Home',
    component: () => import('../views/Home.vue'),
    meta: {    
        private_title: '今日报价',
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
    path: '/plan_update/:plan_id',
    name: 'PlanUpdate',
    component: () => import('../views/PlanUpdate.vue'),
    meta: {    
        private_title: '计划更新',
        has_go_back:true,
    },
  },
  {
    path: '/company_order',
    name: 'CompanyOrder',
    component: () => import('../views/CompanyOrder.vue'),
    meta: {    
        private_title: '订单',
        has_go_back:false,
        keepAlive: true,
        scrollTop: 0,
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
    path: '/admin',
    name: 'Admin',
    component: () => import('../views/Admin.vue'),
    meta: {    
        private_title: '管理员菜单',
        has_go_back:true,
    },
  },
  {
    path: '/deliver_plan/:plan_id',
    name: 'DeliverPlan',
    component: () => import('../views/DeliverPlan.vue'),
    meta: {    
        private_title: '出货',
        has_go_back:true,
    },
  },
  {
    path: '/statistics',
    name: 'Statistics',
    component: () => import('../views/Statistics.vue'),
    meta: {    
        private_title: '交易统计',
        has_go_back:true,
    },
  },
  {
    path: '/bound_info',
    name: 'BoundInfo',
    component: () => import('../views/BoundInfo.vue'),
    meta: {    
        private_title: '公司数据',
        has_go_back:true,
    },
  },
  {
    path: '/plan_copy/:plan_id',
    name: 'PlanCopy',
    component: () => import('../views/PlanCopy.vue'),
    meta: {
      private_title: '复制计划',
      has_go_back: true,
    },
  },
  {
    path: '/plan_import/:type_id',
    name: 'PlanImport',
    component: () => import('../views/PlanImport.vue'),
    meta: {
      private_title: '导入计划',
      has_go_back: true,
    },
  }, {
    path: '/contract',
    name: 'Contract',
    component: () => import('../views/Contract.vue'),
    meta: {
      private_title: '合同管理',
      has_go_back: true,
    },
  },{
    path: '/extra_vichele',
    name: 'ExtraVichele',
    component: () => import('../views/ExtraVichele.vue'),
    meta: {
      private_title: '货车进厂',
      has_go_back: false,
      extra_page:true,
    },
  },{
    path: '/company_extra_vichele',
    name: 'CompanyExtraVichele',
    component: () => import('../views/CompanyExtraVichele.vue'),
    meta: {
      private_title: '采购进厂',
      has_go_back: true,
    },
  },{
    path: '/driver_register',
    name: 'DriverRegister',
    component: () => import('../views/DriverRegister.vue'),
    meta: {
      private_title: '进厂排号',
      has_go_back: false,
      extra_page:true,
    },
  },{
    path: '/third_dev',
    name: 'ThirdDev',
    component: () => import('../views/ThirdDev.vue'),
    meta: {
      private_title: '开发选项',
      has_go_back: true,
      extra_page:false,
    },
  }
]

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes
})

export default router
