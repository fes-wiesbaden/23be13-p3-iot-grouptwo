<template>
  <div class="sensor-grid">
    <div class="sensor-card" v-for="sensor in sensors" :key="sensor.name">
      <div class="value">
        {{ sensor.value }}<span class="unit" v-html="sensor.unit"></span>
      </div>
      <div class="label">{{ sensor.name }}</div>
      <div class="subtext">{{ sensor.type }}</div>
    </div>
  </div>
</template>


<script>
export default {
  name: 'SensorCard',
  data() {
    return {
      sensors: [
        { name: 'Living Room', value: 21, type: 'Temperature', unit: '°C' },
        { name: 'Outside', value: 44, type: 'Humidity', unit: '%' },
        { name: 'Water', value: 86, type: 'Consumption', unit: 'm<sup>3</sup>' },
        { name: 'Internet', value: 346, type: 'All Devices', unit: 'GB' }
      ]
    };
  }
};
</script>

<style scoped>
.sensor-grid {
  display: grid;
  grid-template-columns: repeat(2, 1fr);
  grid-template-rows: repeat(2, 1fr);
  gap: 12px;
  width: 100%;
  height: 100%;
  padding: 0;
  box-sizing: border-box;
}

.sensor-card {
  background-color: #fff;
  border-radius: 16px;
  padding: 16px;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.06);
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
}

.value {
  font-size: 36px;
  font-weight: 700;
  color: #2ecc71;
}

.unit {
  font-weight: 600;
  font-size: 16px;
  vertical-align: super;
  margin-left: 2px;
}

.label {
  font-size: 16px;
  font-weight: 600;
  color: #1e293b;
  margin-top: 8px;
}

.subtext {
  font-size: 14px;
  color: #64748b;
  margin-top: 2px;
}
</style>
