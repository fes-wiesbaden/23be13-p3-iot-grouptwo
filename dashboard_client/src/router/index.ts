import { createRouter, createWebHistory } from 'vue-router';
import type { RouteRecordRaw } from 'vue-router'; // ✅ Type-only import

import HomePage from '@/pages/HomePage.vue';
import Dashboardv2 from '@/pages/Dashboardv2.vue';

const routes: RouteRecordRaw[] = [
  {
    path: '/',
    name: 'Home',
    component: Dashboardv2,
  },
  {
    path: "/v2",
    name: "DashboardV2",
    component: HomePage,
  },
];

const router = createRouter({
  history: createWebHistory(),
  routes,
});

export default router;
