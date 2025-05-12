<!-- src/components/LightDimmer.vue -->
<template>
  <div class="light-dimmer">
    <!-- Light Icon -->
    <div class="icon">
      <span v-if="lightIntensity === 0" class="material-icons">lightbulb_outline</span>
      <span v-else-if="lightIntensity < 50" class="material-icons">lightbulb</span>
      <span v-else class="material-icons">lightbulb_circle</span>
    </div>

    <!-- Dimmer Slider -->
    <input 
      type="range" 
      min="0" 
      max="100" 
      v-model="lightIntensity" 
      @input="onSliderInput" 
      class="slider" 
      :style="{ filter: `brightness(${lightIntensity}%)` }" 
    />

    <!-- Label with current intensity -->
    <div class="intensity-label">{{ lightIntensity }}%</div>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue';

const lightIntensity = ref(50); // Default light intensity at 50%

// Optional: You can use this method to perform extra actions on input change (e.g., saving state)
const onSliderInput = () => {
  console.log('Light intensity:', lightIntensity.value);
};
</script>

<style scoped>
.light-dimmer {
  display: flex;
  align-items: center;
  justify-content: center;
  flex-direction: column;
  padding: 1rem;
  width: 200px;
}

.icon {
  font-size: 3rem;
  transition: transform 0.3s ease;
}

.slider {
  -webkit-appearance: none;
  appearance: none;
  width: 80%;
  height: 10px;
  background: #ddd;
  border-radius: 5px;
  outline: none;
  cursor: pointer;
  transition: background 0.3s ease;
}

.slider:hover {
  background: #ccc;
}

.slider:active {
  background: #aaa;
}

.intensity-label {
  font-size: 1rem;
  margin-top: 10px;
  font-weight: bold;
  color: #555;
  transition: opacity 0.3s ease;
}

/* CSS Animation for the dimmer slider */
.icon {
  transition: transform 0.3s ease-in-out, opacity 0.3s ease-in-out;
}

.icon:hover {
  transform: scale(1.1);
  opacity: 0.8;
}
</style>
