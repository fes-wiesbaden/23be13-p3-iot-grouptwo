<!-- src/components/NotificationBar.vue -->
<template>
    <div v-if="message" class="notification-bar" :class="type">
      {{ message }}
    </div>
  </template>
  
  <script setup lang="ts">
  import { defineProps } from 'vue';
  
  const props = defineProps<{
    message: string;
    type?: 'info' | 'success' | 'error';
  }>();
  </script>
  
  <style scoped>
  .notification-bar {
    padding: 1rem;
    text-align: center;
    font-weight: bold;
  }
  .info {
    background-color: #dbeafe;
    color: #1e3a8a;
  }
  .success {
    background-color: #d1fae5;
    color: #065f46;
  }
  .error {
    background-color: #fee2e2;
    color: #991b1b;
  }
  </style>
  