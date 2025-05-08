<template>
    <div class="dashboard-layout">
      <nav>
        <ul>
          <li><router-link to="/">Home</router-link></li>
          <li><router-link to="/devices">Devices</router-link></li>
          <li><router-link to="/settings">Settings</router-link></li>
        </ul>
      </nav>
      <main>
        <router-view />
      </main>
    </div>
  </template>
  
  <script lang="ts">
  export default {
    name: 'DashboardLayout',
  };
  </script>
  
  <style scoped>
  .dashboard-layout {
    display: flex;
  }
  
  nav {
    width: 200px;
    background: #f4f4f4;
    padding: 10px;
  }
  
  main {
    flex: 1;
    padding: 20px;
  }
  </style>