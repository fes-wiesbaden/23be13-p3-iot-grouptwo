import Vue from 'vue';
import VueRouter from 'vue-router';
import HomeDashboard from '../views/HomeDashboard.vue';

Vue.use(VueRouter);

const routes = [
  {
    path: '/',
    name: 'Home',
    component: HomeDashboard, // This is your dashboard view
  },
];

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes,
});

export default router;