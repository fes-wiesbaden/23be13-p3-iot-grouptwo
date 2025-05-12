<template>
  <div class="card">
    <div class="camera-buttons">
      <button
        v-for="(label, key) in cameras"
        :key="key"
        :class="{ active: selectedCamera === key }"
        @click="selectedCamera = key"
      >
        {{ label.label }}
      </button>
    </div>

    <div class="recording-label">
      <span class="dot"></span>
      <span>Recording...</span>
    </div>

    <img :src="cameras[selectedCamera].url" :alt="cameras[selectedCamera].label" />
  </div>
</template>

<script>
export default {
  name: 'CameraCard',
  data() {
    return {
      selectedCamera: 'living',
      cameras: {
        living: {
          label: 'Living Room',
          url: new URL('../assets/living_room.jpg', import.meta.url).href
        },
        door: {
          label: 'Main Door',
          url: new URL('../assets/main_door.jpg', import.meta.url).href
        },
        yard: {
          label: 'Yard',
          url: new URL('../assets/yard.jpg', import.meta.url).href
        }
      }
    };
  }
};
</script>

<style scoped>
.card {
  position: relative;
  width: 100%;
  height: 100%;
  overflow: hidden;
  border-radius: 12px;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
}

/* Camera image */
.card img {
  width: 100%;
  height: 100%;
  object-fit: cover;
  display: block;
  border-radius: 12px;
}

/* Recording label */
.recording-label {
  position: absolute;
  top: 12px;
  right: 12px;
  background-color: rgba(255, 255, 255, 0.85);
  padding: 6px 12px;
  border-radius: 20px;
  display: flex;
  align-items: center;
  font-weight: bold;
  font-size: 14px;
  color: #d62828;
  box-shadow: 0 2px 6px rgba(0, 0, 0, 0.1);
}

/* Red dot */
.dot {
  width: 10px;
  height: 10px;
  background-color: red;
  border-radius: 50%;
  margin-right: 8px;
  animation: pulse 1.2s infinite;
}

/* Pulse animation */
@keyframes pulse {
  0% {
    transform: scale(1);
    opacity: 1;
  }
  70% {
    transform: scale(1.4);
    opacity: 0.4;
  }
  100% {
    transform: scale(1);
    opacity: 1;
  }
}

/* Camera selection buttons */
.camera-buttons {
  position: absolute;
  top: 12px;
  left: 12px;
  display: flex;
  gap: 8px;
}

.camera-buttons button {
  padding: 6px 10px;
  border: none;
  background: rgba(255, 255, 255, 0.9);
  color: #333;
  border-radius: 12px;
  font-size: 12px;
  font-weight: bold;
  cursor: pointer;
  box-shadow: 0 2px 4px rgba(0,0,0,0.1);
  transition: background 0.2s;
}

.camera-buttons button.active,
.camera-buttons button:hover {
  background: #56cc9d;
  color: white;
}
</style>
