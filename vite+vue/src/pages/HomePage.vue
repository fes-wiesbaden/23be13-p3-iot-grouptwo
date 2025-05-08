<template>
  <div id="home">
    <header class="app-header">
      <h1 class="text-white">Smart Home Dashboard</h1>
      <div class="bg-red-500 text-white p-4">Tailwind is working!</div>
      <ThemeToggle />
    </header>

    <NotificationBar :message="notification" type="info" @close="clearNotification" />

    <main class="app-main">
      <Dashboard />
    </main>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue';
import ThemeToggle from '@/components/ThemeToggle.vue';
import NotificationBar from '@/components/NotificationBar.vue';
import Dashboard from '@/components/Dashboard.vue';

const notification = ref('Living Room AC turned on automatically');

const clearNotification = () => {
  notification.value = '';
};

onMounted(() => {
  const savedTheme = localStorage.getItem('theme');
  if (savedTheme === 'dark') {
    document.body.classList.add('dark-mode');
  }
});
</script>

<style scoped>
#home {
  font-family: 'Segoe UI', sans-serif;
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  background-color: #f7f9fc;
}

.dark-mode #home {
  background-color: #1f2937;
}

.app-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  background-color: #1e293b;
  color: white;
  padding: 1rem 2rem;
}

.app-main {
  flex: 1;
  padding: 2rem;
}

/* Other styles copied from App.vue */

body.dark-mode {
  background-color: #121212;
  color: white;
}

body {
  --card-bg-light: #ffffff;
  --card-bg-dark: #2e3b47;
  --card-text-light: #333333;
  --card-text-dark: #f1f1f1;
}

body.light-mode .device-card {
  background-color: var(--card-bg-light);
  color: var(--card-text-light);
}

body.dark-mode .device-card {
  background-color: var(--card-bg-dark);
  color: var(--card-text-dark);
}

.device-card {
  border-radius: 8px;
  width: 200px;
  margin: 10px;
  padding: 10px;
  text-align: center;
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
  cursor: pointer;
  transition: background-color 0.3s, color 0.3s;
}

.device-card.on {
  background-color: #d1fae5;
}

.device-card.off {
  background-color: #fee2e2;
}

.device-card:hover {
  opacity: 0.8;
}

body.dark-mode .device-card:hover {
  background-color: #2a9d8f;
}

body.light-mode .device-card:hover {
  background-color: #c2e8c2;
}
</style>
