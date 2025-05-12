<template>
  <div class="card" :class="weatherType.toLowerCase()">
    <div class="left">
      <h3 class="title">Weather Today</h3>
      <div class="weather-info">
        <img :src="weatherIcon" class="icon" alt="Weather icon" />
        <div class="details">
          <p class="location">{{ location }} - {{ temperature }}°C</p>
          <p class="desc">{{ description }}</p>
        </div>
      </div>
    </div>
    <div class="right">
      <p class="time">{{ currentTime }}</p>
    </div>
  </div>
</template>

<script setup>
import { ref, computed, onMounted } from 'vue';
import sunIcon from '@/assets/sun.png';
import moonIcon from '@/assets/moon.png';

const temperature = ref(null);
const description = ref('');
const location = ref('Loading...');
const weatherType = ref('Day');
const currentTime = ref(getTime());

// 🔐 Replace this with your actual OpenWeatherMap API key
const API_KEY = '37b0645da2284a209f1190815250605';

function getTime() {
  const now = new Date();
  return now.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
}

function getWeather(lat, lon) {
  const url = `https://api.weatherapi.com/v1/current.json?key=${API_KEY}&q=${lat},${lon}&aqi=no`;

  fetch(url)
    .then(res => res.json())
    .then(data => {
      console.log('WeatherAPI response:', data);

      if (data.location && data.current) {
        temperature.value = Math.round(data.current.temp_c);
        description.value = data.current.condition.text;
        location.value = data.location.name;

        const hour = new Date().getHours();
        weatherType.value = hour >= 18 || hour < 6 ? 'Night' : 'Day';
      } else {
        console.error('Unexpected response format:', data);
        location.value = 'Unknown location';
        description.value = 'Unable to fetch weather';
      }
    })
    .catch(err => {
      console.error('Weather fetch failed:', err);
      location.value = 'Error';
      description.value = 'Could not load data';
    });
}


onMounted(() => {
  if (navigator.geolocation) {
    navigator.geolocation.getCurrentPosition(
      position => {
        const { latitude, longitude } = position.coords;
        getWeather(latitude, longitude);
      },
      error => {
        console.error('Geolocation error:', error);
        // Fallback coordinates (e.g., Wiesbaden)
        getWeather(50.0782, 8.2398);
      }
    );
  } else {
    console.warn('Geolocation not supported');
    getWeather(50.0782, 8.2398); // Fallback
  }

  setInterval(() => {
    currentTime.value = getTime();
  }, 60000);
});

const weatherIcon = computed(() =>
  weatherType.value === 'Night' ? moonIcon : sunIcon
);
</script>

<style scoped>
.card {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 24px;
  border-radius: 16px;
  height: 100%;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.08);
  transition: background 0.3s ease, color 0.3s ease;
}

.card.day {
  background: #f0f8ff;
  color: #333;
}

.card.night {
  background: #1e293b;
  color: #f8fafc;
}

.left {
  display: flex;
  flex-direction: column;
  gap: 12px;
  flex: 2;
}

.weather-info {
  display: flex;
  align-items: center;
  gap: 16px;
}

.icon {
  width: 60px;
  height: 60px;
  animation: float 4s ease-in-out infinite;
}

.details {
  display: flex;
  flex-direction: column;
}

.location {
  font-size: 22px;
  font-weight: bold;
}

.desc {
  font-size: 16px;
  opacity: 0.8;
}

.right {
  flex: 1;
  display: flex;
  justify-content: flex-end;
  align-items: center;
}

.time {
  font-size: 60px;
  font-weight: 600;
}

@keyframes float {
  0%, 100% {
    transform: translateY(0);
  }
  50% {
    transform: translateY(-6px);
  }
}
</style>
