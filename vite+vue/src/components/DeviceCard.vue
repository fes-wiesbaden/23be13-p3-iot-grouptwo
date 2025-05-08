<template>
    <div class="device-card" @mouseover="onMouseOver" @mouseleave="onMouseLeave">
      <div class="device-header">
        <span class="device-title">{{ device.name }}</span>
        <span class="device-icon">{{ device.icon }}</span> <!-- Display device's icon -->
      </div>
      <div class="device-details">
        <p>Status: <span :class="deviceStatusClass">{{ device.status }}</span></p> <!-- Conditional status styling -->
      </div>
    </div>
  </template>
  
  <script lang="ts">
  import { defineComponent, PropType } from 'vue';
  
  interface Device {
    id: number;
    name: string;
    type: string;
    icon: string;
    status: string;
  }
  
  export default defineComponent({
    name: 'DeviceCard',
    props: {
      device: {
        type: Object as PropType<Device>,
        required: true
      }
    },
    computed: {
      // Dynamically set class for the status
      deviceStatusClass() {
        return this.device.status === 'on' ? 'status-on' : 'status-off';
      }
    },
    methods: {
      onMouseOver() {
        // You can add hover logic here
        console.log('Hovered over device card!');
      },
      onMouseLeave() {
        // You can add mouse leave logic here
        console.log('Mouse left device card!');
      }
    }
  });
  </script>
  
  <style scoped>
  .device-card {
    user-select: none;
    background-color: #f4f4f4;
    border-radius: 8px;
    padding: 15px;
    margin: 10px;
    display: flex;
    flex-direction: column;
    transition: all 0.3s ease;
    cursor: pointer;
    box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
  }
  
  .device-card:hover {
    transform: scale(1.05);
  }
  
  .device-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
  }
  
  .device-title {
    font-size: 18px;
    font-weight: bold;
  }
  
  .device-icon {
    font-size: 24px; /* Icon size */
  }
  
  .device-details {
    margin-top: 10px;
    font-size: 14px;
    color: #555;
  }
  
  .status-on {
    color: green;
  }
  
  .status-off {
    color: red;
  }
  </style>
  