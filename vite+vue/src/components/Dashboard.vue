<template>
    <div class="dashboard">

        <div class="dashboard">
    <FancyWeatherCard />
    <MusicCard />
    <!-- other dashboard content -->
  </div>
      <div v-for="category in categories" :key="category.name" class="category">
        <h2>{{ category.name }}</h2>
        <div class="device-cards">
          <div
            v-for="device in category.devices"
            :key="device.id"
            class="device-card"
            :class="[device.status, 'clickable']"
            @click="toggleDevice(device)"
          >
            <div class="device-card-header">
              <span class="device-icon">{{ device.icon }}</span>
              <span>{{ device.name }}</span>
            </div>
  
            <div class="device-status">
              <span>{{ device.status === 'on' ? 'ON' : 'OFF' }}</span>
            </div>
  
            <!-- Camera Thumbnail -->
            <div v-if="device.type === 'Camera'" class="device-camera">
              <a :href="device.fullImageUrl" target="_blank" @click.stop>
                <img :src="device.thumbnailUrl" alt="Camera Thumbnail" class="camera-thumbnail" />
              </a>
            </div>
  
            <!-- Fan Slider -->
            <div v-else-if="device.type === 'Fan'" class="device-slider" @click.stop>
              <input
                type="range"
                min="0"
                max="100"
                step="10"
                v-model="device.speed"
              />
              <span>Speed: {{ device.speed }}%</span>
            </div>
          </div>
        </div>
      </div>
    </div>
  </template>
  
  <script setup lang="ts">
  import { reactive } from 'vue';
  import { categories as originalCategories } from '@/data/devices';
  import FancyWeatherCard from '@/components/FancyWeatherCard.vue';
  import MusicCard from '@/components/MusicCard.vue';

  
  const categories = reactive(originalCategories);
  
  const toggleDevice = async (device: any) => {
  device.status = device.status === 'on' ? 'off' : 'on';

  // Send to Arduino if device is a Light
  if (device.type === 'Light') {
    try {
      await fetch('http://localhost:3001/light', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ name: device.name, status: device.status }),
      });
    } catch (error) {
      console.error(`Failed to toggle ${device.name}:`, error);
    }
  }
};

  </script>
  
  <style scoped>
  .dashboard {
    padding: 20px;
  }
  
  .category {
    margin-bottom: 20px;
  }
  
  .device-cards {
    display: flex;
    flex-wrap: wrap;
    gap: 20px;
  }
  
  .device-card {
    user-select: none;
    background-color: #e9eef5; /* Neutral base color */
    color: #1f2937; /* Dark text */
    border-radius: 12px;
    width: 250px;
    height: 220px;
    padding: 15px;
    text-align: center;
    display: flex;
    flex-direction: column;
    justify-content: space-between;
    box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
    cursor: pointer;
    transition: background-color 0.3s, transform 0.2s;
  }
  
  .device-card.on {
    background-color: #c6f6d5; /* Soft green */
    color: #1a202c;
  }
  
  .device-card.off {
    background-color: #fed7d7; /* Soft red */
    color: #1a202c;
  }
  
  .device-card:hover {
    transform: scale(1.03);
  }
  
  .device-card-header {
    font-size: 1.2rem;
    margin-bottom: 10px;
    display: flex;
    justify-content: center;
    align-items: center;
    gap: 8px;
  }
  
  .device-icon {
    font-size: 2rem;
  }
  
  .device-status {
    font-size: 1rem;
    font-weight: bold;
  }
  
  .device-slider input[type='range'] {
    width: 100%;
    accent-color: #3b82f6;
  }
  
  .camera-thumbnail {
    width: 100%;
    max-height: 100px;
    object-fit: cover;
    border-radius: 6px;
    cursor: pointer;
    transition: transform 0.2s;
  }
  
  .camera-thumbnail:hover {
    transform: scale(1.03);
  }
  </style>
  