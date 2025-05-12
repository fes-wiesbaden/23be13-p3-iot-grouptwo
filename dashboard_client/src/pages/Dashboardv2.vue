<template>
  <div class="container">
    <div class="item camera-card">
      <CameraCard />
    </div>
    <div class="item sensor-card">
      <SensorCard />
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

        <!-- <DeviceCard class="device" type="lamp" name="Bedroom Lamp" :initialStatus="true" />
        <DeviceCard class="device" type="lamp" name="Kitchen Room Lamp" :initialStatus="true" />
        <DeviceCard class="device" type="wifi" name="Wi-Fi" :initialStatus="false" />
        <DeviceCard class="device" type="ac" name="Bedroom AC" :initialStatus="true" />
        <DeviceCard class="device" type="lamp" name="Bedroom Lamp" :initialStatus="true" />
        <DeviceCard class="device" type="ac" name="Bedroom AC" :initialStatus="true" />
        <DeviceCard class="device" type="ac" name="Kitchen AC" :initialStatus="true" /> -->

      </div>
    </div>
    <div class="item music-bar">
      <MusicCard />
    </div>
  </div>
</template>

<script>
import CameraCard from '@/components/CameraCard.vue';
import SensorCard from '@/components/SensorCard.vue';
import WeatherCard from '@/components/WeatherCard.vue';
import EnergyCard from '@/components/EnergyCard.vue';
import DeviceCard from '@/components/DeviceCard.vue';
import MusicCard from '@/components/MusicCard.vue';

export default {
  name: 'Dashboard',
  components: {
    CameraCard,
    SensorCard,
    WeatherCard,
    EnergyCard,
    DeviceCard,
    MusicCard
  },
  
  data() {
    return {
      socket: null,
      devices: [
        { id: 'led1', name: 'Living Room Lamp', type: 'lamp', status: false },
        { id: 'led2', name: 'Bedroom Lamp', type: 'lamp', status: false },
        { id: 'lcd1', name: 'lcd', type: 'lamp', status: false }
      ]
    };
  },
  created() {
    this.socket = new WebSocket('ws://10.93.138.100:3001');

    this.socket.onopen = () => {
      console.log('✅ WebSocket connected');
    };

    this.socket.onmessage = (event) => {
      const msg = JSON.parse(event.data);
      const device = this.devices.find(d => d.id === msg.deviceId);
      if (device) device.status = msg.status;
    };
  },
  methods: {
    toggleDevice(deviceId, newState) {
      const message = JSON.stringify({
        deviceId,
        command: newState ? 'ON' : 'OFF'
      });
      this.socket.send(message);
      console.log("messagesendtows: ", message);

      // Optimistically update status
      const device = this.devices.find(d => d.id === deviceId);
      if (device) device.status = newState;
    }
  }
};
</script>


<style scoped>
.container {
  display: grid;
  grid-template-columns: repeat(5, 1fr);
  grid-auto-rows: 200px;
  gap: 12px;
  padding: 16px;
  box-sizing: border-box;
}

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
