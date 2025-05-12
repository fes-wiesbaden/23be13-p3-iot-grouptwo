<template>
    <section class="room-section">
      <h2>{{ name }}</h2>
      <div class="device-grid">
        <DeviceCard
          v-for="device in devices"
          :key="device.id"
          :name="device.name"
          :icon="device.icon"
        />
      </div>
    </section>
  </template>
  
  <script setup lang="ts">
  import DeviceCard from './DeviceCard.vue';
  
  const props = defineProps<{
    name: string;
    devices: Array<{
      id: string;
      name: string;
      icon: string;
    }>;
  }>();
  </script>
  
  <style scoped>
  .device-grid {
    display: flex;
    gap: 1rem;
    flex-wrap: wrap;
  }
  </style>
  