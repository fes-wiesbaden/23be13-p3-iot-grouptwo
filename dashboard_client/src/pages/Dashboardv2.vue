<template>
  <div class="dashboard-layout">
    <!-- LEFT: Main Dashboard -->
    <div class="container">
      <!-- 🔔 Popup Notification -->
      <transition name="fade">
        <div v-if="showPopup" class="popup-notification">
          {{ latestNotification }}
        </div>
      </transition>

      <!-- Cards -->
      <div class="item camera-card">
        <CameraCard />
      </div>
      <div class="item sensor-card">
        <SensorCard :liveTemperature="temperature" :liveHumidity="humidity" />
      </div>
      <div class="item weather-card">
        <WeatherCard />
      </div>
      <div class="item energy-card">
        <EnergyCard />
      </div>
      <div class="item devices-card">
        <div class="devices">
          <DeviceCard
            v-for="device in devices"
            :key="device.id"
            :name="device.name"
            :type="device.type"
            :isOn="device.status"
            @toggle="toggleDevice(device.id, $event)"
          />
          
          <div>
    <CircularRangeSlider v-model="fanSpeed" />
    <p>Fan Speed: {{ fanSpeed }}%</p>
  </div>
        </div>
      </div>
      <div class="item music-bar">
        <MusicCard />
      </div>
    </div>

    <!-- RIGHT: Notification Sidebar -->
    <div class="notification-sidebar">
      <h4>🔔 Notifications</h4>
      <ul>
        <li v-for="(note, index) in notifications" :key="index">
          <strong>{{ note.timestamp }}</strong><br />
          {{ note.message }}
        </li>
      </ul>
    </div>
  </div>
</template>

<script>
import CameraCard from '@/components/CameraCard.vue';
import SensorCard from '@/components/SensorCard.vue';
import WeatherCard from '@/components/WeatherCard.vue';
import EnergyCard from '@/components/EnergyCard.vue';
import DeviceCard  from '@/components/DeviceCard.vue';
import MusicCard from '@/components/MusicCard.vue';
import Slider from '@/components/CircularRangeSlider.vue';

export default {
  name: 'Dashboard',
  components: {
    CameraCard,
    SensorCard,
    WeatherCard,
    EnergyCard,
    DeviceCard,
    MusicCard
    ,Slider
  },

  data() {
    return {
      socket: null,
      temperature: null,
      humidity: null,
      devices: [
        { id: 'led1', name: 'Living Room Lamp', type: 'lamp', status: false },
        { id: 'led2', name: 'Bedroom Lamp', type: 'lamp', status: false },
        { id: 'lcd1', name: 'LCD Display', type: 'lcd', status: false },
        { id: 'servo', name: 'Curtain Servo', type: 'servo', status: false },
        { id: 'Grage/Tor', name: 'Garage Door', type: 'servo', status: false}
      ],
      notifications: [],
      showPopup: false,
      latestNotification: '',
temperature: 60    };
  },

  created() {
    this.socket = new WebSocket(`ws://${window.location.hostname}:3001`);

    this.socket.onopen = () => {
      console.log('✅ WebSocket connected');
      this.addNotification(`✅ WebSocket connected`)
    };

    this.socket.onmessage = (event) => {
      const msg = JSON.parse(event.data);

      if (msg.type === 'status' && msg.device === 'temp') {
        this.temperature = parseFloat(msg.value);
        // this.addNotification(`🌡️ Temperature updated: ${this.temperature}°C`);
      }

      if (msg.type === 'status' && msg.device === 'humidity') {
        this.humidity = parseFloat(msg.value);
        // this.addNotification(`💧 Humidity updated: ${this.humidity}%`);
      }

      if (msg.type == 'status' && msg.device == 'FlameSensore'){
        this.addNotification(`*** Flame in Livingroom Detected!!! ***`);
        console.log(`*** Flame in Livingroom Detected!!! ***`);
      }

      if (msg.type === 'status' && msg.device === 'NumPadLock') {
        this.addNotification(`💧 Door status: ${msg.value}`);
      }

      if (msg.type === 'status' && msg.deviceId) {
        const device = this.devices.find(d => d.id === msg.deviceId);
        if (device) {
          device.status = msg.value === 'ON' || msg.status === true;
          this.addNotification(`🔌 ${device.name} is now ${device.status ? 'ON' : 'OFF'}`);
        }
      }

      if (msg.type == 'status' && msg.deviceId){
        const device = this.devices.find(d = d.id === msg.deviceId );
        if (device){
          device.status = msg.value == 'ON' || msg.status === true;
          this.addNotification(`🚪 ${device.name} is now ${device.status}`);
        }
      }
    };
  },

  methods: {
    toggleDevice(deviceId, newState) {
      const device = this.devices.find(d => d.id === deviceId);
      if (!device) return;

      const message = JSON.stringify({
        deviceId: device.id,
        deviceType: device.type,
        command: newState ? 'ON' : 'OFF'
      });

      this.socket.send(message);
      console.log("📤 Sent WS message:", message);

      device.status = newState;
      this.addNotification(`⚙️ Sent command to ${device.name}: ${newState ? 'ON' : 'OFF'}`);
    },

    addNotification(message) {
      this.latestNotification = message;
      this.notifications.unshift({
        message,
        timestamp: new Date().toLocaleString()
      });
      this.showPopup = true;

      setTimeout(() => {
        this.showPopup = false;
      }, 3000);
    }
  }
};
</script>

<style scoped>
/* Layout wrapper: dashboard and notification bar side by side */
.dashboard-layout {
  display: flex;
  flex-direction: row;
  height: 100vh;
  overflow: hidden;
}

/* Dashboard takes remaining space */
.container {
  flex: 1;
  display: grid;
  grid-template-columns: repeat(5, 1fr);
  grid-auto-rows: 200px;
  gap: 12px;
  padding: 16px;
  box-sizing: border-box;
  overflow-y: auto;
}

/* Notification Sidebar on the right */
.notification-sidebar {
  width: 300px;
  background: #f9f9f9;
  border-left: 1px solid #ccc;
  padding: 16px;
  box-shadow: -2px 0 5px rgba(0, 0, 0, 0.1);
  overflow-y: auto;
  font-size: 14px;
}

.notification-sidebar h4 {
  margin-top: 0;
  margin-bottom: 10px;
}

.notification-sidebar ul {
  list-style: none;
  padding: 0;
  margin: 0;
}

.notification-sidebar li {
  padding: 6px 0;
  border-bottom: 1px solid #e0e0e0;
}

/* Popup Notification */
.popup-notification {
  position: fixed;
  top: 20px;
  right: 270px; /* float left of sidebar */
  background: #007bff;
  color: white;
  padding: 12px 20px;
  border-radius: 6px;
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
  z-index: 1000;
}

.fade-enter-active, .fade-leave-active {
  transition: opacity 0.5s;
}
.fade-enter, .fade-leave-to {
  opacity: 0;
}

/* Grid areas */
.camera-card {
  grid-column: 1 / 4;
  grid-row: 1 / 3;
}
.sensor-card {
  grid-column: 4 / 6;
  grid-row: 1 / 3;
}
.weather-card {
  grid-column: 1 / 4;
  grid-row: 3 / 4;
}
.energy-card {
  grid-column: 4 / 6;
  grid-row: 3 / 4;
}
.devices-card {
  grid-column: 1 / 6;
  grid-row: 4 / 5;
}
.music-bar {
  margin: 10px;
  padding-top: 30px;
  grid-column: 1 / 6;
  grid-row: 6 / 7;
}

.devices {
  display: flex;
  flex-direction: row;
  flex-wrap: wrap;
}
.device {
  flex: 0 0 18%;
  box-sizing: border-box;
  padding: 20px;
}
</style>
