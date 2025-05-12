<template>
  <div class="music-card">
    <div class="glass">
      <div class="music-icon">
        <svg
          viewBox="0 0 16 16"
          class="note"
          fill="currentColor"
          height="20"
          width="20"
          xmlns="http://www.w3.org/2000/svg"
        >
          <path d="M9 13c0 1.105-1.12 2-2.5 2S4 14.105 4 13s1.12-2 2.5-2 2.5.895 2.5 2z"/>
          <path d="M9 3v10H8V3h1z" fill-rule="evenodd"/>
          <path d="M8 2.82a1 1 0 0 1 .804-.98l3-.6A1 1 0 0 1 13 2.22V4L8 5V2.82z"/>
        </svg>
      </div>
      <div class="track-info">
        <span class="track-title">One Piece First Ending</span>
        <span class="track-artist">Desconhecido</span>
      </div>
      <div class="controls">
        <!-- Prev Button -->
        <svg class="icon" viewBox="0 0 16 16" fill="currentColor" xmlns="http://www.w3.org/2000/svg">
          <path d="M7.596 7.304a.802.802 0 0 1 0 1.392l-6.363 3.692C.713 12.69 0 12.345 0 11.692V4.308c0-.653.713-.998 1.233-.696l6.363 3.692Z"/>
          <path d="M15.596 7.304a.802.802 0 0 1 0 1.392l-6.363 3.692C8.713 12.69 8 12.345 8 11.692V4.308c0-.653.713-.998 1.233-.696l6.363 3.692Z"/>
        </svg>

        <!-- Play/Pause Toggle -->
        <svg v-if="!isPlaying" @click="togglePlay" class="icon pulse" viewBox="0 0 16 16" fill="currentColor"
             xmlns="http://www.w3.org/2000/svg">
          <path d="m12.14 8.753-5.482 4.796c-.646.566-1.658.106-1.658-.753V3.204a1 1 0 0 1 1.659-.753l5.48 4.796a1 1 0 0 1 0 1.506z"/>
        </svg>
        <svg v-else @click="togglePlay" class="icon pulse" viewBox="0 0 16 16" fill="currentColor"
             xmlns="http://www.w3.org/2000/svg">
          <path d="M5.5 3.5A.5.5 0 0 1 6 3h1a.5.5 0 0 1 .5.5v9a.5.5 0 0 1-.5.5H6a.5.5 0 0 1-.5-.5v-9zm4 0a.5.5 0 0 1 .5-.5h1a.5.5 0 0 1 .5.5v9a.5.5 0 0 1-.5.5h-1a.5.5 0 0 1-.5-.5v-9z"/>
        </svg>

        <!-- Next Button -->
        <svg class="icon" viewBox="0 0 16 16" fill="currentColor" xmlns="http://www.w3.org/2000/svg">
          <path d="M7.596 7.304a.802.802 0 0 1 0 1.392l-6.363 3.692C.713 12.69 0 12.345 0 11.692V4.308c0-.653.713-.998 1.233-.696l6.363 3.692Z"/>
          <path d="M15.596 7.304a.802.802 0 0 1 0 1.392l-6.363 3.692C8.713 12.69 8 12.345 8 11.692V4.308c0-.653.713-.998 1.233-.696l6.363 3.692Z"/>
        </svg>
      </div>

      <div class="options">
        <svg class="icon small" viewBox="0 0 16 16" fill="currentColor" xmlns="http://www.w3.org/2000/svg">
          <path d="M0 3.5A.5.5 0 0 1 .5 3H1c2.2 0 3.8 1.2 4.9 2.4.5.6.9 1.1 1.1 1.5.3-.4.6-.9 1.1-1.5C9.2 4.2 10.8 3 13 3v1c-1.8 0-3.2 1-4.1 2.1A9.6 9.6 0 0 0 7.6 8a9.6 9.6 0 0 0 1.3 1.9C9.8 11 11.2 12 13 12v1c-2.2 0-3.8-1.2-4.9-2.4a10.6 10.6 0 0 1-1.1-1.5c-.3.4-.6.9-1.1 1.5C4.8 11.8 3.2 13 1 13H.5a.5.5 0 0 1 0-1H1c1.8 0 3.2-1 4.1-2.1A9.6 9.6 0 0 0 6.4 8a9.6 9.6 0 0 0-1.3-1.9C4.2 5 2.8 4 1 4H.5a.5.5 0 0 1-.5-.5z"/>
          <path d="M13 5.5V1.5a.25.25 0 0 1 .41-.2l2.36 2a.25.25 0 0 1 0 .4l-2.36 2a.25.25 0 0 1-.41-.2zM13 14.5v-4a.25.25 0 0 1 .41-.2l2.36 2a.25.25 0 0 1 0 .4l-2.36 2a.25.25 0 0 1-.41-.2z"/>
        </svg>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref } from 'vue'
const isPlaying = ref(false)

const togglePlay = () => {
  isPlaying.value = !isPlaying.value
}
</script>

<style scoped>
.music-card {
  width: 100%;
  height: 100px;
  background: #1f1f1f;
  border-radius: 12px;
  overflow: hidden;
  transition: transform 0.3s ease;
}
.music-card:hover {
  transform: scale(1.01);
}

.glass {
  display: flex;
  align-items: center;
  justify-content: space-between;
  background: rgba(40, 40, 40, 0.85);
  box-shadow: 0 8px 32px 0 rgba(0, 0, 0, 0.5);
  backdrop-filter: blur(8.5px);
  -webkit-backdrop-filter: blur(8.5px);
  border-radius: 12px;
  padding: 10px 20px;
  height: 100%;
  width: 100%;
  box-sizing: border-box;
}

.music-icon {
  background: rgba(70, 70, 70, 0.6);
  border-radius: 10px;
  padding: 10px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.track-info {
  flex-grow: 1;
  padding: 0 20px;
  display: flex;
  flex-direction: column;
  justify-content: center;
}

.track-title {
  font-size: 14px;
  font-weight: 600;
  color: #ffffff;
}

.track-artist {
  font-size: 11px;
  color: #bbbbbb;
}

.controls {
  display: flex;
  gap: 12px;
  align-items: center;
}

.options {
  display: flex;
  gap: 10px;
  margin-left: 20px;
}

.icon {
  fill: #ffffffcc;
  cursor: pointer;
  transition: transform 0.2s ease, fill 0.2s;
}
.icon:hover {
  transform: scale(1.15);
  fill: #ffffff;
}
.icon.small {
  height: 14px;
  width: 14px;
}

.pulse {
  animation: pulse 2s infinite ease-in-out;
}

@keyframes pulse {
  0% { transform: scale(1); opacity: 0.9; }
  50% { transform: scale(1.05); opacity: 1; }
  100% { transform: scale(1); opacity: 0.9; }
}
</style>
