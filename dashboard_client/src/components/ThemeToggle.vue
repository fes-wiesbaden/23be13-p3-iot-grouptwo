<template>
    <button @click="toggleTheme" class="theme-toggle">
      {{ isDark ? '🌙 Dark Mode' : '☀️ Light Mode' }}
    </button>
  </template>
  
  <script setup lang="ts">
  import { ref, onMounted } from 'vue';
  
  const isDark = ref(false);
  
  // Toggle theme on click
  const toggleTheme = () => {
    isDark.value = !isDark.value;
    // Add or remove the 'dark' class on the <html> element
    document.documentElement.classList.toggle('dark', isDark.value);
    // Optionally, save theme preference in localStorage
    localStorage.setItem('theme', isDark.value ? 'dark' : 'light');
  };
  
  // Load saved theme on page load
  onMounted(() => {
    const savedTheme = localStorage.getItem('theme');
    if (savedTheme === 'dark') {
      isDark.value = true;
      document.documentElement.classList.add('dark');
    } else {
      isDark.value = false;
      document.documentElement.classList.remove('dark');
    }
  });
  </script>
  
  <style scoped>
  .theme-toggle {
    cursor: pointer;
    padding: 0.5rem 1rem;
    border-radius: 6px;
    background-color: #ddd;
    border: none;
  }
  </style>
  

  