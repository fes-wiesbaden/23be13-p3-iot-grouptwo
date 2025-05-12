<template>
    <div class="card">
      <h3>{{ deviceName }}</h3>
      <!-- LightDimmer inside the card for controlling brightness -->
      <LightDimmer />
    </div>
  </template>
  
  <script setup lang="ts">
  import LightDimmer from './LightDimmer.vue';
  
  // Prop to accept device name dynamically
  const props = defineProps({
    deviceName: {
      type: String,
      required: true,
    },
  });
  </script>
  
  <style scoped>
  .card {
    background-color: #fff;
    border-radius: 8px;
    padding: 1rem;
    box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
    display: flex;
    flex-direction: column;
    align-items: center;
  }
  
  .card h3 {
    margin-bottom: 1rem;
  }
  </style>
  